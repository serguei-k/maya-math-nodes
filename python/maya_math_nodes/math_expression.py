# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
"""Maya Math Nodes Expression

This library is experimental and is not recommended for production use!
"""
import copy
import re

import maya.cmds as cmds

from info import *


class MathExpression(object):
    """Math Expression"""
    def __init__(self, expression, name_gen):
        """Initialize Expression
        
        Args:
            expression (str): Math expression that will be parsed
        """
        self._name_gen = name_gen
        self._expression = expression
        
        self._attributes_to_set = []
        self._connections_to_make = []
        self._existing_nodes = []
        self._expression_operands = []
        self._nested_expressions = []
        self._nodes_to_create = []
        
        self.output = None
    
    def parse(self):
        """Parse expression to determine the Maya node network
        
        Returns:
            MathExpression
        """
        # remove spaces to make it easier to parse
        exp_no_space = self._expression.replace(' ', '')
        
        # find nested expressions in parentheses
        left_parentheses_count = exp_no_space.count('(')
        right_parentheses_count = exp_no_space.count(')')
        
        if left_parentheses_count != right_parentheses_count:
            raise RuntimeError('Uneven number of parentheses found!')

        pairs = []
        stack = []
        function_stack = []
        for index, char in enumerate(exp_no_space):
            if char == '(':
                if index != 0 and exp_no_space[index - 1] not in OPERATORS:
                    function_stack.append(index)
                else:
                    stack.append(index)
            
            if char == ')':
                if function_stack:
                    function_stack.pop()
                    continue
                
                if not stack:
                    raise RuntimeError('Close parentheses found without an matching open parentheses.')
                open_index = stack.pop()
                
                if not stack:
                    pairs.append((open_index + 1, index))
        
        if stack:
            raise RuntimeError('Open parentheses found without a matching close parentheses.')
        
        offset = 0
        exp_no_space_copy = exp_no_space
        for index, pair in enumerate(pairs):
            expression = MathExpression(exp_no_space[pair[0]:pair[1]], self._name_gen).parse()
            self._expression_operands.append(expression)
            
            expression_key = 'EXP_{0}'.format(index)
            exp_no_space_copy = exp_no_space_copy[:pair[0] - offset - 1] + expression_key + exp_no_space_copy[pair[1] - offset + 1:]
            offset += pair[1] - pair[0] - len(expression_key) + 2
        exp_no_space = exp_no_space_copy
        
        # get existing nodes (node.attr) from expression
        existing_nodes_match = re.findall(r'(?=(\b\w+\.(?:[\w\[\]]+|$)))', exp_no_space)
        
        for match in existing_nodes_match:
            if not re.match(r'^\d+?\.\d+?$', match):
                if not cmds.ls(match):
                    raise RuntimeError('{0} is not found in the current scene.'.format(match))
                
                self._existing_nodes.append(match)
        
        # get arithmetic operators and operands from expression
        ops_re = re.compile(r'[{0}](?!.+\))|[{0}](?=.+\()'.format('\\'.join(OPERATORS)))
        operands = ops_re.split(exp_no_space)
        operators = ops_re.findall(exp_no_space)
        
        if len(operators) != len(operands) - 1:
            raise ValueError('Number of operators does not match the number of operands.')
        
        # in case no operators are used, resolve as function
        if not operators:
            self.output = self._resolve_operand(exp_no_space)
            
            return self
        
        prev_operand = None
        for index, operator in enumerate(operators):
            # resolve operands
            left_operand = prev_operand
            if not left_operand:
                left_operand = self._resolve_operand(operands[index])
                
                if left_operand.operand_type not in FUNCTIONS[operator]['types']:
                    raise RuntimeError('Invalid operand "{0}" of type "{0}" for operator "{1}".'.format(left_operand.operand,
                                                                                                        left_operand.operand_type,
                                                                                                        operator))
            
            right_operand = self._resolve_operand(operands[index + 1])
            
            # resolve operator
            operator_node_base_type = FUNCTIONS[operator]['name']
            operator_node_name = self._name_gen.create_name(operator_node_base_type)
            operator_node_type = '{0}{1}'.format(operator_node_base_type, TYPE_SUFFIX_PER_TYPE[left_operand.operand_type])
            
            if left_operand.operand_type != right_operand.operand_type:
                if right_operand.operand_type not in FUNCTIONS[operator]['types']:
                    raise RuntimeError('Invalid operand "{0}" of type "{1}" for operator "{2}".'.format(right_operand.operand,
                                                                                                        right_operand.operand_type,
                                                                                                        operator))
                
                if FUNCTIONS[operator]['mixed_types'] and right_operand.operand_type in FUNCTIONS[operator]['mixed_types'][left_operand.operand_type]:
                    operator_node_type = '{0}By{1}'.format(operator_node_type, TYPE_SUFFIX_PER_TYPE[right_operand.operand_type])
                else:
                    raise RuntimeError('Invalid operand types "{0}" and "{1}" for operator "{2}".'.format(left_operand.operand_type,
                                                                                                          right_operand.operand_type,
                                                                                                          operator))
                
                attribute_type = cmds.attributeQuery('input2', type=operator_node_type, attributeType=True)
                if attribute_type in ['long', 'short']:
                    attribute_type = 'int'
                
                if right_operand.operand_type != attribute_type:
                    raise RuntimeError('Invalid operand "{0}" of type "{1}" for for operator "{2}".'.format(right_operand.operand,
                                                                                                            right_operand.operand_type,
                                                                                                            operator))
            
            self._nodes_to_create.append((operator_node_type, operator_node_name))
            prev_operand = Operand.from_type('{0}.output'.format(operator_node_name), operator_node_type)
            
            # resolve operand connections to operator
            if left_operand.isConstant:
                self._attributes_to_set.append(('{0}.input1'.format(operator_node_name), left_operand.value))
            elif left_operand.isFunction:
                self._connections_to_make.append((left_operand.operand, '{0}.input1'.format(operator_node_name)))
            else:
                self._connections_to_make.append((left_operand.operand, '{0}.input1'.format(operator_node_name)))
            
            if right_operand.isConstant:
                self._attributes_to_set.append(('{0}.input2'.format(operator_node_name), right_operand.value))
            elif right_operand.isFunction:
                self._connections_to_make.append((right_operand.operand, '{0}.input2'.format(operator_node_name)))
            else:
                self._connections_to_make.append((right_operand.operand, '{0}.input2'.format(operator_node_name)))
        
        self.output = prev_operand
        
        return self
    
    def _resolve_operand(self, operand_str):
        """Resolve operand in expression"""
        if operand_str.startswith('EXP_'):
            return self._expression_operands[int(operand_str.split('_')[1])].output
        elif operand_str in self._existing_nodes:
            return Operand.from_existing(operand_str)
        elif re.match(r'(\b\w+(?=\())', operand_str):
            func = re.match(r'(\b\w+(?=\())', operand_str).group(0)
            if func not in FUNCTIONS:
                raise RuntimeError('Invalid function "{0}".'.format(func))
            
            operand = Operand.from_function(func, self._name_gen)
            self._nodes_to_create.append((operand.node_type, operand.node_name))
            
            # resolve function arguments
            args_match = re.search(r'(?<=\().+(?=\))', operand_str).group()
            args = re.split(r'[,](?!.+\])|[,](?=.?\[)', args_match)
            
            for arg_index, arg in enumerate(args):
                # check if keyword argument is given
                if '=' in arg:
                    attr_name, arg = arg.split('=')
                else:
                    attr_name = FUNCTIONS[func]['attributes'][arg_index]
                
                if re.search(r'[^\w,.\[\]]', arg):
                    # complex argument types use recursive expression
                    expression = MathExpression(arg, self._name_gen).parse()
                    
                    self._connections_to_make.append((expression.output.operand, '{0}.{1}'.format(operand.node_name, attr_name)))
                    self._nested_expressions.append(expression)
                elif arg in self._existing_nodes:
                    attr_type = cmds.attributeQuery(attr_name, type=operand.node_type, attributeType=True)
                    if cmds.getAttr(arg, type=True) != attr_type:
                        raise RuntimeError('Invalid argument type for "{0}" in function "{1}".'.format(arg, func))
                    
                    self._connections_to_make.append((arg, '{0}.{1}'.format(operand.node_name, attr_name)))
                else:
                    attr = Operand.from_constant(arg)
                    self._attributes_to_set.append(('{0}.{1}'.format(operand.node_name, attr_name), attr.value))
            
            return operand
        else:
            return Operand.from_constant(operand_str)
    
    def build(self):
        """Build node network from expression"""
        for expression in self._expression_operands:
            expression.build()
        
        for expression in self._nested_expressions:
            expression.build()
        
        for node_type, node_name in self._nodes_to_create:
            cmds.createNode(node_type, name=node_name)
        
        for source, target in self._connections_to_make:
            cmds.connectAttr(source, target, force=True)
        
        for attr, value in self._attributes_to_set:
            if isinstance(value, list):
                if len(value) == 16:
                    cmds.setAttr(attr, *value, type='matrix')
                else:
                    cmds.setAttr(attr, *value)
            else:
                cmds.setAttr(attr, value)
        
        return self


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


