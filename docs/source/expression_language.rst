.. _expression_language:

Expression Language
===================

Overview
--------

Even simple math expressions often require relatively large node networks, which are tedious to create by hand.
While this process can be scripted, the code is likewise tedious to write and makes it difficult to see the logic at a glance.

To help alleviate these issues, Maya Math Nodes plugin provide a simple expression language that can be used to describe a series of mathematical operations inline,
which can then be interpreted to generate a math node network for you. For example:

.. code:: python

  # project vector to plane
  eval_expression('node.t - (vec(0, 1, 0) * dot(node.t, vec(0, 1, 0)))', 'projectToPlane')

Data Types
----------

The language supports the following data types:

numeric
   float and int types are supported, ex: :code:`-1, 0, 1.0`

boolean
   boolean **true** and **false** values are supported and can cast to POD numeric types

string
   string literals are used to reference Maya attributes, ex: :code:`node.attribute[0]`,
   note that there are no quotation marks around the string literals!

complex
   complex types such as vector, matrix, rotation, and quaternion are specified by using cast functions, ex: :code:`vec(0, 1, 0)`

geometry
   a small subset of functions also supports geometry types such as mesh, nurbsCurve, and nurbsSurface

Operators
---------

The language supports a limited set of arithmetic and logical operators: :code:`+, -, *, /, %, &, |, ^, !`

Conditionals
------------

The language supports the following relational operators: :code:`==, !=, >, <, >=, <=`

These are used in combination with ternary conditional expression: :code:`a == b ? true : false`

Functions
---------

The language supports calling functions with arguments.
These functions map directly to the node operators available in the plugin.

For example :code:`Absolute` node is made available through the :code:`abs()` function call.
Please see the :ref:`Node Reference <node_reference>` for the mapping between node type and function name.

The function arguments correspond with node attributes. For example the :code:`Clamp` node has two input
attributes, therefore the :code:`clamp(arg1, arg2)` function will take two arguments.

Likewise, array arguments are also supported with the following syntax: :code:`minelement([1, 2, 3])`.

Output array arguments can also be index using the :code:`[]` operator.

Cast Functions
++++++++++++++

Several functions that output complex data types can take constant values as input.

mat
  :code:`mat(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1)` can be used to specify constant matrix value,
  :code:`mat()` also maps to several math nodes and can take other arguments, ex: :code:`mat(node.rotate, 0)`

rot
  :code:`rot(0, 1, 0)` can be used to specify constant rotation value, :code:`rot()` also maps to several
  math nodes and can take other arguments, ex: :code:`rot(node.matrix, 0)`

quat
  :code:`quat(0, 0, 0, 1)` can be used to specify constant quaternion value, :code:`quat()` also maps to
  several math nodes and can take other arguments, ex: :code:`quat(node.rotation, 0)`

vec
  :code:`vec(1, 0, 0)` can be used to specify a constant vector value

.. warning::
   | Currently, some nodes do not have expression bindings!
   | See :ref:`Node Reference <node_reference>` section for details.

.. note::
   Function calls require at least one argument to be specified!

Evaluation Order
----------------

Expressions are evaluated left to right with the following operator precedence, listed from lowest to highest:

+----------------------------+-------------------------------------+
| Operator                   | Description                         |
+----------------------------+-------------------------------------+
| <, <=, >, >=, !=, ==, ?, : | Comparisons and ternary             |
+----------------------------+-------------------------------------+
| &, |, ^, !                 | Logical operators                   |
+----------------------------+-------------------------------------+
| +, -                       | Addition and subtraction            |
+----------------------------+-------------------------------------+
| \*, /, %                   | Multiplication, division, remainder |
+----------------------------+-------------------------------------+
| func()                     | Function call                       |
+----------------------------+-------------------------------------+
| (...)                      | Grouping                            |
+----------------------------+-------------------------------------+

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
  eval_expression('twist(ctrl.worldMatrix[0]) * 0.5', 'roll')

  # get toe pivot value for foot roll
  eval_expression('ctrl.roll > ctrl.break ? ctrl.roll - ctrl.break : 0', 'toeroll')

  # compute some pole vector with offset
  eval_expression('cross(axis(ctrl.matrix, 0), vec(0, 1, 0)) * 2', 'pole')
