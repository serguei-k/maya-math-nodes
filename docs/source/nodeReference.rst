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
   In order to achieve consistency and streamlineed workflow, there are a few nodes that duplicate existing Maya functionalility.

The node lbrary tries to adhere to the following set of rules when it comes to choosing the node and attribute names:

- node names are prefixed with `math_`
- nodes are named with affirmative action verbs, ex: `Add`, `Multiply`
- the `get` action verb is implied, ex: `GetDotProduct` is `DotProduct`
- nodes are assumed to operate on doubles by default, ex: `Add`, `Multiply`
- mixed type operations are reflected in the name, ex: `AddVector`, `MultiplyVectorByMatrix`
- conversion nodes have following format `OutputFromSource`, ex: `RotationFromMatrix`
- attributes are generally named `input` and `output`
- if multiple inputs are required they are enumerated, ex: `input1`, `input2`
- for clarity other attribute names are allowed, ex: `translation`, `alpha`, `axis`, `min`


Details
*******

Absolute [Angle, Int]
---------------------
Compute absolute value

Acos
----
Compute arccosine

Add [Angle, Int, Vector]
------------------------
Compute sum of two values

AngleBetweenVectors
-------------------
Compute angle between two vectors

Asin
----
Compute arcsine

Atan
----
Compute arctangent

Atan2
-----
Compute arctangent of `x / y`

AxisFromMatrix
--------------
Get basis vector from matrix for a given axis

.. warning::
   This documentation is not completed, for the full list of nodes please see
   `Plugin.cpp <https://github.com/serguei-k/maya-math-nodes/blob/master/src/Plugin.cpp>`_