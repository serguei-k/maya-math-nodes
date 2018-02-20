# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.
import os
import unittest

import maya.standalone

if __name__ == '__main__':
    print('Maya Math Nodes Unittest')
    
    maya.standalone.initialize(name='python')
    
    root_path = os.path.dirname(os.path.abspath(__file__))
    tests = unittest.TestLoader().discover(root_path, pattern='test_*.py')
    
    test_suite = unittest.TestSuite(tests)
    result = unittest.TextTestRunner(verbosity=2).run(test_suite)
    
    print('Test results:{0}'.format(result))
    
    maya.standalone.uninitialize()
