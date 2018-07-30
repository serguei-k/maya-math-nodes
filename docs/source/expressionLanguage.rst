.. _expressionLanguage:

Expression Language
===================

Overview
--------

Even simple math expressions often require relatively large node networks, which are tedious to create by hand.
While this process can be scripted, the code is likewise tedious to write and makes it difficult to see the logic at a glance.

To help alleviate these issues, Maya Math Nodes plugin provide a simple expression language that can be used to describe
a complex mathematical operation which can then be interpreted to and generate the math node network for you.

Data Types
----------

The language supports several data types:

numeric
   float and int types are supported, ex: :code:`-1, 0, 1.0`

string
   string literals are used to reference Maya attributes, ex: :code:`node.attribte[0]`

complex
   complex types are used to define a vector, matrix, rotation, and quaternion data, ex: :code:`{0, 1, 0}`

Operators
---------

| The language supports a limited set of arithmetic operators: :code:`+, -, *, /, %,`
| In addition the following conditional operators are supported: :code:`==, !=, >, <, >=, <=`

Functions
---------

The language supports calling functions with arguments.
These functions map directly to the node operators available in the plugin.

For example :code:`Absolute` node is made available through the :code:`abs()` function call.
Please see the :ref:`Node Reference <nodeReference>` for the mapping between node type and function name.

The function arguments correspond with node attributes. For example the :code:`Clamp` node two input arguments
therefore the :code:`clamp(arg1, arg2)` function will take two arguments.

Likewise, array arguments are also supported with the following syntax: :code:`minelement([1, 2, 3])`.

Output array arguments can also be index using the :code:`[]` operator.

.. note::
   Default and keyword arguments are currently not supported!

Type Resolution
---------------

The operators and functions are mapped to specific Maya nodes shipped with the plugin, and because the node library is strongly typed
the parser needs to make a determination about types using the following rules:

- for operators, the left operand is used to determine primary type
- for conditional expressions, the true value is used to determine primary selector type
- for functions, the first argument is used to determine primary type
- if operand or argument is literal numeric type then casting to another numeric type is allowed

Name Generator
--------------

The expression evaluator will create Maya nodes procedurally and therefore needs a mechanism to generate unique names consistently.

This is achieved with the :code:`NameGenerator` class. To customize this behavior you can create your own implementation, with the
only requirement that it implements :code:`get_name(str: node_type) -> str` method.

Evaluator
---------

| The public API for this module consist of a single function:
| :code:`eval_expression(str: expression, str: base_node_name='', NameGenerator: name_generator=None) -> str`

The return value is the path to the output attribute of the last node in the generated node network that will
have the result value computed for the expression. This value can then be passed to subsequent expressions to chain them together.

Examples
--------

.. code:: python

  from maya_math_nodes import eval_expression

  # get twist value for roll joint
  eval_expression('twist(ctrl.worldMatrix[0], 0, 0) * 0.5', 'roll')

  # get toe pivot value for foot roll
  eval_expression('ctrl.roll > ctrl.break ? ctrl.roll - ctrl.break : 0', 'toeroll')

  # compute some pole vector with offset
  eval_expression('cross(axis(ctrl.matrix, 0), {0.0, 1.0, 0.0}) * 2', 'pole')
