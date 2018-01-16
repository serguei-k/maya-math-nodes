// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#include <maya/MFnPlugin.h>

#include "Absolute.h"
#include "Lerp.h"
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
    Cos::registerNode(pluginFn, ++typeId);
    Lerp::registerNode(pluginFn, ++typeId);
    LerpAngle::registerNode(pluginFn, ++typeId);
    LerpVector::registerNode(pluginFn, ++typeId);
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
    Cos::deregisterNode(pluginFn);
    Lerp::deregisterNode(pluginFn);
    LerpAngle::deregisterNode(pluginFn);
    LerpVector::deregisterNode(pluginFn);
    Sin::deregisterNode(pluginFn);
    Tan::deregisterNode(pluginFn);
    
    return MS::kSuccess;
}
