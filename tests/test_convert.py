# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestConvert(NodeTestCase):
    def test_rotation_from_matrix(self):
        matrix = [0.0, 0.0, -1.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  1.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('RotationFromMatrix', {'input': matrix}, [0.0, 90.0, 0.0])
    
    def test_rotation_from_quaternion(self):
        self.create_node('RotationFromQuaternion', {'input': [0.7071, 0.0, 0.0, 0.7071]}, [90.0, 0.0, 0.0])
    
    def test_quaternion_from_matrix(self):
        matrix = [1.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  0.0, -1.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('QuaternionFromMatrix', {'input': matrix}, [0.7071, 0.0, 0.0, 0.7071])
    
    def test_quaternion_from_rotation(self):
        self.create_node('QuaternionFromRotation', {'input': [90.0, 0.0, 0.0]}, [0.7071, 0.0, 0.0, 0.7071])
    
    def test_translation_from_matrix(self):
        matrix = [1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  1.0, 2.0, 3.0, 1.0]
        
        self.create_node('TranslationFromMatrix', {'input': matrix}, [1.0, 2.0, 3.0])
    
    def test_matrix_from_rotation(self):
        matrix = [0.0, 0.0, -1.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  1.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('MatrixFromRotation', {'input': [0.0, 90.0, 0.0]}, matrix)
    
    def test_matrix_from_quaternion(self):
        matrix = [1.0, 0.0, 0.0, 0.0,
                  0.0, 0.0, 1.0, 0.0,
                  0.0, -1.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('MatrixFromQuaternion', {'input': [0.7071, 0.0, 0.0, 0.7071]}, matrix)
    
    def test_matrix_from_direction(self):
        matrix = [0.0, 0.0, 1.0, 0.0,
                  1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('MatrixFromDirection',
                         {'direction': [1.0, 0.0, 0.0], 'up': [0.0, 0.0, 1.0], 'alignment': 2}, matrix)
    
    def test_scale_from_matrix(self):
        matrix = [0.5, 0.0, 0.0, 0.0,
                  0.0, 0.5, 0.0, 0.0,
                  0.0, 0.0, 0.5, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('ScaleFromMatrix', {'input': matrix}, [0.5, 0.5, 0.5])
    
    def test_matrix_from_trs(self):
        result = [0.0, 0.0, -1.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  1.0, 0.0, 0.0, 0.0,
                  1.0, 2.0, 3.0, 1.0]
        
        self.create_node('MatrixFromTRS',
                         {'translation': [1.0, 2.0, 3.0], 'rotation': [0.0, 90.0, 0.0]},
                         result)
    
    def test_axis_from_matrix(self):
        matrix = [0.5, 0.0, 0.0, 0.0,
                  0.0, 0.5, 0.0, 0.0,
                  0.0, 0.0, 0.5, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('AxisFromMatrix', {'input': matrix, 'axis': 1}, [0.0, 0.5, 0.0])
