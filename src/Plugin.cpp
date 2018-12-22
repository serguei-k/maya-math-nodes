// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#include <maya/MFnPlugin.h>

#include "Absolute.h"
#include "Add.h"
#include "Array.h"
#include "Clamp.h"
#include "Condition.h"
#include "Convert.h"
#include "Debug.h"
#include "Distance.h"
#include "Divide.h"
#include "Inverse.h"
#include "Interpolate.h"
#include "Multiply.h"
#include "Negate.h"
#include "Power.h"
#include "Round.h"
#include "Subtract.h"
#include "Trig.h"
#include "Twist.h"
#include "VectorOps.h"

MStatus
initializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj, "Serguei Kalentchouk, et al.", PROJECT_VERSION, "Any");
    
    int typeId = 0x0012c340;
    
    // 1.0.0
    // Do not change the registration order after a release, start a new alphabetized block below instead
    Absolute::registerNode(pluginFn, typeId++);
    AbsoluteAngle::registerNode(pluginFn, typeId++);
    AbsoluteInt::registerNode(pluginFn, typeId++);
    Acos::registerNode(pluginFn, typeId++);
    Add::registerNode(pluginFn, typeId++);
    AddAngle::registerNode(pluginFn, typeId++);
    AddInt::registerNode(pluginFn, typeId++);
    AddVector::registerNode(pluginFn, typeId++);
    AngleBetweenVectors::registerNode(pluginFn, typeId++);
    Asin::registerNode(pluginFn, typeId++);
    Atan::registerNode(pluginFn, typeId++);
    Atan2::registerNode(pluginFn, typeId++);
    AxisFromMatrix::registerNode(pluginFn, typeId++);
    Ceil::registerNode(pluginFn, typeId++);
    CeilAngle::registerNode(pluginFn, typeId++);
    Clamp::registerNode(pluginFn, typeId++);
    ClampAngle::registerNode(pluginFn, typeId++);
    ClampInt::registerNode(pluginFn, typeId++);
    Compare::registerNode(pluginFn, typeId++);
    CompareAngle::registerNode(pluginFn, typeId++);
    CosAngle::registerNode(pluginFn, typeId++);
    CrossProduct::registerNode(pluginFn, typeId++);
    Divide::registerNode(pluginFn, typeId++);
    DivideAngle::registerNode(pluginFn, typeId++);
    DivideAngleByInt::registerNode(pluginFn, typeId++);
    DivideByInt::registerNode(pluginFn, typeId++);
    DotProduct::registerNode(pluginFn, typeId++);
    DistancePoints::registerNode(pluginFn, typeId++);
    DistanceTransforms::registerNode(pluginFn, typeId++);
    Floor::registerNode(pluginFn, typeId++);
    FloorAngle::registerNode(pluginFn, typeId++);
    InverseMatrix::registerNode(pluginFn, typeId++);
    InverseQuaternion::registerNode(pluginFn, typeId++);
    InverseRotation::registerNode(pluginFn, typeId++);
    Lerp::registerNode(pluginFn, typeId++);
    LerpAngle::registerNode(pluginFn, typeId++);
    LerpMatrix::registerNode(pluginFn, typeId++);
    LerpVector::registerNode(pluginFn, typeId++);
    MatrixFromTRS::registerNode(pluginFn, typeId++);
    Max::registerNode(pluginFn, typeId++);
    MaxAngle::registerNode(pluginFn, typeId++);
    MaxInt::registerNode(pluginFn, typeId++);
    Min::registerNode(pluginFn, typeId++);
    MinAngle::registerNode(pluginFn, typeId++);
    MinInt::registerNode(pluginFn, typeId++);
    ModulusInt::registerNode(pluginFn, typeId++);
    Multiply::registerNode(pluginFn, typeId++);
    MultiplyAngle::registerNode(pluginFn, typeId++);
    MultiplyAngleByInt::registerNode(pluginFn, typeId++);
    MultiplyByInt::registerNode(pluginFn, typeId++);
    MultiplyInt::registerNode(pluginFn, typeId++);
    MultiplyMatrix::registerNode(pluginFn, typeId++);
    MultiplyVector::registerNode(pluginFn, typeId++);
    MultiplyVectorByMatrix::registerNode(pluginFn, typeId++);
    MultiplyQuaternion::registerNode(pluginFn, typeId++);
    MultiplyRotation::registerNode(pluginFn, typeId++);
    Negate::registerNode(pluginFn, typeId++);
    NegateAngle::registerNode(pluginFn, typeId++);
    NegateInt::registerNode(pluginFn, typeId++);
    NegateVector::registerNode(pluginFn, typeId++);
    NormalizeVector::registerNode(pluginFn, typeId++);
    Power::registerNode(pluginFn, typeId++);
    QuaternionFromMatrix::registerNode(pluginFn, typeId++);
    QuaternionFromRotation::registerNode(pluginFn, typeId++);
    Round::registerNode(pluginFn, typeId++);
    RoundAngle::registerNode(pluginFn, typeId++);
    RotationFromMatrix::registerNode(pluginFn, typeId++);
    RotationFromQuaternion::registerNode(pluginFn, typeId++);
    ScaleFromMatrix::registerNode(pluginFn, typeId++);
    Select::registerNode(pluginFn, typeId++);
    SelectAngle::registerNode(pluginFn, typeId++);
    SelectInt::registerNode(pluginFn, typeId++);
    SelectMatrix::registerNode(pluginFn, typeId++);
    SelectQuaternion::registerNode(pluginFn, typeId++);
    SelectRotation::registerNode(pluginFn, typeId++);
    SelectVector::registerNode(pluginFn, typeId++);
    SinAngle::registerNode(pluginFn, typeId++);
    SlerpQuaternion::registerNode(pluginFn, typeId++);
    Subtract::registerNode(pluginFn, typeId++);
    SubtractAngle::registerNode(pluginFn, typeId++);
    SubtractInt::registerNode(pluginFn, typeId++);
    SubtractVector::registerNode(pluginFn, typeId++);
    SquareRoot::registerNode(pluginFn, typeId++);
    TanAngle::registerNode(pluginFn, typeId++);
    TranslationFromMatrix::registerNode(pluginFn, typeId++);
    TwistFromMatrix::registerNode(pluginFn, typeId++);
    TwistFromRotation::registerNode(pluginFn, typeId++);
    VectorLength::registerNode(pluginFn, typeId++);
    VectorLengthSquared::registerNode(pluginFn, typeId++);
    
    // 1.1.0
    AndBool::registerNode(pluginFn, typeId++);
    AndInt::registerNode(pluginFn, typeId++);
    Average::registerNode(pluginFn, typeId++);
    AverageAngle::registerNode(pluginFn, typeId++);
    AverageInt::registerNode(pluginFn, typeId++);
    AverageMatrix::registerNode(pluginFn, typeId++);
    AverageRotation::registerNode(pluginFn, typeId++);
    AverageVector::registerNode(pluginFn, typeId++);
    AverageQuaternion::registerNode(pluginFn, typeId++);
    OrBool::registerNode(pluginFn, typeId++);
    OrInt::registerNode(pluginFn, typeId++);
    Sum::registerNode(pluginFn, typeId++);
    SumAngle::registerNode(pluginFn, typeId++);
    SumInt::registerNode(pluginFn, typeId++);
    SumVector::registerNode(pluginFn, typeId++);
    WeightedAverage::registerNode(pluginFn, typeId++);
    WeightedAverageAngle::registerNode(pluginFn, typeId++);
    WeightedAverageInt::registerNode(pluginFn, typeId++);
    WeightedAverageMatrix::registerNode(pluginFn, typeId++);
    WeightedAverageQuaternion::registerNode(pluginFn, typeId++);
    WeightedAverageRotation::registerNode(pluginFn, typeId++);
    WeightedAverageVector::registerNode(pluginFn, typeId++);
    XorBool::registerNode(pluginFn, typeId++);
    XorInt::registerNode(pluginFn, typeId++);
    
    // 1.2.0
    MaxElement::registerNode(pluginFn, typeId++);
    MaxAngleElement::registerNode(pluginFn, typeId++);
    MaxIntElement::registerNode(pluginFn, typeId++);
    MinElement::registerNode(pluginFn, typeId++);
    MinAngleElement::registerNode(pluginFn, typeId++);
    MinIntElement::registerNode(pluginFn, typeId++);
    NormalizeArray::registerNode(pluginFn, typeId++);
    NormalizeWeightsArray::registerNode(pluginFn, typeId++);
    SelectArray::registerNode(pluginFn, typeId++);
    SelectAngleArray::registerNode(pluginFn, typeId++);
    SelectIntArray::registerNode(pluginFn, typeId++);
    SelectMatrixArray::registerNode(pluginFn, typeId++);
    SelectVectorArray::registerNode(pluginFn, typeId++);
    
    // 1.5.0
    DebugLog::registerNode(pluginFn, typeId++);
    DebugLogAngle::registerNode(pluginFn, typeId++);
    DebugLogInt::registerNode(pluginFn, typeId++);
    DebugLogVector::registerNode(pluginFn, typeId++);
    DebugLogRotation::registerNode(pluginFn, typeId++);
    DebugLogQuaternion::registerNode(pluginFn, typeId++);
    DebugLogMatrix::registerNode(pluginFn, typeId++);
    CompareInt::registerNode(pluginFn, typeId++);
    MatrixFromDirection::registerNode(pluginFn, typeId++);
    MatrixFromQuaternion::registerNode(pluginFn, typeId++);
    MatrixFromRotation::registerNode(pluginFn, typeId++);
    NotBool::registerNode(pluginFn, typeId++);
    Remap::registerNode(pluginFn, typeId++);
    RemapAngle::registerNode(pluginFn, typeId++);
    RemapInt::registerNode(pluginFn, typeId++);
    SelectCurve::registerNode(pluginFn, typeId++);
    SelectMesh::registerNode(pluginFn, typeId++);
    SelectSurface::registerNode(pluginFn, typeId++);
    Smoothstep::registerNode(pluginFn, typeId++);
    
    return MS::kSuccess;
}

