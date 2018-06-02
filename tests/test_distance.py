# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import math
from node_test_case import NodeTestCase


class TestDistance(NodeTestCase):
    def test_distance_points(self):
        self.create_node('DistancePoints', {'input1': [5.0, 0.0, 0.0], 'input2': [0.0, 5.0, 0.0]}, math.sqrt(50.0))
    
    def test_distance_transforms(self):
        matrix1 = [1.0, 0.0, 0.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0,
                   5.0, 0.0, 0.0, 1.0]
        
        matrix2 = [1.0, 0.0, 0.0, 0.0,
                   0.0, 1.0, 0.0, 0.0,
                   0.0, 0.0, 1.0, 0.0,
                   0.0, 5.0, 0.0, 1.0]
        
        self.create_node('DistanceTransforms', {'input1': matrix1, 'input2': matrix2}, math.sqrt(50.0))
