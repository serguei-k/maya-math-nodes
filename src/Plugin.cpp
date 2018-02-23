// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#include <maya/MFnPlugin.h>

#include "Absolute.h"
#include "Add.h"
#include "Clamp.h"
#include "Convert.h"
#include "Divide.h"
#include "Inverse.h"
#include "Interpolate.h"
#include "Multiply.h"
#include "Power.h"
#include "Round.h"
#include "Subtract.h"
#include "Trig.h"
#include "Twist.h"
#include "VectorOps.h"

MStatus
initializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj, "Serguei Kalentchouk, et al.", "0.0.1", "Any");
    
    int typeId = 0x0012c340;
    
    // Do not change the registration order after a major release, start a new alphabetized block below instead
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
    CosAngle::registerNode(pluginFn, typeId++);
    CrossProduct::registerNode(pluginFn, typeId++);
    Divide::registerNode(pluginFn, typeId++);
    DivideAngle::registerNode(pluginFn, typeId++);
    DivideAngleByInt::registerNode(pluginFn, typeId++);
    DivideByInt::registerNode(pluginFn, typeId++);
    DotProduct::registerNode(pluginFn, typeId++);
    Floor::registerNode(pluginFn, typeId++);
    FloorAngle::registerNode(pluginFn, typeId++);
    InverseMatrix::registerNode(pluginFn, typeId++);
    InverseQuaternion::registerNode(pluginFn, typeId++);
    Lerp::registerNode(pluginFn, typeId++);
    LerpAngle::registerNode(pluginFn, typeId++);
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
    NormalizeVector::registerNode(pluginFn, typeId++);
    Power::registerNode(pluginFn, typeId++);
    QuaternionFromMatrix::registerNode(pluginFn, typeId++);
    QuaternionFromRotation::registerNode(pluginFn, typeId++);
    Round::registerNode(pluginFn, typeId++);
    RoundAngle::registerNode(pluginFn, typeId++);
    RotationFromMatrix::registerNode(pluginFn, typeId++);
    RotationFromQuaternion::registerNode(pluginFn, typeId++);
    ScaleFromMatrix::registerNode(pluginFn, typeId++);
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
    VectorLengthSquared::registerNode(pluginFn, typeId);
    
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
    CosAngle::deregisterNode(pluginFn);
    CrossProduct::deregisterNode(pluginFn);
    Divide::deregisterNode(pluginFn);
    DivideAngle::deregisterNode(pluginFn);
    DivideAngleByInt::deregisterNode(pluginFn);
    DivideByInt::deregisterNode(pluginFn);
    DotProduct::deregisterNode(pluginFn);
    Floor::deregisterNode(pluginFn);
    FloorAngle::deregisterNode(pluginFn);
    InverseMatrix::deregisterNode(pluginFn);
    InverseQuaternion::deregisterNode(pluginFn);
    Lerp::deregisterNode(pluginFn);
    LerpAngle::deregisterNode(pluginFn);
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
    NormalizeVector::deregisterNode(pluginFn);
    Power::deregisterNode(pluginFn);
    QuaternionFromMatrix::deregisterNode(pluginFn);
    QuaternionFromRotation::deregisterNode(pluginFn);
    Round::deregisterNode(pluginFn);
    RoundAngle::deregisterNode(pluginFn);
    RotationFromMatrix::deregisterNode(pluginFn);
    RotationFromQuaternion::deregisterNode(pluginFn);
    ScaleFromMatrix::deregisterNode(pluginFn);
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
    
    return MS::kSuccess;
}
