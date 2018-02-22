# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestTwist(NodeTestCase):
    def test_twist_from_matrix(self):
        matrix = [0.0, 0.0, 1.0, 0.0,
                  1.0, 0.0, 0.0, 0.0,
                  0.0, 1.0, 0.0, 0.0,
                  0.0, 0.0, 0.0, 1.0]
        
        self.create_node('TwistFromMatrix', {'input': matrix}, -90.0)
    
    def test_twist_from_rotation(self):
        self.create_node('TwistFromRotation', {'input': [0.0, -90.0, -90.0]}, -90.0)
