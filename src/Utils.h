#pragma once

#include <cmath>

#include <maya/MAngle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPxNode.h>

inline void createAttribute(MObject& attr, const char* name, double value, bool isInput = true)
{
    MFnNumericAttribute numericFn;
    attr = numericFn.create(name, name, MFnNumericData::kDouble, value);
    numericFn.setKeyable(isInput);
}

inline void createAttribute(MObject& attr, const char* name, int value, bool isInput = true)
{
    MFnNumericAttribute attrFn;
    attr = attrFn.create(name, name, MFnNumericData::kInt, value);
    attrFn.setKeyable(isInput);
}

inline void createAttribute(MObject& attr, const char* name, const MAngle& value, bool isInput = true)
{
    MFnUnitAttribute attrFn;
    attr = attrFn.create(name, name, value);
    attrFn.setKeyable(isInput);
}

template <typename TType>
inline const TType getAttribute(const MDataHandle& handle);

template <>
inline const double getAttribute(const MDataHandle& handle)
{
    return handle.asDouble();
}

template <>
inline const int getAttribute(const MDataHandle& handle)
{
    return handle.asInt();
}

template <>
inline const MAngle getAttribute(const MDataHandle& handle)
{
    return handle.asAngle();
}


template<typename TClass, const char* TTypeName, int TTypeId>
class BaseNode : public MPxNode
{
public:
    static void registerNode(class MFnPlugin& pluginFn)
    {
        pluginFn.registerNode(TTypeName, TTypeId, []() -> void* { return new TClass(); }, TClass::initialize);
    }
    
    static void deregisterNode(class MFnPlugin& pluginFn)
    {
        pluginFn.deregisterNode(TTypeId);
    }
};
