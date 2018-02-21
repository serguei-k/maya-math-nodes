# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestAdd(NodeTestCase):
    def test_add(self):
        self.create_node('Add', {'input1': 5.0, 'input2': -3.0}, 2.0)
    
    def test_add_int(self):
        self.create_node('AddInt', {'input1': 5, 'input2': -3}, 2)
    
    def test_add_angle(self):
        self.create_node('AddAngle', {'input1': 5.0, 'input2': -3.0}, 2.0)
    
    def test_add_vector(self):
        self.create_node('AddVector', {'input1': [1.0, 1.0, 1.0], 'input2': [1.0, -1.0, 0.5]}, [2.0, 0.0, 1.5])
