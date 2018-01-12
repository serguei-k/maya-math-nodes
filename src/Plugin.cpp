#include <maya/MFnPlugin.h>

#include "Trig.h"

MStatus
__unused initializePlugin(MObject pluginObj)
{
    MFnPlugin pluginFn(pluginObj, "Serguei Kalentchouk, et al.", "0.0.1", "Any");
    
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
    
    pluginFn.deregisterNode(Cos::kTypeId);
    pluginFn.deregisterNode(Sin::kTypeId);
    pluginFn.deregisterNode(Tan::kTypeId);
    pluginFn.deregisterNode(Acos::kTypeId);
    pluginFn.deregisterNode(Asin::kTypeId);
    pluginFn.deregisterNode(Atan::kTypeId);
    
    return MS::kSuccess;
}
