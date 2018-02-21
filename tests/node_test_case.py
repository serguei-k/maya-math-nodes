# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import unittest
import maya.cmds as cmds

plugin_name = 'mayaMathNodes'
node_name_prefix = 'math_'

class NodeTestCase(unittest.TestCase):
    """Maya Node Test Case
    
    The test case assumes that the plugin is either already loaded or is discoverable by Maya.
    """
    @classmethod
    def setUpClass(cls):
        if not cmds.pluginInfo(plugin_name, query=True, loaded=True):
            cmds.loadPlugin(plugin_name)
    
    def setUp(self):
        cmds.file(new=True, force=True)
    
    def create_node(self, node_type, inputs, output):
        """Node test utility
        
        Simple utility that creates a new node, sets input attribute values and tests against expected output result.
        
        Inputs are provided as dictionary of attribute name and value.
        """
        full_node_type = '{0}{1}'.format(node_name_prefix, node_type)
        
        node = cmds.createNode(full_node_type, skipSelect=True)
        self.assertTrue(cmds.objectType(node, isType=full_node_type))
        
        for attr in inputs:
            if isinstance(inputs[attr], list):
                cmds.setAttr('{0}.{1}'.format(node, attr), *inputs[attr])
            else:
                cmds.setAttr('{0}.{1}'.format(node, attr), inputs[attr])
        
        if isinstance(output, list):
            self.assertItemsAlmostEqual(cmds.getAttr('{0}.output'.format(node))[0], output, 4)
        else:
            self.assertAlmostEqual(cmds.getAttr('{0}.output'.format(node)), output, 4)
        
        return node
    
    def assertItemsAlmostEqual(self, expected_seq, actual_seq, places=7, msg=None):
        for expected, actual in zip(expected_seq, actual_seq):
            self.assertAlmostEqual(expected, actual, places, msg)
