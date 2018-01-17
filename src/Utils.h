// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <cmath>

#include <maya/MAngle.h>
#include <maya/MMatrix.h>
#include <maya/MFnMatrixAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPxNode.h>
#include <maya/MVector.h>
#include <maya/MQuaternion.h>

#ifdef _MSC_VER
    #define TEMPLATE_PARAMETER_LINKAGE extern
#else
    #define TEMPLATE_PARAMETER_LINKAGE constexpr
#endif

// Default value templates
// Note that complex types are always defaulted to zero/identity
template <typename TType>
inline TType DefaultValue(double from)
{
    return TType(from);
}

template <>
inline MVector DefaultValue(double)
{
    return MVector::zero;
}

template <>
inline MMatrix DefaultValue(double)
{
    return MMatrix::identity;
}

template <>
inline MQuaternion DefaultValue(double)
{
    return MQuaternion::identity;
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

inline void createAttribute(MObject& attr, const char* name, const MMatrix& value, bool isKeyable = true)
{
    MFnMatrixAttribute attrFn;
    attr = attrFn.create(name, name);
    attrFn.setDefault(value);
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

template <>
inline MMatrix getAttribute(const MDataHandle& handle)
{
    return handle.asMatrix();
}

template <typename TInputType, typename TOutputType>
inline TOutputType getAttribute(const MDataHandle& handle);

template <>
inline double getAttribute<MAngle, double>(const MDataHandle& handle)
{
    return handle.asAngle().asRadians();
}

template <>
inline double getAttribute<double, double>(const MDataHandle& handle)
{
    return handle.asDouble();
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

MAngle operator*(const MAngle& a, double b)
{
    return MAngle(a.asRadians() * b);
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
