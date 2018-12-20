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
    
    def test_compare_int(self):
        self.create_node('CompareInt', {'input1': 15, 'input2': 15}, True)
    
    def test_select(self):
        node = self.create_node('Select', {'input1': 1.0, 'input2': 2.0}, 1.0)
        
        cmds.setAttr('{0}.{1}'.format(node, 'condition'), True)
        self.assertAlmostEqual(cmds.getAttr('{0}.output'.format(node)), 2.0)
    
    def test_select_array(self):
        node = self.create_node('SelectArray', {'input1[0]': 1.0, 'input1[1]': 2.0,
                                                'input2[0]': -1.0, 'input2[1]': -2.0}, [1.0, 2.0])
        
        cmds.setAttr('{0}.{1}'.format(node, 'condition'), True)
        self.assertAlmostEqual(cmds.getAttr('{0}.output[0]'.format(node)), -1.0)
    
    def test_select_int(self):
        self.create_node('SelectInt', {'input1': 1, 'input2': 2, 'condition': True}, 2)
    
    def test_select_int_array(self):
        self.create_node('SelectIntArray', {'input1[0]': 1, 'input1[1]': 2,
                                            'input2[0]': -1, 'input2[1]': -2,
                                            'condition': True}, [-1, -2])
    
    def test_select_angle(self):
        self.create_node('SelectAngle', {'input1': 1.0, 'input2': 2.0, 'condition': True}, 2.0)
    
    def test_select_angle_array(self):
        # seems like output angle array does NOT respect the angle unit preference
        self.create_node('SelectAngleArray', {'input1[0]': 1.0, 'input1[1]': 2.0,
                                              'input2[0]': 45.0, 'input2[1]': 90.0,
                                              'condition': True}, [0.785398, 1.570796])
    
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

    def test_and_bool(self):
        self.create_node('AndBool', {'input1': True, 'input2': True}, True)

    def test_and_int(self):
        self.create_node('AndInt', {'input1': 1, 'input2': 0}, False)

    def test_or_bool(self):
        self.create_node('OrBool', {'input1': True, 'input2': True}, True)

    def test_or_int(self):
        self.create_node('OrInt', {'input1': 0, 'input2': 0}, False)

    def test_xor_bool(self):
        self.create_node('XorBool', {'input1': True, 'input2': True}, False)

    def test_xor_int(self):
        self.create_node('XorInt', {'input1': 1, 'input2': 0}, True)
