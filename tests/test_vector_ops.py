# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestVectorOps(NodeTestCase):
    def test_dot_product(self):
        self.create_node('DotProduct', {'input1': [1.0, 0.0, 0.0], 'input2': [0.0, 1.0, 0.0]}, 0.0)
    
    def test_cross_product(self):
        self.create_node('CrossProduct', {'input1': [1.0, 0.0, 0.0], 'input2': [0.0, 1.0, 0.0]}, [0.0, 0.0, 1.0])
    
    def test_angle_between(self):
        self.create_node('AngleBetweenVectors', {'input1': [1.0, 0.0, 0.0], 'input2': [0.0, 1.0, 0.0]}, 90.0)
    
    def test_vector_length(self):
        self.create_node('VectorLength', {'input': [1.0, 1.0, 1.0]}, 1.7321)

    def test_vector_length_squared(self):
        self.create_node('VectorLengthSquared', {'input': [1.0, 1.0, 1.0]}, 3.0)
    
    def test_normalize_vector(self):
        self.create_node('NormalizeVector', {'input': [1.0, 1.0, 1.0]}, [0.5774, 0.5774, 0.5774])
