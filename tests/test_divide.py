# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestDivide(NodeTestCase):
    def test_divide(self):
        self.create_node('Divide', {'input1': 10.0, 'input2': 2.0}, 5.0)
    
    def test_divide_by_int(self):
        self.create_node('DivideByInt', {'input1': 10.0, 'input2': 2}, 5.0)
    
    def test_divide_angle(self):
        self.create_node('DivideAngle', {'input1': 10.0, 'input2': 2.0}, 5.0)
    
    def test_divide_angle_by_int(self):
        self.create_node('DivideAngleByInt', {'input1': 10.0, 'input2': 2}, 5.0)
    
    def test_modulus_int(self):
        self.create_node('ModulusInt', {'input1': 5, 'input2': 4}, 1)
