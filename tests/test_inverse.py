# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestInverse(NodeTestCase):
    def test_inverse_matrix(self):
        matrix = [0.0, 0.0, -1.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  1.0, 0.0, 0.0, 0.0,
                  5.0, -5.0, 5.0, 1.0]
        
        inverse_matrix = [0.0, 0.0, 1.0, 0.0,
                          0.0, 1.0, 0.0, 0.0,
                          -1.0, 0.0, 0.0, 0.0,
                          5.0, 5.0, -5.0, 1.0]
        
        self.create_node('InverseMatrix', {'input': matrix}, inverse_matrix)
    
    def test_inverse_quaternion(self):
        self.create_node('InverseQuaternion', {'input': [1.0, 0.0, 0.0, 0.0]}, [-1.0, 0.0, 0.0, 0.0])
    
    def test_inverse_rotation(self):
        self.create_node('InverseRotation', {'input': [45.0, 0.0, -15.0]}, [-45.0, 0.0, 15.0])
