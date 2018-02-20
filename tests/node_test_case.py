# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import unittest
import maya.cmds as cmds

plugin_name = 'mayaMathNodes'


class NodeTestCase(unittest.TestCase):
    """Maya Node Test Case
    
    The test case assumes that the plugin is either already loaded or is discoverable by Maya.
    """
    @classmethod
    def setUpClass(cls):
        if not cmds.pluginInfo(plugin_name, query=True, loaded=True):
            cmds.loadPlugin(plugin_name)
    
    def setUp(self):
        cmds.file(new=True, force=True)