MStatus
uninitializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj);
    
    Absolute::deregisterNode(pluginFn);
    AbsoluteAngle::deregisterNode(pluginFn);
    AbsoluteInt::deregisterNode(pluginFn);
    Acos::deregisterNode(pluginFn);
    Add::deregisterNode(pluginFn);
    AddAngle::deregisterNode(pluginFn);
    AddInt::deregisterNode(pluginFn);
    AddVector::deregisterNode(pluginFn);
    AngleBetweenVectors::deregisterNode(pluginFn);
    Asin::deregisterNode(pluginFn);
    Atan::deregisterNode(pluginFn);
    Atan2::deregisterNode(pluginFn);
    AxisFromMatrix::deregisterNode(pluginFn);
    Ceil::deregisterNode(pluginFn);
    CeilAngle::deregisterNode(pluginFn);
    Clamp::deregisterNode(pluginFn);
    ClampAngle::deregisterNode(pluginFn);
    ClampInt::deregisterNode(pluginFn);
    Compare::deregisterNode(pluginFn);
    CompareAngle::deregisterNode(pluginFn);
    CosAngle::deregisterNode(pluginFn);
    CrossProduct::deregisterNode(pluginFn);
    Divide::deregisterNode(pluginFn);
    DivideAngle::deregisterNode(pluginFn);
    DivideAngleByInt::deregisterNode(pluginFn);
    DivideByInt::deregisterNode(pluginFn);
    DotProduct::deregisterNode(pluginFn);
    DistancePoints::deregisterNode(pluginFn);
    DistanceTransforms::deregisterNode(pluginFn);
    Floor::deregisterNode(pluginFn);
    FloorAngle::deregisterNode(pluginFn);
    InverseMatrix::deregisterNode(pluginFn);
    InverseQuaternion::deregisterNode(pluginFn);
    InverseRotation::deregisterNode(pluginFn);
    Lerp::deregisterNode(pluginFn);
    LerpAngle::deregisterNode(pluginFn);
    LerpMatrix::deregisterNode(pluginFn);
    LerpVector::deregisterNode(pluginFn);
    MatrixFromTRS::deregisterNode(pluginFn);
    Max::deregisterNode(pluginFn);
    MaxAngle::deregisterNode(pluginFn);
    MaxInt::deregisterNode(pluginFn);
    Min::deregisterNode(pluginFn);
    MinAngle::deregisterNode(pluginFn);
    MinInt::deregisterNode(pluginFn);
    ModulusInt::deregisterNode(pluginFn);
    Multiply::deregisterNode(pluginFn);
    MultiplyAngle::deregisterNode(pluginFn);
    MultiplyAngleByInt::deregisterNode(pluginFn);
    MultiplyByInt::deregisterNode(pluginFn);
    MultiplyInt::deregisterNode(pluginFn);
    MultiplyMatrix::deregisterNode(pluginFn);
    MultiplyVector::deregisterNode(pluginFn);
    MultiplyVectorByMatrix::deregisterNode(pluginFn);
    MultiplyQuaternion::deregisterNode(pluginFn);
    MultiplyRotation::deregisterNode(pluginFn);
    Negate::deregisterNode(pluginFn);
    NegateAngle::deregisterNode(pluginFn);
    NegateInt::deregisterNode(pluginFn);
    NegateVector::deregisterNode(pluginFn);
    NormalizeVector::deregisterNode(pluginFn);
    Power::deregisterNode(pluginFn);
    QuaternionFromMatrix::deregisterNode(pluginFn);
    QuaternionFromRotation::deregisterNode(pluginFn);
    Round::deregisterNode(pluginFn);
    RoundAngle::deregisterNode(pluginFn);
    RotationFromMatrix::deregisterNode(pluginFn);
    RotationFromQuaternion::deregisterNode(pluginFn);
    ScaleFromMatrix::deregisterNode(pluginFn);
    Select::deregisterNode(pluginFn);
    SelectAngle::deregisterNode(pluginFn);
    SelectInt::deregisterNode(pluginFn);
    SelectMatrix::deregisterNode(pluginFn);
    SelectQuaternion::deregisterNode(pluginFn);
    SelectRotation::deregisterNode(pluginFn);
    SelectVector::deregisterNode(pluginFn);
    SinAngle::deregisterNode(pluginFn);
    SlerpQuaternion::deregisterNode(pluginFn);
    Subtract::deregisterNode(pluginFn);
    SubtractAngle::deregisterNode(pluginFn);
    SubtractInt::deregisterNode(pluginFn);
    SubtractVector::deregisterNode(pluginFn);
    SquareRoot::deregisterNode(pluginFn);
    TanAngle::deregisterNode(pluginFn);
    TranslationFromMatrix::deregisterNode(pluginFn);
    TwistFromMatrix::deregisterNode(pluginFn);
    TwistFromRotation::deregisterNode(pluginFn);
    VectorLength::deregisterNode(pluginFn);
    VectorLengthSquared::deregisterNode(pluginFn);
    
    AndBool::deregisterNode(pluginFn);
    AndInt::deregisterNode(pluginFn);
    Average::deregisterNode(pluginFn);
    AverageAngle::deregisterNode(pluginFn);
    AverageInt::deregisterNode(pluginFn);
    AverageMatrix::deregisterNode(pluginFn);
    AverageRotation::deregisterNode(pluginFn);
    AverageVector::deregisterNode(pluginFn);
    AverageQuaternion::deregisterNode(pluginFn);
    OrBool::deregisterNode(pluginFn);
    OrInt::deregisterNode(pluginFn);
    Sum::deregisterNode(pluginFn);
    SumAngle::deregisterNode(pluginFn);
    SumInt::deregisterNode(pluginFn);
    SumVector::deregisterNode(pluginFn);
    WeightedAverage::deregisterNode(pluginFn);
    WeightedAverageAngle::deregisterNode(pluginFn);
    WeightedAverageInt::deregisterNode(pluginFn);
    WeightedAverageMatrix::deregisterNode(pluginFn);
    WeightedAverageQuaternion::deregisterNode(pluginFn);
    WeightedAverageRotation::deregisterNode(pluginFn);
    WeightedAverageVector::deregisterNode(pluginFn);
    XorBool::deregisterNode(pluginFn);
    XorInt::deregisterNode(pluginFn);
    
    MaxElement::deregisterNode(pluginFn);
    MaxAngleElement::deregisterNode(pluginFn);
    MaxIntElement::deregisterNode(pluginFn);
    MinElement::deregisterNode(pluginFn);
    MinAngleElement::deregisterNode(pluginFn);
    MinIntElement::deregisterNode(pluginFn);
    NormalizeArray::deregisterNode(pluginFn);
    NormalizeWeightsArray::deregisterNode(pluginFn);
    SelectArray::deregisterNode(pluginFn);
    SelectAngleArray::deregisterNode(pluginFn);
    SelectIntArray::deregisterNode(pluginFn);
    SelectMatrixArray::deregisterNode(pluginFn);
    SelectVectorArray::deregisterNode(pluginFn);
    
    DebugLog::deregisterNode(pluginFn);
    DebugLogAngle::deregisterNode(pluginFn);
    DebugLogInt::deregisterNode(pluginFn);
    DebugLogVector::deregisterNode(pluginFn);
    DebugLogRotation::deregisterNode(pluginFn);
    DebugLogQuaternion::deregisterNode(pluginFn);
    DebugLogMatrix::deregisterNode(pluginFn);
    CompareInt::deregisterNode(pluginFn);
    MatrixFromDirection::deregisterNode(pluginFn);
    MatrixFromQuaternion::deregisterNode(pluginFn);
    MatrixFromRotation::deregisterNode(pluginFn);
    NotBool::deregisterNode(pluginFn);
    Remap::deregisterNode(pluginFn);
    RemapAngle::deregisterNode(pluginFn);
    RemapInt::deregisterNode(pluginFn);
    SelectCurve::deregisterNode(pluginFn);
    SelectMesh::deregisterNode(pluginFn);
    SelectSurface::deregisterNode(pluginFn);
    Smoothstep::deregisterNode(pluginFn);
    
    return MS::kSuccess;
}
