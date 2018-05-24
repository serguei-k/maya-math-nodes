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
    
    def test_max_element(self):
        self.create_node('MaxElement', {'input[0]': 5.0, 'input[1]': -3.0, 'input[2]': 2.0}, 5.0)
    
    def test_max_int_element(self):
        self.create_node('MaxIntElement', {'input[0]': 5, 'input[1]': -3, 'input[2]': 2}, 5)
    
    def test_max_angle_element(self):
        self.create_node('MaxAngleElement', {'input[0]': 5.0, 'input[1]': -3.0, 'input[2]': 2.0}, 5.0)
    
    def test_min_element(self):
        self.create_node('MinElement', {'input[0]': 5.0, 'input[1]': -3.0, 'input[2]': 2.0}, -3.0)
    
    def test_min_int_element(self):
        self.create_node('MinIntElement', {'input[0]': 5, 'input[1]': -3, 'input[2]': 2}, -3)
    
    def test_min_angle_element(self):
        self.create_node('MinAngleElement', {'input[0]': 5.0, 'input[1]': -3.0, 'input[2]': 2.0}, -3.0)
    
    def test_average(self):
        self.create_node('Average', {'input[0]': 5.0, 'input[1]': 5.0, 'input[2]': 5.0}, 5.0)
    
    def test_weighted_average(self):
        self.create_node('WeightedAverage', {'input[0].value': 5.0, 'input[0].weight': 1.0,
                                             'input[1].value': 5.0, 'input[1].weight': 0.5,
                                             'input[2].value': 5.0, 'input[2].weight': 0.25}, 5.0)
    
    def test_average_int(self):
        self.create_node('AverageInt', {'input[0]': 3, 'input[1]': 2, 'input[2]': 2}, 7.0 / 3)
    
    def test_weighted_average_int(self):
        self.create_node('WeightedAverageInt', {'input[0].value': 5, 'input[0].weight': 1.0,
                                                'input[1].value': 5, 'input[1].weight': 0.5,
                                                'input[2].value': 5, 'input[2].weight': 0.25}, 5.0)
    
    def test_average_angle(self):
        self.create_node('AverageAngle', {'input[0]': 5.0, 'input[1]': 5.0, 'input[2]': 5.0}, 5.0)
    
    def test_weighted_average_angle(self):
        self.create_node('WeightedAverageAngle', {'input[0].value': 5.0, 'input[0].weight': 1.0,
                                                  'input[1].value': 5.0, 'input[1].weight': 0.5,
                                                  'input[2].value': 5.0, 'input[2].weight': 0.25}, 5.0)
    
    def test_average_vector(self):
        third = 1.0 / 3.0
        self.create_node('AverageVector', {'input[0]': [1.0, 0.0, 0.0],
                                           'input[1]': [0.0, 1.0, 0.0],
                                           'input[2]': [0.0, 0.0, 1.0]}, [third, third, third])
    
    def test_weighted_average_vector(self):
        self.create_node('WeightedAverageVector', {'input[0].value': [1.0, 0.0, 0.0], 'input[0].weight': 1.0,
                                                   'input[1].value': [0.0, 1.0, 0.0], 'input[1].weight': 0.5,
                                                   'input[2].value': [0.0, 0.0, 1.0], 'input[2].weight': 0.25},
                         [0.571428, 0.285714, 0.142857])
    
    def test_average_rotation(self):
        self.create_node('AverageRotation', {'input[0]': [0.0, 0.0, 0.0],
                                             'input[1]': [0.0, 90.0, 0.0],
                                             'input[2]': [-90.0, 0.0, 0.0]}, [-32.874, 28.493, -8.567], 3)
    
    def test_weighted_average_rotation(self):
        self.create_node('WeightedAverageRotation', {'input[0].value': [0.0, 0.0, 0.0], 'input[0].weight': 1.0 / 3,
                                                     'input[1].value': [0.0, 90.0, 0.0], 'input[1].weight': 1.0 / 3,
                                                     'input[2].value': [-90.0, 0.0, 0.0], 'input[2].weight': 1.0 / 3},
                         [-32.874, 28.493, -8.567], 3)
    
    def test_average_quaternion(self):
        self.create_node('AverageQuaternion', {'input[0]': [0.0, 0.0, 0.0, 1.0],
                                               'input[1]': [0, 0.7071068, 0, 0.7071068],
                                               'input[2]': [-0.7071068, 0, 0, 0.7071068]},
                         [-0.256, 0.256, 0.0, 0.932], 3)
    
    def test_weighted_average_quaternion(self):
        self.create_node('WeightedAverageQuaternion', {'input[0].value': [0.0, 0.0, 0.0, 1.0], 'input[0].weight': 1.0 / 3,
                                                       'input[1].value': [0, 0.7071068, 0, 0.7071068], 'input[1].weight': 1.0 / 3,
                                                       'input[2].value': [-0.7071068, 0, 0, 0.7071068], 'input[2].weight': 1.0 / 3},
                         [-0.256, 0.256, 0.0, 0.932], 3)
    
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
    
    def test_weighted_average_matrix(self):
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
        
        self.create_node('WeightedAverageMatrix', {'input[0].value': matrix1, 'input[0].weight': 1.0 / 3,
                                                   'input[1].value': matrix2, 'input[1].weight': 1.0 / 3,
                                                   'input[2].value': matrix3, 'input[2].weight': 1.0 / 3}, result, 3)
    
    def test_normalize_array(self):
        self.create_node('NormalizeArray', {'input[0]': 40.0, 'input[1]': 10.0}, [0.8, 0.2])
    
    def test_normalize_weights_array(self):
        self.create_node('NormalizeWeightsArray', {'input[0]': 0.8, 'input[1]': 0.2, 'input[2]': -0.2, 'input[3]': 1.2}, [0.4, 0.1, 0.0, 0.5])
