# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase, cmds


class TestCondition(NodeTestCase):
    def test_compare(self):
        node = self.create_node('Compare', {'input1': 0.01, 'input2': 0.01}, True)
        
        cmds.setAttr('{0}.{1}'.format(node, 'input2'), 1.0)
        cmds.setAttr('{0}.{1}'.format(node, 'operation'), 1)
        self.assertTrue(cmds.getAttr('{0}.output'.format(node)))
        
        cmds.setAttr('{0}.{1}'.format(node, 'operation'), 2)
        self.assertFalse(cmds.getAttr('{0}.output'.format(node)))
        
        cmds.setAttr('{0}.{1}'.format(node, 'operation'), 3)
        self.assertTrue(cmds.getAttr('{0}.output'.format(node)))
        
        cmds.setAttr('{0}.{1}'.format(node, 'operation'), 4)
        self.assertTrue(cmds.getAttr('{0}.output'.format(node)))
        
        cmds.setAttr('{0}.{1}'.format(node, 'operation'), 5)
        self.assertFalse(cmds.getAttr('{0}.output'.format(node)))
    
    def test_compare_angle(self):
        self.create_node('CompareAngle', {'input1': 15.0, 'input2': 15.0}, True)
    
    def test_select(self):
        node = self.create_node('Select', {'input1': 1.0, 'input2': 2.0}, 1.0)
        
        cmds.setAttr('{0}.{1}'.format(node, 'condition'), True)
        self.assertAlmostEqual(cmds.getAttr('{0}.output'.format(node)), 2.0)
    
    def test_select_int(self):
        self.create_node('SelectInt', {'input1': 1.0, 'input2': 2.0, 'condition': True}, 2.0)
    
    def test_select_angle(self):
        self.create_node('SelectAngle', {'input1': 1.0, 'input2': 2.0, 'condition': True}, 2.0)
    
    def test_select_vector(self):
        self.create_node('SelectVector', {'input1': [0.0, 1.0, 0.0], 'condition': False}, [0.0, 1.0, 0.0])
    
    def test_select_rotation(self):
        self.create_node('SelectRotation', {'input1': [0.0, 90.0, 0.0], 'condition': False}, [0.0, 90.0, 0.0])
    
    def test_select_matrix(self):
        matrix = [1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  10.0, 0.0, 0.0, 1.0]
        
        self.create_node('SelectMatrix', {'input2': matrix, 'condition': True}, matrix)
    
    def test_select_quaternion(self):
        self.create_node('SelectQuaternion', {'input2': [1.0, 0.0, 0.0, 1.0], 'condition': True}, [1.0, 0.0, 0.0, 1.0])
