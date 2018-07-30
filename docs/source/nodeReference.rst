.. _nodeReference:

Node Reference
==============

Overview
********

The nodes are designed with the following principles in mind:

- nodes perform a single operation
- nodes have a single output attribute
- nodes are strongly typed

.. note::
   In order to achieve consistency and streamlined workflow, there are a few nodes that duplicate existing Maya functionality.

The node library tries to adhere to the following set of rules when it comes to choosing the node and attribute names:

- node names are prefixed with :code:`math_`
- nodes are named with affirmative action verbs, ex: :code:`Add`, :code:`Multiply`
- the `get` action verb is implied, ex: :code:`GetDotProduct` is :code:`DotProduct`
- nodes are assumed to operate on doubles by default, ex: :code:`Add`, :code:`Multiply`
- mixed type operations are reflected in the name, ex: :code:`AddVector`, :code:`MultiplyVectorByMatrix`
- conversion nodes have following format `OutputFromSource`, ex: :code:`RotationFromMatrix`
- attributes are generally named :code:`input` and :code:`output`
- if multiple inputs are required they are enumerated, ex: :code:`input1`, :code:`input2`
- for clarity other attribute names are allowed, ex: :code:`translation`, :code:`alpha`, :code:`axis`, :code:`min`


Details
*******

| **Format**:
| Node Type Name [Type Variants,] | Expression Function Name

Absolute [angle, int] | abs
---------------------------
Compute absolute value

Acos | acos
-----------
Compute arccosine

Add [angle, int, vector] | add
------------------------------
Compute sum of two values

AngleBetweenVectors | anglebetween
----------------------------------
Compute angle between two vectors

Asin | asin
-----------
Compute arcsine

Atan | atan
-----------
Compute arctangent

Atan2 | atan2
-------------
Compute arctangent of `x / y`

AxisFromMatrix | axis
---------------------
Get basis vector from matrix for a given axis

.. warning::
   This documentation is not completed, for the full list of nodes please see
   `Plugin.cpp <https://github.com/serguei-k/maya-math-nodes/blob/master/src/Plugin.cpp>`_