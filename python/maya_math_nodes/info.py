# Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
# Use of this source code is governed by an MIT license that can be found in the LICENSE file.

TYPE_SUFFIX_PER_TYPE = {
    'double': '',
    'double3': 'Vector',
    'double4': 'Quaternion',
    'doubleAngle': 'Angle',
    'doubleLinear': '',
    'int': 'Int',
    'matrix': 'Matrix'
}

OPERATORS = ['+', '/', '%', '*', '-']

FUNCTIONS = {
    # operators
    '+': {
        'name': 'math_Add',
        'attributes': ['input1', 'input2'],
        'types': ['double', 'double3', 'doubleAngle', 'doubleLinear', 'int'],
        'mixed_types': {}
    },
    '/': {
        'name': 'math_Divide',
        'attributes': ['input1', 'input2'],
        'types': ['double', 'doubleAngle', 'doubleLinear', 'int'],
        'mixed_types': {'double': ['int'], 'doubleAngle': ['int']}
    },
    '%': {
        'name': 'math_Modulus',
        'attributes': ['input'],
        'types': ['int'],
        'mixed_types': {}
    },
    '*': {
        'name': 'math_Multiply',
        'attributes': ['input1', 'input2'],
        'types': ['double', 'double3', 'doubleAngle', 'doubleLinear', 'int', 'matrix', 'quaternion'],
        'mixed_types': {'double': ['int'], 'double3': ['matrix'], 'doubleAngle': ['int'], 'doubleLinear': ['int']}
    },
    '-': {
        'name': 'math_Subtract',
        'attributes': ['input1', 'input2'],
        'types': ['double', 'double3', 'doubleAngle', 'doubleLinear', 'int'],
        'mixed_types': {}
    },
    
    # functions
    'abs': {
        'name': 'math_Absolute',
        'attributes': ['input'],
        'types': ['double', 'doubleAngle', 'doubleLinear', 'int']
    },
    'clamp': {
        'name': 'math_Clamp',
        'attributes': ['input', 'inputMin', 'inputMax'],
        'types': ['double', 'doubleAngle', 'doubleLinear', 'int']
    },
    'cross': {
        'name': 'math_CrossProduct',
        'attributes': ['input1', 'input2'],
        'types': ['double3']
    },
    'dot': {
        'name': 'math_DotProduct',
        'attributes': ['input1', 'input2'],
        'types': ['double3']
    },
    'length': {
        'name': 'math_VectorLength',
        'attributes': ['input'],
        'types': ['double3']
    }
}