class Operand(object):
    """Math Expression Operand"""
    def __init__(self):
        self.isConstant = False
        self.isFunction = False
        self.node_name = None
        self.node_type = None
        self.operand = None
        self.operand_type = None
        self.value = None
    
    @staticmethod
    def from_existing(operand):
        """Create operand from existing node"""
        new_operand = Operand()
        
        new_operand.node_type = cmds.objectType(operand.split('.')[0])
        new_operand.operand = operand
        new_operand.operand_type = cmds.getAttr(operand, type=True)
        
        return new_operand
    
    @staticmethod
    def from_function(operand, name_gen):
        """Create operand from function"""
        new_operand = Operand()
        
        new_operand.isFunction = True
        new_operand.node_type = FUNCTIONS[operand]['name']
        new_operand.node_name = name_gen.create_name(new_operand.node_type)
        new_operand.operand = '{0}.output'.format(new_operand.node_name)
        new_operand.operand_type = cmds.attributeQuery('output', type=new_operand.node_type, attributeType=True)
        
        return new_operand
    
    @staticmethod
    def from_type(operand, node_type):
        """Create operand from node type"""
        new_operand = Operand()
        
        new_operand.node_name = operand.split('.')[0]
        new_operand.node_type = node_type
        new_operand.operand = operand
        new_operand.operand_type = cmds.attributeQuery(operand.split('.')[1], type=node_type, attributeType=True)
        
        return new_operand
    
    @staticmethod
    def from_constant(operand):
        """Create operand from constant value"""
        new_operand = Operand()
        
        new_operand.isConstant = True
        new_operand.operand = operand
        
        if re.match(r'^\d+?\.\d+?$', operand):
            new_operand.operand_type = 'double'
            new_operand.value = float(operand)
        elif re.match(r'^\d+?$', operand):
            new_operand.operand_type = 'int'
            new_operand.value = int(operand)
        elif re.search(r'(?<=\[).+(?=\])', operand):
            values = re.search(r'(?<=\[).+(?=\])', operand).group(0).replace(' ', '').split(',')
            values_count = len(values)
            
            if values_count not in [3, 4, 16]:
                raise RuntimeError('Invalid constant value operand {0}.'.format(operand))
            
            for value in values:
                if re.match(r'\D+?', value):
                    raise RuntimeError('Invalid constant value operand {0}.'.format(operand))
            
            if values_count == 16:
                new_operand.operand_type = 'matrix'
            else:
                new_operand.operand_type = 'double{0}'.format(values_count)
            
            new_operand.value = [float(value) for value in values]
        else:
            raise RuntimeError('Invalid constant value operand {0}.'.format(operand))
        
        return new_operand
