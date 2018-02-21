# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestMinMax(NodeTestCase):
    def test_min(self):
        self.create_node('Max', {'input1': 1.0, 'input2': 1.5}, 1.5)
    
    def test_max(self):
        self.create_node('Min', {'input1': 1.0, 'input2': 1.5}, 1.0)
    
    def test_min_int(self):
        self.create_node('MaxInt', {'input1': 1, 'input2': 2}, 2)
    
    def test_max_int(self):
        self.create_node('MinInt', {'input1': 1, 'input2': 2}, 1)
    
    def test_min_angle(self):
        self.create_node('MaxAngle', {'input1': 1.0, 'input2': 1.5}, 1.5)
    
    def test_max_angle(self):
        self.create_node('MinAngle', {'input1': 1.0, 'input2': 1.5}, 1.0)
