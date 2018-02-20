# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase


class TestAbsolute(NodeTestCase):
    def test_absolute(self):
        self.create_node('Absolute', {'input': -1.0}, 1.0)
    
    def test_absolute_int(self):
        self.create_node('AbsoluteInt', {'input': 1}, 1)
    
    def test_absolute_angle(self):
        self.create_node('AbsoluteAngle', {'input': -1.0}, 1.0)
