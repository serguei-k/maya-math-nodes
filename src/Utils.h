#pragma once

#include <cmath>

#include <maya/MAngle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPxNode.h>
#include <maya/MVector.h>

template <typename TType>
inline TType CastTo(double from)
{
    return TType(from);
}

template <>
inline MVector CastTo(double from)
{
    return MVector(from, from, from);
}

// Overloads for createAttribute
inline void createAttribute(MObject& attr, const char* name, double value, bool isKeyable = true)
{
    MFnNumericAttribute numericFn;
    attr = numericFn.create(name, name, MFnNumericData::kDouble, value);
    numericFn.setKeyable(isKeyable);
}

inline void createAttribute(MObject& attr, const char* name, int value, bool isKeyable = true)
{
    MFnNumericAttribute attrFn;
    attr = attrFn.create(name, name, MFnNumericData::kInt, value);
    attrFn.setKeyable(isKeyable);
}

inline void createAttribute(MObject& attr, const char* name, const MAngle& value, bool isKeyable = true)
{
    MFnUnitAttribute attrFn;
    attr = attrFn.create(name, name, value);
    attrFn.setKeyable(isKeyable);
}

inline void createAttribute(MObject& attr, const char* name, const MVector& value, bool isKeyable = true)
{
    MFnNumericAttribute attrFn;
    attr = attrFn.create(name, name, MFnNumericData::k3Double);
    attrFn.setDefault(value.x, value.y, value.z);
    attrFn.setKeyable(isKeyable);
}

// Explicit specializations for getAttribute
template <typename TType>
inline TType getAttribute(const MDataHandle& handle);

template <>
inline double getAttribute(const MDataHandle& handle)
{
    return handle.asDouble();
}

template <>
inline int getAttribute(const MDataHandle& handle)
{
    return handle.asInt();
}

template <>
inline MAngle getAttribute(const MDataHandle& handle)
{
    return handle.asAngle();
}

template <>
inline MVector getAttribute(const MDataHandle& handle)
{
    return handle.asVector();
}

// MAngle operator overloads
MAngle operator+(const MAngle& a, const MAngle& b)
{
    return MAngle(a.asRadians() + b.asRadians());
}

MAngle operator-(const MAngle& a, const MAngle& b)
{
    return MAngle(a.asRadians() - b.asRadians());
}

MAngle operator*(double a, const MAngle& b)
{
    return MAngle(a * b.asRadians());
}

// Base node type definition used for all math nodes in this library
template<typename TClass, const char* TTypeName>
class BaseNode : public MPxNode
{
public:
    static void registerNode(class MFnPlugin& pluginFn, int typeId)
    {
        kTypeId = typeId;
        pluginFn.registerNode(TTypeName, typeId, []() -> void* { return new TClass(); }, TClass::initialize);
    }
    
    static void deregisterNode(class MFnPlugin& pluginFn)
    {
        pluginFn.deregisterNode(kTypeId);
    }

protected:
    static int kTypeId;
};

template<typename TClass, const char* TTypeName>
int BaseNode<TClass, TTypeName>::kTypeId = -1;
