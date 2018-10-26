# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestNegate(NodeTestCase):
    def test_negate(self):
        self.create_node('Negate', {'input': 2.0}, -2.0)
    
    def test_negate_int(self):
        self.create_node('NegateInt', {'input': 2}, -2)
    
    def test_negate_angle(self):
        self.create_node('NegateAngle', {'input': 2.0}, -2.0)
    
    def test_negate_vector(self):
        self.create_node('NegateVector', {'input': [1.0, -1.0, 0.0]}, [-1.0, 1.0, 0.0])
    
    def test_not_bool(self):
        self.create_node('NotBool', {'input': True}, False)
