# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
"""Maya Math Expression Lexer

Expression Language Rules:
  - single line only
  - 
"""
import collections
import copy
import functools
import re

import maya.cmds as cmds

from info import *

OPERATORS = ['+', '/', '%', '*', '-']
CONDITION = ['<', '>', '=', '!']
TERNARY = ['?', ':']

PRECEDENCE = {
    "<": 10, ">": 10, "<=": 10, ">=": 10, "==": 10, "!=": 10,
    "+": 20, "-": 20,
    "*": 30, "/": 30, "%": 30,
}

Token = collections.namedtuple('Token', ['type', 'value'])
Attribute = collections.namedtuple('Attribute', ['type', 'value'])

BracketToken = 0
NumberToken = 1
OperatorToken = 2
ConditionToken = 3
TernaryToken = 4
StringToken = 6
FunctionToken = 7
CommaToken = 8

class Number(object):
    def __init__(self, value):
        self.value = value

        if isinstance(value, list):
            if len(value) == 3:
                self.type = 'double3'
            elif len(value) == 4:
                self.type = 'double4'
            else:
                self.type = 'matrix'
            self.value = [float(v) for v in value]
        else:
            self.type = 'double' if '.' in value else 'int'
            self.value = float(value) if self.type == 'double' else int(value)
    
    def negate(self):
        self.value = self.value * -1
    
    def __repr__(self):
        return '(Number: {0}({1}))'.format(self.type, self.value)

class String(object):
    def __init__(self, value, index=None):
        self._value = value
        self.index = index
    
    @property
    def value(self):
        return '{0}{1}'.format(self._value, '[{0}]'.format(self.index) if self.index else '')
    
    def __repr__(self):
        return '(String: {0})'.format(self.value)

class Binary(object):
    def __init__(self, value, left, right):
        self.value = value
        self.left = left
        self.right = right
    
    def __repr__(self):
        return '(Binary: {0} {1} {2})'.format(self.left, self.value, self.right)

class Conditional(object):
    def __init__(self, value, left, right, true, false):
        self.value = value
        self.left = left
        self.right = right
        self.true = true
        self.false = false
    
    def __repr__(self):
        return '(Conditional: {0} {1} {2} ? {3} : {4})'.format(self.left, self.value, self.right, self.true, self.false)

class Function(object):
    def __init__(self, value, args, index=None):
        self.value = value
        self.args = args
        self.index = index
    
    def __repr__(self):
        return '(Function: {0}({1}){2})'.format(self.value, self.args, '[{0}]'.format(self.index) if self.index else '')

class ExpressionStream(object):
    """Expression Stream"""
    def __init__(self, input_str):
        self._data = input_str
        self._pos = 0

    def next(self):
        """Read next character in the stream"""
        char = self._data[self._pos]
        self._pos += 1
        return char
    
    def peek(self):
        """Look ahead at next character in the stream"""
        if self.end():
            return None
        
        return self._data[self._pos]
    
    def end(self):
        """Check for end of expression"""
        return self._pos == len(self._data)
    
    def error(self, message):
        raise RuntimeError('{0} at {1}'.format(message, self._pos))


