// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#include <maya/MFnPlugin.h>

#include "Absolute.h"
#include "Clamp.h"
#include "Lerp.h"
#include "Multiply.h"
#include "Trig.h"

MStatus
__unused initializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj, "Serguei Kalentchouk, et al.", "0.0.1", "Any");
    
    //TODO: change to real node IDs
    int typeId = 1000;
    
    Absolute::registerNode(pluginFn, ++typeId);
    AcosAngle::registerNode(pluginFn, ++typeId);
    AsinAngle::registerNode(pluginFn, ++typeId);
    AtanAngle::registerNode(pluginFn, ++typeId);
    Clamp::registerNode(pluginFn, ++typeId);
    ClampAngle::registerNode(pluginFn, ++typeId);
    ClampInt::registerNode(pluginFn, ++typeId);
    Cos::registerNode(pluginFn, ++typeId);
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
    MultiplyByInt::registerNode(pluginFn, ++typeId);
    MultiplyInt::registerNode(pluginFn, ++typeId);
    MultiplyMatrix::registerNode(pluginFn, ++typeId);
    MultiplyVector::registerNode(pluginFn, ++typeId);
    MultiplyVectorByMatrix::registerNode(pluginFn, ++typeId);
    Sin::registerNode(pluginFn, ++typeId);
    Tan::registerNode(pluginFn, ++typeId);
    
    return MS::kSuccess;
}

MStatus
__unused uninitializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj);
    
    Absolute::deregisterNode(pluginFn);
    AcosAngle::deregisterNode(pluginFn);
    AsinAngle::deregisterNode(pluginFn);
    AtanAngle::deregisterNode(pluginFn);
    Clamp::deregisterNode(pluginFn);
    ClampAngle::deregisterNode(pluginFn);
    ClampInt::deregisterNode(pluginFn);
    Cos::deregisterNode(pluginFn);
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
    MultiplyByInt::deregisterNode(pluginFn);
    MultiplyInt::deregisterNode(pluginFn);
    MultiplyMatrix::deregisterNode(pluginFn);
    MultiplyVector::deregisterNode(pluginFn);
    MultiplyVectorByMatrix::deregisterNode(pluginFn);
    Sin::deregisterNode(pluginFn);
    Tan::deregisterNode(pluginFn);
    
    return MS::kSuccess;
}
