# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestSubtract(NodeTestCase):
    def test_subtract(self):
        self.create_node('Subtract', {'input1': 10.0, 'input2': 5.0}, 5.0)
    
    def test_subtract_int(self):
        self.create_node('SubtractInt', {'input1': 10, 'input2': 5}, 5)
    
    def test_subtract_angle(self):
        self.create_node('SubtractAngle', {'input1': 10.0, 'input2': 5.0}, 5.0)
    
    def test_subtract_vector(self):
        self.create_node('SubtractVector', {'input1': [5.0, 5.0, 5.0], 'input2': [2.0, 2.0, 2.0]}, [3.0, 3.0, 3.0])
