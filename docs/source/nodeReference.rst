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

Absolute [Angle, Int] | abs
---------------------------
Compute absolute value

Acos | acos
-----------
Compute arccosine

Add [Angle, Int, Vector] | add
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

Ceil [Angle] | ceil
-------------------
Compute the smallest integer value greater than or equal to input

Clamp [Angle, Int] | clamp
--------------------------
Compute the value within the given min and max range

Compare [Angle] | compare
-------------------------
Compute how the two values compare to each other

CosAngle | cos
--------------
Compute the cosine of angle

CrossProduct | cross
--------------------
Compute the cross product of two vectors

Divide [Angle, AngleByInt, ByInt] | /
-------------------------------------
Compute the quotient of two values

DotProduct | dot
----------------
Compute the dot product of two vectors

DistancePoints [Transforms] | distance
--------------------------------------
Compute the distance between two points or matrices

Floor [Angle] | floor
---------------------
Compute the largest integer value less than or equal to input

InverseMatrix [Quaternion, Rotation] | inverse
----------------------------------------------
Compute the inverse of value

Lerp [Angle, Matrix, Vector] | lerp
-----------------------------------
Compute leanear interpolation between two values

MatrixFromTRS
-------------
Compute a matrix from translation, rotation and scale

Max [Angle, Int] | max
----------------------
Get the smallest of the two values

Min [Angle, Int] | min
----------------------
Get the largest of the two values

ModulusInt | %
--------------
Compute the remainder of the two values

Multiply [Angle, AngleByInt, ByInt, Int, Matrix, Quaternion, Rotation, Vector, VectorByMatrix] | *
--------------------------------------------------------------------------------------------------
Compute the product of two values

Negate [Angle, Int, Vector] | negate
------------------------------------
Compute the negation of value

NormalizeVector | normalize
---------------------------
Compute normalized vector

Power | power
-------------
Compute the exponent

QuaternionFromMatrix [FromRotation] | quat
------------------------------------------
Get quatenrion from matrix or rotation

Round [Angle] | round
---------------------
Compute rounded value

RotationFromMatrix [FromQuaternion] | rot
-----------------------------------------
Get rotation from matrix or quaternion

ScaleFromMatrix
---------------
Get scale from matrix

Select [Angle, Int, Matrix, Quaternion, Rotaiton, Vector] | select
------------------------------------------------------------------
Toggle selection

SinAngle | sin
--------------
Compute sin of angle

SlerpQuaternion | slerp
-----------------------
Comptue slerp interpolation between two quaternions

Subtract [Angle, Int, Vector] | -
---------------------------------
Compute the difference between two values

TanAngle | tan
--------------
Compute tangent of angle

TranslationFromMatrix
---------------------
Get translation from matrix

TwistFromMatrix [FromRotaiton] | twist
--------------------------------------
Compute twist around axis from matrix or rotation

VectorLength | length
---------------------
Compute length of vector

VectorLengthSquared | lengthsquared
-----------------------------------
Compute squared length of vector

.. warning::
   This documentation is not completed, for the full list of nodes please see
   `Plugin.cpp <https://github.com/serguei-k/maya-math-nodes/blob/master/src/Plugin.cpp>`_