class ExpressionLexer(object):
    """Expression Lexer"""
    def __init__(self, input_stream):
        self._data = ExpressionStream(input_stream)
        self._current = None
        self._next = None
    
    def next(self):
        """Read next token from stream"""
        if self._next:
            self._current = self._next
            self._next = None
        else:
            self._current = self.read_next()
        return self._current

    def peek(self):
        """Look ahead at next token in stream"""
        if not self._next:
            self._next = self.read_next()
        return self._next
    
    def end(self):
        """Check for end of expression"""
        return self.peek() == None
    
    def error(self, message):
        self._data.error(message)

    def read_next(self):
        """Read next token from stream"""
        self.read_while(self.is_whitespace)
        if self._data.end():
            return None

        char = self._data.peek()
        if self.is_string(char):
            return self.read_string()
        if self.is_digit(char):
            return self.read_number()
        if self.is_operator(char):
            return self.read_operator()
        if self.is_conditional(char):
            return self.read_conditional()
        if self.is_ternary(char):
            return self.read_ternary()
        if self.is_bracket(char):
            return self.read_bracket()
        if self.is_open_curly(char):
            return self.read_curly()
        if char == ',':
            return Token(CommaToken, self._data.next())
        
        self._data.error('Failed to handle character: {0}'.format(char))

    def read_while(self, predicate):
        """Read from stream while condition of the predicate is met"""
        str = ''
        while not self._data.end() and predicate(self._data.peek()):
            str += self._data.next()

        return str
    
    def is_whitespace(self, char):
        """Check for whitespace character"""
        return char == ' '

    def is_digit(self, char, dot_ok=False):
        """Check for digit character"""
        if dot_ok and char == '.':
            return True
        return char.isdigit()
    
    def read_number(self, negate=''):
        """Read number from stream"""
        return Token(NumberToken, negate + self.read_while(functools.partial(self.is_digit, dot_ok=True)))

    def is_operator(self, char):
        """Check for operator character"""
        return char in OPERATORS

    def read_operator(self):
        """Read operator from stream"""
        return Token(OperatorToken, self.read_while(self.is_operator))

    def is_conditional(self, char):
        """Check for conditional character"""
        return char in CONDITION

    def read_conditional(self):
        """Read conditional from stream"""
        return Token(ConditionToken, self.read_while(self.is_conditional))
    
    def is_ternary(self, char):
        """Check for ternary character"""
        return char in TERNARY

    def read_ternary(self):
        """Read ternary from stream"""
        return Token(TernaryToken, self.read_while(self.is_ternary))

    def is_bracket(self, char):
        """Check for bracket character"""
        return char in ['(', ')', '[', ']']
    
    def read_bracket(self):
        """Read bracket from stream"""
        return Token(BracketToken, self._data.next())
    
    def is_string(self, char):
        """Check for string character"""
        return char.isalpha() or char == '.'
    
    def read_string(self):
        """Read string from stream"""
        return Token(StringToken, self.read_while(self.is_string))
    
    def is_open_curly(self, char):
        """Check for open curly brance character"""
        return char == '{'
    
    def is_not_close_curly(self, char):
        """Check for close curly brance character"""
        return char != '}'
    
    def read_curly(self):
        """Read curly brace value from stream"""
        self._data.next()  # ear open brace
        value = self.read_while(self.is_not_close_curly)
        
        if self._data.peek() != '}':
            self.error('Expected closing curly brance, got "{0}" instead'.format(self._data.peek()))

        value = value.replace(' ', '').split(',')
        if len(value) not in [3, 4, 16]:
            self.error('Expected a vector, quaternion or matrix, got "{0}" instead'.format(value))
        
        self._data.next()  # ear close brace
        return Token(NumberToken, value)

