.. _expressionLanguage:

Expression Language
===================

Overview
--------

Even simle math expressions often require relatively large node networks, which are tedrious to create by hand.
While this process can be scripted, the resulting code is likewise tedious to write and makes it difficult to see the logic at a glance.

To help alleviate these issues, Maya Math Nodes plugin provide a simple expression language that can be used to descibe
a complex mathematical operation which can then be interpreted to and generate a math node network for you.

Data Types
----------

The language supports several data types:

numeric
   float and int types are supported, ex: `-1, 0, 1.0`

string
   string literals are used to reference Maya attributes, ex: `node.attribte[0]`

complex
   complex types are used to define a vector, rotation, and quaternion data, ex: `{0, 1, 0}`

Operators
---------

