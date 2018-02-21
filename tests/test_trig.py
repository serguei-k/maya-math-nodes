# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestTrig(NodeTestCase):
    def test_sin(self):
        self.create_node('SinAngle', {'input': 90.0}, 1.0)
    
    def test_asin(self):
        self.create_node('Asin', {'input': 1.0}, 90.0)
    
    def test_cos(self):
        self.create_node('CosAngle', {'input': 180.0}, -1.0)
    
    def test_acos(self):
        self.create_node('Acos', {'input': -1.0}, 180.0)
    
    def test_tan(self):
        self.create_node('TanAngle', {'input': 45.0}, 1.0)
    
    def test_atan(self):
        self.create_node('Atan', {'input': 1.0}, 45.0)
    
    def test_atan2(self):
        self.create_node('Atan2', {'input1': 1.0, 'input2': 0.0}, 90.0)
