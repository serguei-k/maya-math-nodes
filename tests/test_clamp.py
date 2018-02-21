# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
from node_test_case import NodeTestCase, cmds


class TestClamp(NodeTestCase):
    def test_clamp(self):
        node = self.create_node('Clamp', {'input': 5.0, 'inputMin': 0.0, 'inputMax': 2.0}, 2.0)
        
        cmds.setAttr('{0}.{1}'.format(node, 'input'), -1.0)
        self.assertAlmostEqual(cmds.getAttr('{0}.output'.format(node)), 0.0)
    
    def test_clamp_int(self):
        node = self.create_node('Clamp', {'input': 5, 'inputMin': 0, 'inputMax': 2}, 2)
        
        cmds.setAttr('{0}.{1}'.format(node, 'input'), -1)
        self.assertAlmostEqual(cmds.getAttr('{0}.output'.format(node)), 0)
    
    def test_clamp_angle(self):
        node = self.create_node('Clamp', {'input': 5.0, 'inputMin': 0.0, 'inputMax': 2.0}, 2.0)
        
        cmds.setAttr('{0}.{1}'.format(node, 'input'), -1.0)
        self.assertAlmostEqual(cmds.getAttr('{0}.output'.format(node)), 0.0)
