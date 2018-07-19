# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import collections

import maya.cmds as cmds

from expression_info import *
from expression_parser import *

Attribute = collections.namedtuple('Attribute', ['type', 'value'])


class BuildingError(Exception):
    """Building exception raised from the builder"""
    pass


class NodeNameGenerator(object):
    """Node Name Generator
    
    This class is used to procedurally generate unique names for nodes created by the expression builder.
    
    You can replace its use by any class that satisfies the get_name(node_type) interface to accomodate your
    pipeline naming requirements.
    """
    def __init__(self, base_name):
        """Initialize name generator
        
        Args:
            base_name (str): Base name used by the generator
        """
        self._base_name = base_name

        self._counter_per_type = dict()
        for node_type in [value['name'] for value in FUNCTIONS.values()]:
            self._counter_per_type[node_type] = 0
    
    def get_name(self, node_type):
        """Get unique node name for a given type
        
        Args:
            node_type (str): Maya node type
        
        Returns:
            str: Returns new unique name
        """
        index = self._counter_per_type[node_type]
        self._counter_per_type[node_type] = index + 1
        suffix = node_type.split('_')[1][:3].upper()
        
        return '{0}{1}_{2}'.format(self._base_name, index, suffix)


class ExpresionBuilder(object):
    """Expression Builder
    
    This class takes in the expression AST and generates the required Maya commands to
    create the node network for the given expression.
    """
    def __init__(self, name_generator):
        """Initialize expression builder

        Args:
            name_generator (NodeNameGenerator): Node name generator
        """
        self._nodes = []
        self._connections = []
        self._values = []
        self._namer = name_generator
    
    def generate(self, ast):
        """Generate Maya data from AST
        
        Args:
            ast (object): Expression AST node
        
        Returns:
            Attribute: Returns resulting output attribute for node graph that would be generated from AST
        """
        if isinstance(ast, Binary):
            return self.generate_binary(ast)
        elif isinstance(ast, Conditional):
            return self.generate_conditional(ast)
        elif isinstance(ast, Function):
            return self.generate_function(ast)
        else:
            return ast
    
    def set_node_values(self, attr, value):
        """Set node values
        
        Args:
            attr (str): Attribute to set the value for, in form node.attribute
            value (object): Value data to set the attribute to
        """
        if isinstance(value, Number):
            self._values.append((attr, value.value))
        elif isinstance(value, String) or isinstance(value, Attribute):
            self._connections.append((value.value, attr))
        elif isinstance(value, basestring):
            self._connections.append((value, attr))

    def get_value_type(self, value):
        """Get value type
        
        Args:
            value (object | str): Value data to query the type for
        
        Returns:
            str: Returns value type
        """
        if isinstance(value, Number) or isinstance(value, Attribute):
            return value.type
        elif isinstance(value, String):
            attr_type = cmds.getAttr(value.value, type=True)
            if attr_type == 'doubleLinear':
                attr_type = 'double'
            return attr_type
        elif isinstance(value, basestring):
            attr_type = cmds.getAttr(value, type=True)
            if attr_type == 'doubleLinear':
                attr_type = 'double'
            return attr_type
    
    def resolve_operand_types(self, operator, left, right):
        """Resolve operand types"""
        node = FUNCTIONS[operator]

        left_type = self.get_value_type(left)
        right_type = self.get_value_type(right)
        
        # cast left operand to double if no type specific node exists
        left_cast_ok = isinstance(left, Number) and left_type in NUMERIC_POD_TYPES
        if left_type not in node['types']:
            if left_cast_ok:
                left_type = 'double'
            else:
                raise BuildingError('Operands of different types "{0} {1} {2}" are not supported'.format(left_type, operator, right_type))

        if left_type == right_type:
            return left_type, right_type

        # check if mixed type node exist for this pair
        if left_type in node['mixed_types'] and right_type in node['mixed_types'][left_type]:
            return left_type, right_type
        
        # otherwise try to cast right to left
        right_cast_ok = isinstance(right, Number) and right_type in NUMERIC_POD_TYPES
        if right_cast_ok and left_type in NUMERIC_POD_TYPES:
            return left_type, left_type
        elif right_cast_ok and left_type in node['mixed_types']:
            # check if there is a mixed type node we can cast to
            for pod_type in NUMERIC_POD_TYPES:
                if pod_type in node['mixed_types'][left_type]:
                    return left_type, pod_type
        else:
            raise BuildingError('Operands of different types "{0} {1} {2}" are not supported'.format(left_type, operator, right_type))

    def generate_function(self, ast):
        """Generate Maya data for function AST node
        
        Args:
            ast (Function): AST funciton node to process
        
        Returns:
            Attribute: Returns resulting output attribute for node graph generated for the function
        """
        if ast.value not in FUNCTIONS:
            raise BuildingError('Function "{0}" does not exist'.format(ast.value))
        
        attributes = FUNCTIONS[ast.value]['attributes']
        if len(ast.args) != len(attributes):
            raise BuildingError('Number of arguments does not match, expected "{0}" got "{1}" instead'.format(len(attributes), len(ast.args)))
        
        # use first function argument to deduce node type
        # make sure to check if the first argument value is an array
        if isinstance(ast.args[0], list):
            primary_value_type = self.get_value_type(ast.args[0][0])
        else:
            primary_value_type = self.get_value_type(ast.args[0])
        
        operator_node_base_type = FUNCTIONS[ast.value]['name']
        operator_node_name = self._namer.get_name(operator_node_base_type)
        
        # if there are type specific node variants we need to account for that in the node type name
        if len(FUNCTIONS[ast.value]['types']) == 1:
            operator_node_type = operator_node_base_type
        else:
            operator_node_type = operator_node_base_type.format(TYPE_SUFFIX_PER_TYPE[primary_value_type])
        self._nodes.append((operator_node_type, operator_node_name))
        
        for index, arg_ast in enumerate(ast.args):
            # handle array values
            if isinstance(arg_ast, list):
                for array_index, array_arg_ast in enumerate(arg_ast):
                    # recursively generate function argument value
                    arg = self.generate(array_arg_ast)
                    arg_type = self.get_value_type(arg)
                    
                    self.set_node_values('{0}.{1}'.format(operator_node_name, '{0}[{1}]'.format(attributes[index], array_index)), arg)
            else:
                # recursively generate function argument value
                arg = self.generate(arg_ast)
                arg_type = self.get_value_type(arg)
                
                self.set_node_values('{0}.{1}'.format(operator_node_name, attributes[index]), arg)
        
        index = '[{0}]'.format(ast.index) if ast.index else ''
        attr_type = cmds.attributeQuery('output', type=operator_node_type, attributeType=True)
        return Attribute(attr_type, '{0}.output{1}'.format(operator_node_name, index))
    
    def generate_conditional(self, ast):
        """Generate Maya data for conditional AST node
        
        Args:
            ast (Condition): AST condition node to process
        
        Returns:
            Attribute: Returns resulting output attribute for node graph generated for this condition
        """
        # recursively generate the left and right conditional expression operands
        left = self.generate(ast.left)
        right = self.generate(ast.right)
        
        left_type, right_type = self.resolve_operand_types(ast.value, left, right)

        operator_node_base_type = FUNCTIONS[ast.value]['name']
        operator_node_name = self._namer.get_name(operator_node_base_type)
        operator_node_type = operator_node_base_type.format(TYPE_SUFFIX_PER_TYPE[left_type])
        self._nodes.append((operator_node_type, operator_node_name))

        operations = ['==', '<', '>', '!=', '<=', '>=']
        self._values.append(('{0}.operation'.format(operator_node_name), operations.index(ast.value)))
        
        self.set_node_values('{0}.input1'.format(operator_node_name), left)
        self.set_node_values('{0}.input2'.format(operator_node_name), right)
        
        # recursively geneate true and false outputs
        true = self.generate(ast.true)
        false = self.generate(ast.false)

        true_type, false_type = self.resolve_operand_types('select', true, false)

        select_node_base_type = FUNCTIONS['select']['name']
        select_node_name = self._namer.get_name(select_node_base_type)
        select_node_type = select_node_base_type.format(TYPE_SUFFIX_PER_TYPE[true_type])
        self._nodes.append((select_node_type, select_node_name))
        self.set_node_values('{0}.condition'.format(select_node_name), '{0}.output'.format(operator_node_name))
        self.set_node_values('{0}.input1'.format(select_node_name), false)
        self.set_node_values('{0}.input2'.format(select_node_name), true)

        return Attribute(left_type, '{0}.output'.format(select_node_name))

    def generate_binary(self, ast):
        """Generate Maya data for binary operation AST node"""
        # recursively generate the left and right conditional expresion operands
        left = self.generate(ast.left)
        right = self.generate(ast.right)

        left_type, right_type = self.resolve_operand_types(ast.value, left, right)
        
        operator_node_base_type = FUNCTIONS[ast.value]['name']
        operator_node_name = self._namer.get_name(operator_node_base_type)
        operator_node_type = operator_node_base_type.format(TYPE_SUFFIX_PER_TYPE[left_type])
        if left_type != right_type:
            operator_node_type += 'By{0}'.format(TYPE_SUFFIX_PER_TYPE[right_type])
        
        # TODO: some workarounds for inconsistencies
        if operator_node_type == 'math_MultiplyVectorByInt':
            operator_node_type = 'math_MultiplyVector'
        
        try:
            cmds.nodeType(operator_node_type, isTypeName=True)
        except RuntimeError:
            raise BuildingError('Binary operation generated unrecognized node type "{0}" for "{1} {2} {3}"'.format(
                operator_node_type, left_type, ast.value, right_type))

        self._nodes.append((operator_node_type, operator_node_name))

        self.set_node_values('{0}.input1'.format(operator_node_name), left)
        self.set_node_values('{0}.input2'.format(operator_node_name), right)
        
        return Attribute(left_type, '{0}.output'.format(operator_node_name))
    
    def build(self):
        """Build Maya node network that represents the expression AST"""
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
