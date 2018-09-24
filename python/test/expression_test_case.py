# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import unittest
import maya.cmds as cmds

import maya_math_nodes
from maya_math_nodes.expression_lexer import ParsingError
from maya_math_nodes.expression_builder import BuildingError

plugin_name = 'mayaMathNodes'


class ExpressionTestCase(unittest.TestCase):
    """Maya Node Test Case
    
    The test case assumes that the plugin is either already loaded or is discoverable by Maya.
    """
    @classmethod
    def setUpClass(cls):
        if not cmds.pluginInfo(plugin_name, query=True, loaded=True):
            cmds.loadPlugin(plugin_name)
    
    def setUp(self):
        cmds.file(new=True, force=True)
        
        self.name_generator = maya_math_nodes.NodeNameGenerator('test')
        self.dummy = cmds.createNode('transform', name='dummy')
        
        cmds.setAttr('dummy.tx', 2.0)
        cmds.setAttr('dummy.ty', 2.0)
        cmds.setAttr('dummy.tz', 2.0)
        cmds.setAttr('dummy.rx', 2.0)
        cmds.setAttr('dummy.ry', 2.0)
        cmds.setAttr('dummy.rz', 2.0)
    
    def eval_expression(self, expression, output, node_type=None, exception=None, places=4):
        """Expression test utility
        
        Simple utility that evaluates the expression and tests against expected output result.
        Optionally it will also check against expected node type.
        
        Args:
            expression (str): Math expression to evaluate
            output (int | float | bool | list): Computed expression output value
            node_type (str): Result node type to compare
            exception (Exception): Expected exception to be thrown
            places (int): Numeric comparison epsilon
        
        Returns:
            str: Return expression output attribute
        """
        if exception:
            with self.assertRaises(exception):
                maya_math_nodes.eval_expression(expression, name_generator=self.name_generator)
            return
        
        exp_output = maya_math_nodes.eval_expression(expression, name_generator=self.name_generator)
        
        if isinstance(output, bool):
            result = cmds.getAttr(exp_output)
            self.assertEquals(type(result), type(output))
            self.assertEquals(result, output)
        elif isinstance(output, list):
            result = cmds.getAttr(exp_output)
            if isinstance(result[0], tuple):
                result = result[0]
            
            self.assertItemsAlmostEqual(result, output, places)
        else:
            result = cmds.getAttr(exp_output)
            self.assertEquals(type(result), type(output))
            self.assertAlmostEqual(result, output, places)
        
        if node_type:
            self.assertEquals(cmds.objectType(exp_output.split('.')[0]), node_type)
        
        return exp_output
    
    def assertItemsAlmostEqual(self, expected_seq, actual_seq, places=7, msg=None):
        """Assert that container items are near equal within epsilon threshold"""
        for expected, actual in zip(expected_seq, actual_seq):
            self.assertEquals(type(expected), type(actual))
            self.assertAlmostEqual(expected, actual, places, msg)
