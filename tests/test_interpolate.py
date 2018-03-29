# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase, cmds


class TestInterpolate(NodeTestCase):
    def test_lerp(self):
        self.create_node('Lerp', {'input1': 0.0, 'input2': 10.0, 'alpha': 0.5}, 5.0)
    
    def test_lerp_angle(self):
        self.create_node('LerpAngle', {'input1': 0.0, 'input2': 10.0, 'alpha': 0.5}, 5.0)
    
    def test_lerp_vector(self):
        self.create_node('LerpVector',
                         {'input1': [0.0, 0.0, 0.0], 'input2': [10.0, 10.0, 10.0], 'alpha': 0.5},
                         [5.0, 5.0, 5.0])
    
    def test_slerp_quaternion(self):
        node = self.create_node('SlerpQuaternion',
                                {'input1': [0.0, 0.0, 0.0, 1.0], 'input2': [1.0, 0.0, 0.0, 0.0], 'alpha': 0.5},
                                [0.7071, 0.0, 0.0, 0.7071])
        
        cmds.setAttr('{0}.{1}'.format(node, 'interpolationType'), 1)
        self.assertItemsAlmostEqual(cmds.getAttr('{0}.output'.format(node))[0], [-0.7071, 0.0, 0.0, 0.7071], 4)
    
    def test_lerp_matrix(self):
        matrix1 = [1.0, 0.0, 0.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0,
                   10.0, -10.0, 10.0, 1.0]
        
        matrix2 = [-1.0, 0.0, 0.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, -1.0, 0.0,
                   5.0, -5.0, 5.0, 1.0]
        
        out_matrix = [0.0, 0.0, -1.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      1.0, 0.0, 0.0, 0.0,
                      7.5, -7.5, 7.5, 1.0]
        
        self.create_node('LerpMatrix', {'input1': matrix1, 'input2': matrix2, 'alpha': 0.5}, out_matrix)
