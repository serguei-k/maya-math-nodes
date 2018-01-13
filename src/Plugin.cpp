// Copyright 2018 Serguei Kalentchouk, et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#include <maya/MFnPlugin.h>

#include "Angle.h"
#include "Trig.h"

MStatus
__unused initializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj, "Serguei Kalentchouk, et al.", "0.0.1", "Any");
    
    pluginFn.registerNode(AddAngle::kTypeName, AddAngle::kTypeId, AddAngle::create, AddAngle::initialize);
    pluginFn.registerNode(DivideAngle::kTypeName, DivideAngle::kTypeId, DivideAngle::create, DivideAngle::initialize);
    pluginFn.registerNode(MultiplyAngle::kTypeName, MultiplyAngle::kTypeId, MultiplyAngle::create, MultiplyAngle::initialize);
    pluginFn.registerNode(SubtractAngle::kTypeName, SubtractAngle::kTypeId, SubtractAngle::create, SubtractAngle::initialize);
    
    pluginFn.registerNode(Cos::kTypeName, Cos::kTypeId, Acos::create, Acos::initialize);
    pluginFn.registerNode(Sin::kTypeName, Sin::kTypeId, Sin::create, Sin::initialize);
    pluginFn.registerNode(Tan::kTypeName, Tan::kTypeId, Tan::create, Tan::initialize);
    pluginFn.registerNode(Acos::kTypeName, Acos::kTypeId, Acos::create, Acos::initialize);
    pluginFn.registerNode(Asin::kTypeName, Asin::kTypeId, Asin::create, Asin::initialize);
    pluginFn.registerNode(Atan::kTypeName, Atan::kTypeId, Atan::create, Atan::initialize);
    
    return MS::kSuccess;
}

MStatus
__unused uninitializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj);
    
    pluginFn.deregisterNode(AddAngle::kTypeId);
    pluginFn.deregisterNode(DivideAngle::kTypeId);
    pluginFn.deregisterNode(MultiplyAngle::kTypeId);
    pluginFn.deregisterNode(SubtractAngle::kTypeId);
    
    pluginFn.deregisterNode(Cos::kTypeId);
    pluginFn.deregisterNode(Sin::kTypeId);
    pluginFn.deregisterNode(Tan::kTypeId);
    pluginFn.deregisterNode(Acos::kTypeId);
    pluginFn.deregisterNode(Asin::kTypeId);
    pluginFn.deregisterNode(Atan::kTypeId);
    
    return MS::kSuccess;
}
