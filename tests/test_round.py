# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestRound(NodeTestCase):
    def test_round(self):
        self.create_node('Round', {'input': 1.4}, 1.0)
    
    def test_round_angle(self):
        self.create_node('RoundAngle', {'input': 1.4}, 1.0)
    
    def test_floor(self):
        self.create_node('Floor', {'input': 1.8}, 1.0)
    
    def test_floor_angle(self):
        self.create_node('FloorAngle', {'input': 1.8}, 1.0)
    
    def test_ceil(self):
        self.create_node('Ceil', {'input': 1.3}, 2.0)
    
    def test_ceil_angle(self):
        self.create_node('CeilAngle', {'input': 1.3}, 2.0)