class ExpressionParser(object):
    def __init__(self, token_stream):
        self._data = ExpressionLexer(token_stream)

    @property
    def token(self):
        return self._data._current

    def error(self, message):
        self._data.error(message)

    def parse(self):
        """Parse the entire expression"""
        self._data.next()
        return self.parse_expression()

    def get_precedence(self):
        """Get operator precedence from current token"""
        if not self.token or self.token.type is not OperatorToken:
            return -1
        
        return PRECEDENCE[self.token.value]

    def parse_number(self):
        """Parse current token as number
        
        Consumes current token!
        """
        num = Number(self.token.value)
        self._data.next()
        return num

    def parse_string(self):
        """Parse current token as string

        Consumes current token!
        """
        string = String(self.token.value)
        self._data.next()

        if self.token and self.token.value == '[':
            self._data.next()  # consume open bracket
            if not self.token or self.token.type != NumberToken:
                self.error('Expected a numeric index, got "{0}" instead'.format(self.token))
            string.index = self.token.value
            self._data.next() # consume index bracket
            
            if not self.token or self.token.value != ']':
                self.error('Expected a closing bracket, got "{0}" instead'.format(self.token))
            self._data.next() # consume close bracket

        return string

    def parse_element(self):
        """Parse token element
        
        Consumes current token!
        """
        if not self.token:
            self.error('Expected a valid token, got "None" instead')
        
        if self.token.type == NumberToken:
            return self.parse_number()
        elif self.token.type == StringToken:
            # special case for function calls
            next = self._data.peek()
            if next and next.type == BracketToken and next.value == '(':
                return self.parse_function()
            return self.parse_string()
        elif self.token.type == FunctionToken:
            return self.parse_function()
        elif self.token.type == BracketToken and self.token.value == '(':
            return self.parse_parentheses()
        elif self.token.type == OperatorToken and self.token.value == '-':
            # special case for negative values
            self._data.next()  # consume negate
            
            if self.token.type == NumberToken:
                number = self.parse_number()
                number.negate()
                return number
            else:
                self.error('Expected a number after negate, got "{0}" instead'.format(self.token.value))
        else:
            self.error('Could not handle token "{0}"'.format(self.token))
    
    def parse_expression(self):
        """Parse expression"""
        left = self.parse_element()
        
        if self.token and self.token.type == OperatorToken:
            left = self.parse_binary_right(0, left)
        
        if self.token and self.token.type == ConditionToken:
            left = self.parse_conditional(left)

        # if self.token:
        #     self.error('Expected end of expression or another operator, got "{0}" instead'.format(self.token))

        return left
    
    def parse_parentheses(self):
        """Parse parentheses"""
        self._data.next()  # consume open paren
        result = self.parse_expression()

        if not result:
            self.error('Encountered empty parentheses')

        if not self.token or self.token.value != ')':
            self.error('Expected closing parenthesis, got "{0}" instead'.format(self.token))
        
        self._data.next()  # consume closing paren
        return result
    
    def parse_function(self):
        """Parse function expression"""
        function = self.token.value
        self._data.next()  # consume function
        
        if not self.token or self.token.value != '(':
            self.error('Expected function call parentheses, got "{0}" instead'.format(self.token))
        
        args = []
        args_nested = []
        arg_is_list = False
        self._data.next()  # consume open paren
        while True:
            if self.token and self.token.value == '[':
                arg_is_list = True
                self._data.next()  # consume open bracket
            
            arg = self.parse_expression()
            if not arg:
                self.error('Expected a valid argument, got "None" instead')
            
            if arg_is_list:
                args_nested.append(arg)
            else:
                args.append(arg)
            
            if self.token and self.token.value == ']':
                self._data.next()  # consume close bracket
                
                if self.token and self.token.value in [',', ')']:
                    prev_token = self.token
                    self._data.next()  # consume comma or close paren
                    args.append(args_nested)
                    arts_nested = []
                    arg_is_list = False
                    if prev_token.value == ')':
                        break
                else:
                    self.error('Expected comma or closing parenthesis, got "{0}" instead'.format(next)) 
            elif self.token and self.token.value == ')':
                self._data.next()  # consume close paren
                break
            elif self.token and self.token.value == ',':
                self._data.next()  # consume comma
            else:
                self.error('Expected closing parenthesis, got "{0}" instead'.format(self.token))
        
        index = None
        if self.token and self.token.value == '[':
            self._data.next()  # consume open bracket
            if not self.token or self.token.type != NumberToken:
                self.error('Expected a numeric index, got "{0}" instead'.format(self.token))
            index = self.token.value
            self._data.next()  # consume index

            if not self.token or self.token.value != ']':
                self.error('Expected a closing bracket, got "{0}" instead'.format(self.token))
            self._data.next()  # consume close bracket

        return Function(function, args, index)

    def parse_binary_right(self, prec, left):
        """Parse binary expression with precendence
        
        Consumes current operator token!
        """
        while True:
            left_prec = self.get_precedence()
            if left_prec < prec:
                return left
            
            op_value = self.token.value
            self._data.next()  # consume op

            right = self.parse_element()
            if left_prec < self.get_precedence():
                right = self.parse_binary_right(left_prec + 1, right)
            
            left = Binary(op_value, left, right)

    def parse_conditional(self, left):
        """Parse conditional expression"""
        op_value = self.token.value
        self._data.next()  # consume op

        right = self.parse_expression()
        if not self.token.type == TernaryToken:
            self.error('Expected ternary operator, got "{0}" instead'.format(self.token.value))
        
        self._data.next()  # consume op
        true = self.parse_expression()
        false = None
        if self.token.type == TernaryToken:
            self._data.next()  # consume op
            false = self.parse_expression()
        
        return Conditional(op_value, left, right, true, false)

# str = '{0, 1, 0, 1} * 3'
# str = '1.0 + node.attr > 55 - 2 ? 2.5 + 3 : node.attr * 1'

# str = 'powers(2, node.attr[0])[1] - 12.3'

