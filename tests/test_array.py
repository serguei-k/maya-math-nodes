# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestArray(NodeTestCase):
    def test_sum(self):
        self.create_node('Sum', {'input[0]': 5.0, 'input[1]': -3.0, 'input[2]': 2.0}, 4.0)
    
    def test_sum_int(self):
        self.create_node('SumInt', {'input[0]': 5, 'input[1]': -3, 'input[2]': 2}, 4)
    
    def test_sum_angle(self):
        self.create_node('SumAngle', {'input[0]': 5.0, 'input[1]': -3.0, 'input[2]': 2.0}, 4.0)
    
    def test_sum_vector(self):
        self.create_node('SumVector', {'input[0]': [5.0, 1.0, 0.0],
                                       'input[1]': [-3.0, -2.0, 1.0],
                                       'input[2]': [2.0, 0.0, 3.0]}, [4.0, -1.0, 4.0])
    
    def test_average(self):
        self.create_node('Average', {'input[0]': 5.0, 'input[1]': 5.0, 'input[2]': 5.0}, 5.0)
    
    def test_average_int(self):
        self.create_node('AverageInt', {'input[0]': 5, 'input[1]': 5, 'input[2]': 5}, 5.0)
    
    def test_average_angle(self):
        self.create_node('AverageAngle', {'input[0]': 5.0, 'input[1]': 5.0, 'input[2]': 5.0}, 5.0)
    
    def test_average_vector(self):
        third = 1.0 / 3.0
        self.create_node('AverageVector', {'input[0]': [1.0, 0.0, 0.0],
                                           'input[1]': [0.0, 1.0, 0.0],
                                           'input[2]': [0.0, 0.0, 1.0]}, [third, third, third])

    def test_average_rotation(self):
        self.create_node('AverageRotation', {'input[0]': [0.0, 0.0, 0.0],
                                             'input[1]': [0.0, 90.0, 0.0],
                                             'input[2]': [-90.0, 0.0, 0.0]}, [-32.874, 28.493, -8.567], 3)
    
    def test_average_matrix(self):
        matrix1 = [1.0, 0.0, 0.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0,
                   0.0, 0.0, 0.0, 1.0]
        
        matrix2 = [0.0, 0.0, -1.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   1.0, 0.0, 0.0, 0.0,
                   2.0, 0.0, 0.0, 1.0]
        
        matrix3 = [1.0, 0.0, 0.0, 0.0,
                   0.0, 0.0, -1.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, 2.0, 1.0]
        
        result = [0.869, -0.131, -0.477, 0.0,
                  -0.131, 0.869, -0.477, 0.0,
                  0.477, 0.477, 0.738, 0.0,
                  0.667, 0.0, 0.667, 1.0]
        
        self.create_node('AverageMatrix', {'input[0]': matrix1,
                                           'input[1]': matrix2,
                                           'input[2]': matrix3}, result, 3)
