// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#include <maya/MFnPlugin.h>

#include "Absolute.h"
#include "Add.h"
#include "Clamp.h"
#include "Convert.h"
#include "Divide.h"
#include "Inverse.h"
#include "Lerp.h"
#include "Multiply.h"
#include "Power.h"
#include "Subtract.h"
#include "Trig.h"
#include "VectorOps.h"

MStatus
initializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj, "Serguei Kalentchouk, et al.", "0.0.1", "Any");
    
    //TODO: change to real node IDs
    int typeId = 1000;
    
    Absolute::registerNode(pluginFn, ++typeId);
    AcosAngle::registerNode(pluginFn, ++typeId);
    Add::registerNode(pluginFn, ++typeId);
    AddAngle::registerNode(pluginFn, ++typeId);
    AddInt::registerNode(pluginFn, ++typeId);
    AddVector::registerNode(pluginFn, ++typeId);
    AngleBetweenVectors::registerNode(pluginFn, ++typeId);
    AsinAngle::registerNode(pluginFn, ++typeId);
    AtanAngle::registerNode(pluginFn, ++typeId);
    Clamp::registerNode(pluginFn, ++typeId);
    ClampAngle::registerNode(pluginFn, ++typeId);
    ClampInt::registerNode(pluginFn, ++typeId);
    Cos::registerNode(pluginFn, ++typeId);
    CrossProduct::registerNode(pluginFn, ++typeId);
    Divide::registerNode(pluginFn, ++typeId);
    DivideAngle::registerNode(pluginFn, ++typeId);
    DivideAngleByInt::registerNode(pluginFn, ++typeId);
    DivideByInt::registerNode(pluginFn, ++typeId);
    DotProduct::registerNode(pluginFn, ++typeId);
    InverseMatrix::registerNode(pluginFn, ++typeId);
    InverseQuaternion::registerNode(pluginFn, ++typeId);
    Lerp::registerNode(pluginFn, ++typeId);
    LerpAngle::registerNode(pluginFn, ++typeId);
    LerpVector::registerNode(pluginFn, ++typeId);
    Max::registerNode(pluginFn, ++typeId);
    Min::registerNode(pluginFn, ++typeId);
    MaxInt::registerNode(pluginFn, ++typeId);
    MinInt::registerNode(pluginFn, ++typeId);
    MaxAngle::registerNode(pluginFn, ++typeId);
    MinAngle::registerNode(pluginFn, ++typeId);
    Multiply::registerNode(pluginFn, ++typeId);
    MultiplyAngle::registerNode(pluginFn, ++typeId);
    MultiplyAngleByInt::registerNode(pluginFn, ++typeId);
    MultiplyByInt::registerNode(pluginFn, ++typeId);
    MultiplyInt::registerNode(pluginFn, ++typeId);
    MultiplyMatrix::registerNode(pluginFn, ++typeId);
    MultiplyVector::registerNode(pluginFn, ++typeId);
    MultiplyVectorByMatrix::registerNode(pluginFn, ++typeId);
    MultiplyQuaternion::registerNode(pluginFn, ++typeId);
    NormalizeVector::registerNode(pluginFn, ++typeId);
    Power::registerNode(pluginFn, ++typeId);
    RotationFromMatrix::registerNode(pluginFn, ++typeId);
    RotationFromQuaternion::registerNode(pluginFn, ++typeId);
    Sin::registerNode(pluginFn, ++typeId);
    Subtract::registerNode(pluginFn, ++typeId);
    SubtractAngle::registerNode(pluginFn, ++typeId);
    SubtractInt::registerNode(pluginFn, ++typeId);
    SubtractVector::registerNode(pluginFn, ++typeId);
    SquareRoot::registerNode(pluginFn, ++typeId);
    Tan::registerNode(pluginFn, ++typeId);
    VectorLength::registerNode(pluginFn, ++typeId);
    
    return MS::kSuccess;
}

MStatus
uninitializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj);
    
    Absolute::deregisterNode(pluginFn);
    AcosAngle::deregisterNode(pluginFn);
    Add::deregisterNode(pluginFn);
    AddAngle::deregisterNode(pluginFn);
    AddInt::deregisterNode(pluginFn);
    AddVector::deregisterNode(pluginFn);
    AngleBetweenVectors::deregisterNode(pluginFn);
    AsinAngle::deregisterNode(pluginFn);
    AtanAngle::deregisterNode(pluginFn);
    Clamp::deregisterNode(pluginFn);
    ClampAngle::deregisterNode(pluginFn);
    ClampInt::deregisterNode(pluginFn);
    Cos::deregisterNode(pluginFn);
    CrossProduct::deregisterNode(pluginFn);
    Divide::deregisterNode(pluginFn);
    DivideAngle::deregisterNode(pluginFn);
    DivideAngleByInt::deregisterNode(pluginFn);
    DivideByInt::deregisterNode(pluginFn);
    DotProduct::deregisterNode(pluginFn);
    InverseMatrix::deregisterNode(pluginFn);
    InverseQuaternion::deregisterNode(pluginFn);
    Lerp::deregisterNode(pluginFn);
    LerpAngle::deregisterNode(pluginFn);
    LerpVector::deregisterNode(pluginFn);
    Max::deregisterNode(pluginFn);
    Min::deregisterNode(pluginFn);
    MaxInt::deregisterNode(pluginFn);
    MinInt::deregisterNode(pluginFn);
    MaxAngle::deregisterNode(pluginFn);
    MinAngle::deregisterNode(pluginFn);
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
    RotationFromMatrix::deregisterNode(pluginFn);
    RotationFromQuaternion::deregisterNode(pluginFn);
    Sin::deregisterNode(pluginFn);
    Subtract::deregisterNode(pluginFn);
    SubtractAngle::deregisterNode(pluginFn);
    SubtractInt::deregisterNode(pluginFn);
    SubtractVector::deregisterNode(pluginFn);
    SquareRoot::deregisterNode(pluginFn);
    Tan::deregisterNode(pluginFn);
    VectorLength::deregisterNode(pluginFn);
    
    return MS::kSuccess;
}