# str = '1 < 2.5 ? 3 : 1'

# exp = ExpressionParser(str).parse()
# print exp

class NameGenerator(object):
    """Name Generator"""
    def __init__(self, base):
        self._base = base
        
        self._counter_per_type = dict()
        for node_type in [value['name'] for value in FUNCTIONS.values()]:
            self._counter_per_type[node_type] = 0
    
    def get_name(self, node_type):
        """Get last name for node of given type"""
        index = self._counter_per_type[node_type]
        suffix = node_type.split('_')[1][:3].upper()
        return '{0}{1}_{2}'.format(self._base, index, suffix)
    
    def create_name(self, node_type):
        """Create unique name for node of given type"""
        index = self._counter_per_type[node_type]
        self._counter_per_type[node_type] = index + 1
        suffix = node_type.split('_')[1][:3].upper()
        
        return '{0}{1}_{2}'.format(self._base, index, suffix)

class ExpresionBuilder(object):
    """Expression Builder"""
    def __init__(self, base_name):
        self._nodes = []
        self._connections = []
        self._values = []
        self._namer = NameGenerator(base_name)
    
    def error(self, message):
        raise RuntimeError(message)
    
    def generate(self, ast):
        """Generate Maya data from AST"""
        if isinstance(ast, Binary):
            return self.generate_binary(ast)
        elif isinstance(ast, Conditional):
            return self.generate_conditional(ast)
        elif isinstance(ast, Function):
            return self.generate_function(ast)
        else:
            return ast
    
    def set_node_values(self, attr, input):
        """Set node values"""
        if isinstance(input, Number):
            self._values.append((attr, input.value))
        elif isinstance(input, String) or isinstance(input, Attribute):
            self._connections.append((input.value, attr))
        elif isinstance(input, basestring):
            self._connections.append((input, attr))

    def get_value_type(self, input):
        """Get value type"""
        if isinstance(input, Number) or isinstance(input, Attribute):
            return input.type
        elif isinstance(input, String):
            attr_type = cmds.getAttr(input.value, type=True)
            if attr_type == 'doubleLinear':
                attr_type = 'double'
            return attr_type
        elif isinstance(input, basestring):
            attr_type = cmds.getAttr(input, type=True)
            if attr_type == 'doubleLinear':
                attr_type = 'double'
            return attr_type
    
    def is_type_compatible(self, node, left_type, right_type, cast_ok=False):
        """Is value types compatible with a given node"""
        if left_type == right_type:
            return True
        
        if cast_ok:
            if left_type in NUMERIC_POD_TYPES and right_type in NUMERIC_POD_TYPES:
                return True 
        
        if left_type in node['mixed_types'] and right_type in node['mixed_types'][left_type]:
            return True
        
        return False
    
    def generate_function(self, ast):
        """Generate Maya data for function abstract"""
        if ast.value not in FUNCTIONS:
            self.error('Function "{0}" does not exist'.format(ast.value))
        
        attributes = FUNCTIONS[ast.value]['attributes']
        if len(ast.args) != len(attributes):
            self.error('Number of arguments does not match, expected "{0}" got "{1}" instead'.format(len(attributes), len(ast.args)))
        
        if isinstance(ast.args[0], list):
            primary_value_type = self.get_value_type(ast.args[0][0])
        else:
            primary_value_type = self.get_value_type(ast.args[0])
        
        operator_node_base_type = FUNCTIONS[ast.value]['name']
        operator_node_name = self._namer.create_name(operator_node_base_type)
        
        if len(FUNCTIONS[ast.value]['types']) == 1:
            operator_node_type = operator_node_base_type
        else:
            operator_node_type = operator_node_base_type.format(TYPE_SUFFIX_PER_TYPE[primary_value_type])
        self._nodes.append((operator_node_type, operator_node_name))
        
        for index, arg_ast in enumerate(ast.args):
            if isinstance(arg_ast, list):
                for array_index, array_arg_ast in enumerate(arg_ast):
                    arg = self.generate(array_arg_ast)
                    arg_type = self.get_value_type(arg)
                    
                    self.set_node_values('{0}.{1}'.format(operator_node_name, '{0}[{1}]'.format(attributes[index], array_index)), arg)
            else:
                arg = self.generate(arg_ast)
                arg_type = self.get_value_type(arg)
                
                self.set_node_values('{0}.{1}'.format(operator_node_name, attributes[index]), arg)
        
        index = '[{0}]'.format(ast.index) if ast.index else ''
        attr_type = cmds.attributeQuery('output', type=operator_node_type, attributeType=True)
        return Attribute(attr_type, '{0}.output{1}'.format(operator_node_name, index))
    
    def generate_conditional(self, ast):
        """Generate Maya data for conditional abstract"""
        left = self.generate(ast.left)
        right = self.generate(ast.right)

        left_type = self.get_value_type(left)
        right_type = self.get_value_type(right)
        
        if left_type not in FUNCTIONS[ast.value]['types']:
            self.error('Operator "{0}" does not support left operand of type {1}'.format(ast.value, left_type))
        
        if left_type not in FUNCTIONS['select']['types']:
            self.error('No selector found for values of type {0}'.format(left_type))
        
        if left_type != right_type:
            self.error('Cannot compare values of different type "{0}" and {1}'.format(left_type, right_type))
        
        operator_node_base_type = FUNCTIONS[ast.value]['name']
        operator_node_name = self._namer.create_name(operator_node_base_type)
        operator_node_type = operator_node_base_type.format(TYPE_SUFFIX_PER_TYPE[left_type])
        self._nodes.append((operator_node_type, operator_node_name))

        operations = ['==', '<', '>', '!=', '<=', '>=']
        self._values.append(('{0}.operation'.format(operator_node_name), operations.index(ast.value)))
        
        self.set_node_values('{0}.input1'.format(operator_node_name), left)
        self.set_node_values('{0}.input2'.format(operator_node_name), right)
        
        select_node_base_type = 'math_Select'
        select_node_name = self._namer.create_name(select_node_base_type)
        select_node_type = select_node_base_type.format(TYPE_SUFFIX_PER_TYPE[left_type])
        self._nodes.append((select_node_type, select_node_name))
        self.set_node_values('{0}.condition'.format(select_node_name), '{0}.output'.format(operator_node_name))

        true = self.generate(ast.true)
        self.set_node_values('{0}.input1'.format(select_node_name), true)

        false = self.generate(ast.false)
        self.set_node_values('{0}.input2'.format(select_node_name), false)

        return Attribute(left_type, '{0}.output'.format(select_node_name))

    def generate_binary(self, ast):
        """Generate Maya data for binary abstract"""
        left = self.generate(ast.left)
        right = self.generate(ast.right)

        left_type = self.get_value_type(left)
        right_type = self.get_value_type(right)
        
        if left_type not in FUNCTIONS[ast.value]['types']:
            self.error('Operator "{0}" does not support left operand of type "{1}"'.format(ast.value, left_type))
        
        if not self.is_type_compatible(FUNCTIONS[ast.value], left_type, right_type, isinstance(right, Number)):
            self.error('Operands of different types "{0} {1} {2}" are not support'.format(left_type, ast.value, right_type))
        
        operator_node_base_type = FUNCTIONS[ast.value]['name']
        operator_node_name = self._namer.create_name(operator_node_base_type)
        operator_node_type = operator_node_base_type.format(TYPE_SUFFIX_PER_TYPE[left_type])
        if left_type != right_type:
            operator_node_type += 'By{0}'.format(TYPE_SUFFIX_PER_TYPE[right_type])
        
        self._nodes.append((operator_node_type, operator_node_name))

        self.set_node_values('{0}.input1'.format(operator_node_name), left)
        self.set_node_values('{0}.input2'.format(operator_node_name), right)
        
        return Attribute(left_type, '{0}.output'.format(operator_node_name))
    
    def build(self):
        """Build Maya node network"""
        import maya.cmds as cmds

        for node_type, node_name in self._nodes:
            cmds.createNode(node_type, name=node_name)
        
        for source, target in self._connections:
            cmds.connectAttr(source, target, force=True)
        
        for attr, value in self._values:
            if isinstance(value, list):
                if len(value) == 16:
                    cmds.setAttr(attr, *value, type='matrix')
                else:
                    cmds.setAttr(attr, *value)
            else:
                cmds.setAttr(attr, value)
        
        return self

# bld = ExpresionBuilder('test')
# bld.generate(exp)
# print bld._nodes
# print bld._connections
# print bld._values