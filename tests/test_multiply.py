# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestMultiply(NodeTestCase):
    def test_multiply(self):
        self.create_node('Multiply', {'input1': 2.0, 'input2': 2.0}, 4.0)
    
    def test_multiply_by_int(self):
        self.create_node('MultiplyByInt', {'input1': 2.0, 'input2': 2}, 4.0)
    
    def test_multiply_angle(self):
        self.create_node('MultiplyAngle', {'input1': 2.0, 'input2': 2.0}, 4.0)
    
    def test_multiply_angle_by_int(self):
        self.create_node('MultiplyAngleByInt', {'input1': 2.0, 'input2': 2}, 4.0)
    
    def test_multiply_int(self):
        self.create_node('MultiplyInt', {'input1': 2, 'input2': 2}, 4)
    
    def test_multiply_matrix(self):
        matrix1 = [0.0, 0.0, -1.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   1.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 0.0, 1.0]
        
        matrix2 = [0.0, 0.0, -1.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   1.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, 0.0, 1.0]
        
        result = [-1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, -1.0, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('MultiplyMatrix', {'input1': matrix1, 'input2': matrix2}, result)
    
    def test_multiply_vector(self):
        self.create_node('MultiplyVector', {'input1': [1.0, 1.0, 1.0], 'input2': 0.5}, [0.5, 0.5, 0.5])

    def test_multiply_vector_by_matrix(self):
        matrix = [0.0, 0.0, -1.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  1.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('MultiplyVectorByMatrix', {'input1': [1.0, 0.0, 0.0], 'input2': matrix}, [0.0, 0.0, -1.0])
    
    def test_multiply_quaternion(self):
        self.create_node('MultiplyQuaternion',
                         {'input1': [0.7071, 0.0, 0.0, 0.7071], 'input2': [0.7071, 0.0, 0.0, 0.7071]},
                         [1.0, 0.0, 0.0, 0.0])
    
    def test_multiply_rotation(self):
        self.create_node('MultiplyRotation', {'input1': [45.0, 0.0, -15.0], 'input2': 2.0}, [90.0, 0.0, -30.0])
