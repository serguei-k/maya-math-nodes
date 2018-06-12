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

OPERATORS = ['+', '/', '%', '*', '-']
CONDITION = ['<', '>', '=', '!']
TERNARY = ['?', ':']

PRECEDENCE = {
    "<": 10, ">": 10, "<=": 10, ">=": 10, "==": 10, "!=": 10,
    "+": 20, "-": 20,
    "*": 30, "/": 30, "%": 30,
}

Token = collections.namedtuple('Token', ['type', 'value'])

BraketToken = 0
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
    
    def __repr__(self):
        return '(Number: {0})'.format(self.value)

class String(object):
    def __init__(self, value, index=None):
        self.value = value
        self.index = index
    
    def __repr__(self):
        return '(String: {0}{1})'.format(self.value, '[{0}]'.format(self.index) if self.index else '')

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
        self._next = None
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
            token = self.read_string()
            if self._data.peek() == '(':
                token = Token(FunctionToken, token.value)
            return token
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
    
    def read_number(self):
        """Read number from stream"""
        return Token(NumberToken, self.read_while(functools.partial(self.is_digit, dot_ok=True)))

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
        return Token(BraketToken, self._data.next())
    
    def is_string(self, char):
        """Check for string character"""
        return char.isalpha() or char == '.'
    
    def read_string(self):
        """Read string from stream"""
        return Token(StringToken, self.read_while(self.is_string))


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
            self.error('Expected a valid token, got None instead')
        
        if self.token.type == NumberToken:
            return self.parse_number()
        elif self.token.type == StringToken:
            return self.parse_string()
        elif self.token.type == FunctionToken:
            return self.parse_function()
        elif self.token.type == BraketToken and self.token.value == '(':
            return self.parse_parentheses()
        else:
            self.error('Could not handle token "{0}"'.format(self.token))
    
    def parse_expression(self):
        """Parse expression"""
        left = self.parse_element()

        if self.token and self.token.type == OperatorToken:
            left = self.parse_binary_right(0, left)
        
        if self.token and self.token.type == ConditionToken:
            left = self.parse_conditional(left)

        if self.token:
            self.error('Expected end of expression or another operator, got "{0}" instead'.format(self.token))

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
        self._data.next()  # consume open paren
        while True:
            arg = self.parse_expression()
            if not arg:
                self.error('Expected a valid argument, got "None" istead')
            
            args.append(arg)

            if self.token and self.token.value == ')':
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
            self._data.next() # consume index bracket

            if not self.token or self.token.value != ']':
                self.error('Expected a closing bracket, got "{0}" instead'.format(self.token))
            self._data.next() # consume close bracket

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

# str = '1.0 + node.attr'
# str = '1.0 + node.attr > 55 - 2 ? 2.5 + 3 : node.attr * 1'

str = 'powers(2, node.attr[0])[1] - 12.3'
exp = ExpressionParser(str).parse()
print exp