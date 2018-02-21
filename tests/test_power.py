# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestPower(NodeTestCase):
    def test_power(self):
        self.create_node('Power', {'input': 3.0, 'exponent': 2.0}, 9.0)
    
    def test_square_root(self):
        self.create_node('SquareRoot', {'input': 9.0}, 3.0)
