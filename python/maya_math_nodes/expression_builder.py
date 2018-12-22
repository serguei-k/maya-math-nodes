# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import collections

import maya.api.OpenMaya as om
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

    You can replace its use by any class that satisfies the get_name(node_type) interface to accommodate your
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
            if node_type.endswith('}'):
                node_type = node_type[:-3]
            suffix = node_type.split('_')[1][:3].upper()

            self._counter_per_type['{0}_{1}'.format(self._base_name, suffix)] = 0

    def get_name(self, node_type):
        """Get unique node name for a given type

        Args:
            node_type (str): Maya node type

        Returns:
            str: Returns new unique name
        """
        if node_type.endswith('}'):
            node_type = node_type[:-3]
        suffix = node_type.split('_')[1][:3].upper()

        counter_key = '{0}_{1}'.format(self._base_name, suffix)
        index = self._counter_per_type[counter_key]
        self._counter_per_type[counter_key] = index + 1

        return '{0}{1}_{2}'.format(self._base_name, index, suffix)


class ExpressionBuilder(object):
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
        self._first = True

    def generate(self, ast):
        """Generate Maya data from AST

        Args:
            ast (object): Expression AST node

        Returns:
            Attribute: Returns resulting output attribute for node graph that would be generated from AST
        """
        if isinstance(ast, Binary):
            self._first = False
            return self.generate_binary(ast)
        elif isinstance(ast, Conditional):
            self._first = False
            return self.generate_conditional(ast)
        elif isinstance(ast, Function):
            self._first = False
            return self.generate_function(ast)
        elif self._first:
            raise BuildingError("Expected expression, got '{0}' instead".format(ast.value))

        self._first = False
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

    @staticmethod
    def condition_type(maya_type):
        """Condition Maya attribute type to one supported by the builder

        Args:
            maya_type (str): Maya attribute type

        Returns:
            str: Return conditioned attribute type
        """
        if maya_type == 'doubleLinear':
            maya_type = 'double'
        elif maya_type == 'long' or maya_type == 'short' or maya_type == 'enum':
            maya_type = 'int'
        elif maya_type == 'TdataCompound':
            maya_type = 'compound'

        return maya_type

    @staticmethod
    def get_attribute_type(attr_name, node_type):
        """Get attribute type

        Args:
            attr_name (str): Attribute name to query the type for
            node_type (str): Node type the attribute belongs to

        Returns:
            str: Returns attribute type
        """
        if '[' in attr_name:
            attr_name = attr_name.split('[')[0]

        try:
            attr_type = cmds.attributeQuery(attr_name, type=node_type, attributeType=True)
            attr_type = ExpressionBuilder.condition_type(attr_type)
        except RuntimeError:
            raise BuildingError("Invalid Maya attribute specified '{0}'".format(attr_name))

        if attr_type == 'double3':
            children = cmds.attributeQuery(attr_name, type=node_type, listChildren=True)
            if cmds.attributeQuery(children[0], type=node_type, attributeType=True) == 'doubleAngle':
                attr_type = 'double3Angle'
        elif attr_type == 'compound':
            children = cmds.attributeQuery(attr_name, type=node_type, listChildren=True)
            if len(children) == 4 and cmds.attributeQuery(children[0], type=node_type, attributeType=True) == 'double':
                attr_type = 'double4'
        elif attr_type == 'typed':
            node_class = om.MNodeClass(node_type)
            attr_obj = node_class.attribute(attr_name)
            if om.MFnTypedAttribute(attr_obj).attrType() == om.MFnData.kMatrix:
                attr_type = 'matrix'
            elif om.MFnTypedAttribute(attr_obj).attrType() == om.MFnData.kMesh:
                attr_type = 'mesh'
            elif om.MFnTypedAttribute(attr_obj).attrType() == om.MFnData.kNurbsCurve:
                attr_type = 'nurbsCurve'
            elif om.MFnTypedAttribute(attr_obj).attrType() == om.MFnData.kNurbsSurface:
                attr_type = 'nurbsSurface'
        
        return attr_type
    
    @staticmethod
    def get_value_type(value):
        """Get value type

        Args:
            value (Attribute | Number | String | str): Value data to query the type for

        Returns:
            str: Returns value type
        """
        if isinstance(value, Number) or isinstance(value, Attribute):
            return value.type

        if isinstance(value, String):
            attr_name = value.value
        elif isinstance(value, basestring):
            attr_name = value

        try:
            node_name, attr_name = attr_name.split('.')
            node_type = cmds.nodeType(node_name)
        except ValueError:
            raise BuildingError("Invalid Maya attribute specified '{0}'".format(attr_name))
        except RuntimeError:
            raise BuildingError("Invalid Maya node specified '{0}'".format(node_name))

        attr_type = ExpressionBuilder.get_attribute_type(attr_name, node_type)
        return attr_type

    @staticmethod
    def resolve_operand_types(operator, left, right):
        """Resolve operand types

        Args:
            operator (str): Operation type
            left (Attribute | Number | String | str): Left operand
            right (Attribute | Number | String | str): Right operand

        Returns:
            tuple[str, str]: Returns left and right operand types
        """
        node = FUNCTIONS[operator]

        left_type = ExpressionBuilder.get_value_type(left)
        right_type = ExpressionBuilder.get_value_type(right)

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

    @staticmethod
    def validate_node_type(node_type, message):
        """Validate generated node type against Maya nodes

        Args:
            node_type (str): Node type to check
            message (str): Error message in case validation fails

        Returns:
            str: Return conditioned node type
        """
        # TODO: some workarounds for inconsistently named nodes
        if node_type == 'math_MultiplyVectorBy':
            node_type = 'math_MultiplyVector'
        elif node_type == 'math_DistanceVector':
            node_type = 'math_DistancePoints'
        elif node_type == 'math_DistanceMatrix':
            node_type = 'math_DistanceTransforms'

        try:
            cmds.nodeType(node_type, isTypeName=True)
        except RuntimeError:
            raise BuildingError('Generated unrecognized node type "{0}" for {1}'.format(node_type, message))

        return node_type

    @staticmethod
    def validate_function_argument_type(attr_name, node_type, arg_type, cast_ok):
        """Check if argument type is compatible with attribute type

        Args:
            attr_name (str): Attribute name
            node_type (str): Maya node type
            arg_type (str): Proposed expression argument type
            cast_ok (bool): Ok to cast literal numeric values

        Returns:
            bool: Returns True if function argument type is compatible with attribute type
        """
        attr_type = ExpressionBuilder.get_attribute_type(attr_name, node_type)
        if attr_type == arg_type:
            return True

        if attr_type in NUMERIC_POD_TYPES and (cast_ok and arg_type in NUMERIC_POD_TYPES):
            return True

        raise BuildingError('Invalid argument of type "{0}" used for node attribute "{1}.{2}"'.format(arg_type, node_type, attr_name))

    def generate_function(self, ast):
        """Generate Maya data for function AST node

        Args:
            ast (Function): AST function node to process

        Returns:
            Attribute: Returns resulting output attribute for node graph generated for the function
        """
        if ast.value not in FUNCTIONS:
            raise BuildingError('Function "{0}" does not exist'.format(ast.value))

        attributes = FUNCTIONS[ast.value]['attributes']
        if len(ast.args) < 1:
            raise BuildingError('Function "{}" require at least one argument.'.format(ast.value))
        elif len(ast.args) > len(attributes):
            raise BuildingError('Too many arguments specified for function "{}", expected "{}" got "{}" instead'.format(
                                ast.value, len(attributes), len(ast.args)))

        operator_node_base_type = FUNCTIONS[ast.value]['name']
        operator_node_name = self._namer.get_name(operator_node_base_type)

        for index, arg_ast in enumerate(ast.args):
            # handle array values
            if isinstance(arg_ast, list):
                for array_index, array_arg_ast in enumerate(arg_ast):
                    # recursively generate function argument value
                    arg = self.generate(array_arg_ast)
                    arg_type = self.get_value_type(arg)
                    arg_type = self.condition_type(arg_type)

                    # we use first function argument to deduce node type
                    if index == 0 and array_index == 0:
                        # if there are type specific node variants we need to account for that in the node type name
                        if len(FUNCTIONS[ast.value]['types']) == 1:
                            operator_node_type = operator_node_base_type
                        else:
                            operator_node_type = operator_node_base_type.format(TYPE_SUFFIX_PER_TYPE[arg_type])
                        operator_node_type = self.validate_node_type(operator_node_type, 'function "{0}"'.format(ast.value))
                        self._nodes.append((operator_node_type, operator_node_name))

                    self.validate_function_argument_type(attributes[index], operator_node_type, arg_type, isinstance(arg, Number))
                    self.set_node_values('{0}.{1}'.format(operator_node_name, '{0}[{1}]'.format(attributes[index], array_index)), arg)
            else:
                # recursively generate function argument value
                arg = self.generate(arg_ast)
                arg_type = self.get_value_type(arg)
                arg_type = self.condition_type(arg_type)

                # we use first function argument to deduce node type
                if index == 0:
                    if arg_type not in FUNCTIONS[ast.value]['types']:
                        # check if it is possible to cast a pod argument to a supported pod type
                        pod_types = [typ for typ in FUNCTIONS[ast.value]['types'] if typ in NUMERIC_POD_TYPES]
                        if pod_types and isinstance(arg, Number) and arg_type in NUMERIC_POD_TYPES:
                            arg_type = pod_types[0]
                        else:
                            raise BuildingError("Function '{0}' does not support type '{1}'".format(ast.value, arg_type))

                    # if there are type specific node variants we need to account for that in the node type name
                    if len(FUNCTIONS[ast.value]['types']) == 1:
                        operator_node_type = operator_node_base_type
                    else:
                        operator_node_type = operator_node_base_type.format(TYPE_SUFFIX_PER_TYPE[arg_type])
                    operator_node_type = self.validate_node_type(operator_node_type, 'function "{0}"'.format(ast.value))
                    self._nodes.append((operator_node_type, operator_node_name))

                self.validate_function_argument_type(attributes[index], operator_node_type, arg_type, isinstance(arg, Number))
                self.set_node_values('{0}.{1}'.format(operator_node_name, attributes[index]), arg)

        index = '[{0}]'.format(ast.index) if ast.index else ''
        attr_type = self.get_attribute_type('output', operator_node_type)
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
        operator_node_type = self.validate_node_type(operator_node_type, '"{0} {1} {2}"'.format(left_type, ast.value, right_type))
        self._nodes.append((operator_node_type, operator_node_name))

        operations = ['==', '<', '>', '!=', '<=', '>=']
        self._values.append(('{0}.operation'.format(operator_node_name), operations.index(ast.value)))

        self.set_node_values('{0}.input1'.format(operator_node_name), left)
        self.set_node_values('{0}.input2'.format(operator_node_name), right)

        # recursively generate true and false outputs
        true = self.generate(ast.true)
        false = self.generate(ast.false)

        true_type, false_type = self.resolve_operand_types('select', true, false)

        select_node_base_type = FUNCTIONS['select']['name']
        select_node_name = self._namer.get_name(select_node_base_type)
        select_node_type = select_node_base_type.format(TYPE_SUFFIX_PER_TYPE[true_type])
        operator_node_type = self.validate_node_type(select_node_type, '"{0} {1} {2}"'.format(true_type, ast.value, false_type))
        self._nodes.append((operator_node_type, select_node_name))
        self.set_node_values('{0}.condition'.format(select_node_name), '{0}.output'.format(operator_node_name))
        self.set_node_values('{0}.input1'.format(select_node_name), false)
        self.set_node_values('{0}.input2'.format(select_node_name), true)

        return Attribute(true_type, '{0}.output'.format(select_node_name))

    def generate_binary(self, ast):
        """Generate Maya data for binary operation AST node

        Args:
            ast (Binary): AST binary node to process

        Returns:
            Attribute: Returns resulting output attribute for node graph generated for this operation
        """
        # recursively generate the left and right operands
        left = self.generate(ast.left)
        right = self.generate(ast.right)

        left_type, right_type = self.resolve_operand_types(ast.value, left, right)

        operator_node_base_type = FUNCTIONS[ast.value]['name']
        operator_node_name = self._namer.get_name(operator_node_base_type)
        operator_node_type = operator_node_base_type.format(TYPE_SUFFIX_PER_TYPE[left_type])
        if left_type != right_type:
            operator_node_type += 'By{0}'.format(TYPE_SUFFIX_PER_TYPE[right_type])

        operator_node_type = self.validate_node_type(operator_node_type, '"{0} {1} {2}"'.format(left_type, ast.value, right_type))
        self._nodes.append((operator_node_type, operator_node_name))

        self.set_node_values('{0}.input1'.format(operator_node_name), left)
        self.set_node_values('{0}.input2'.format(operator_node_name), right)

        attr_type = self.get_attribute_type('output', operator_node_type)
        return Attribute(attr_type, '{0}.output'.format(operator_node_name))

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
