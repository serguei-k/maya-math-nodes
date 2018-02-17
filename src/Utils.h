// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <cmath>
#include <string>
#include <type_traits>

#include <maya/MAngle.h>
#include <maya/MEulerRotation.h>
#include <maya/MMatrix.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFnDependencyNode.h>
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

struct Attribute
{
    MObject attr;
    MObject attrX;
    MObject attrY;
    MObject attrZ;
    MObject attrW;
    
    operator const MObject&() const { return attr; }
    
    Attribute& operator=(const MObject& object)
    {
        this->attr = object;
        return *this;
    }
};

// Default value templates
// Note that complex types are always defaulted to zero/identity
template <typename TType>
inline TType DefaultValue(double from)
{
    return TType(from);
}

template <>
inline MVector DefaultValue(double from)
{
    return MVector(from, from, from);
}

template <>
inline MMatrix DefaultValue(double)
{
    return MMatrix::identity;
}

template <>
inline MEulerRotation DefaultValue(double)
{
    return MEulerRotation::identity;
}

template <>
inline MQuaternion DefaultValue(double)
{
    return MQuaternion::identity;
}

// Overloads for createAttribute
inline void createAttribute(Attribute& attr, const char* name, double value, bool isInput = true)
{
    MFnNumericAttribute attrFn;
    attr.attr = attrFn.create(name, name, MFnNumericData::kDouble, value);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
}

inline void createAttribute(Attribute& attr, const char* name, int value, bool isInput = true)
{
    MFnNumericAttribute attrFn;
    attr.attr = attrFn.create(name, name, MFnNumericData::kInt, value);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
}

inline void createAttribute(Attribute& attr, const char* name, const MAngle& value, bool isInput = true)
{
    MFnUnitAttribute attrFn;
    attr.attr = attrFn.create(name, name, value);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
}

inline void createAttribute(Attribute& attr, const char* name, const MVector& value, bool isInput = true)
{
    MFnNumericAttribute attrFn;
    
    const std::string attrXName = (std::string(name) + "X");
    attr.attrX = attrFn.create(attrXName.c_str(), attrXName.c_str(), MFnNumericData::kDouble, value.x);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
    
    const std::string attrYName = (std::string(name) + "Y");
    attr.attrY = attrFn.create(attrYName.c_str(), attrYName.c_str(), MFnNumericData::kDouble, value.y);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
    
    const std::string attrZName = (std::string(name) + "Z");
    attr.attrZ = attrFn.create(attrZName.c_str(), attrZName.c_str(), MFnNumericData::kDouble, value.z);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
    
    attr.attr = attrFn.create(name, name, attr.attrX, attr.attrY, attr.attrZ);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
}

inline void createAttribute(Attribute& attr, const char* name, const MMatrix& value, bool isInput = true)
{
    MFnMatrixAttribute attrFn;
    attr.attr = attrFn.create(name, name);
    attrFn.setDefault(value);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
}

inline void createAttribute(Attribute& attr, const char* name, const MQuaternion& value, bool isInput = true)
{
    MFnCompoundAttribute cAttrFn;
    MFnNumericAttribute attrFn;
    
    const std::string attrXName = (std::string(name) + "X");
    attr.attrX = attrFn.create(attrXName.c_str(), attrXName.c_str(), MFnNumericData::kDouble, value.x);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
    
    const std::string attrYName = (std::string(name) + "Y");
    attr.attrY = attrFn.create(attrYName.c_str(), attrYName.c_str(), MFnNumericData::kDouble, value.y);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
    
    const std::string attrZName = (std::string(name) + "Z");
    attr.attrZ = attrFn.create(attrZName.c_str(), attrZName.c_str(), MFnNumericData::kDouble, value.z);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
    
    const std::string attrWName = (std::string(name) + "W");
    attr.attrW = attrFn.create(attrWName.c_str(), attrWName.c_str(), MFnNumericData::kDouble, value.w);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
    
    attr.attr = cAttrFn.create(name, name);
    cAttrFn.addChild(attr.attrX);
    cAttrFn.addChild(attr.attrY);
    cAttrFn.addChild(attr.attrZ);
    cAttrFn.addChild(attr.attrW);
    cAttrFn.setKeyable(isInput);
    cAttrFn.setStorable(isInput);
}

inline void createAttribute(Attribute& attr, const char* name, const MEulerRotation& value, bool isInput = true)
{
    MFnNumericAttribute attrFn;
    MFnUnitAttribute uAttrFn;
    
    const std::string attrXName = (std::string(name) + "X");
    attr.attrX = uAttrFn.create(attrXName.c_str(), attrXName.c_str(), MAngle(value.x));
    uAttrFn.setKeyable(isInput);
    uAttrFn.setStorable(isInput);
    
    const std::string attrYName = (std::string(name) + "Y");
    attr.attrY = uAttrFn.create(attrYName.c_str(), attrYName.c_str(), MAngle(value.y));
    uAttrFn.setKeyable(isInput);
    uAttrFn.setStorable(isInput);
    
    const std::string attrZName = (std::string(name) + "Z");
    attr.attrZ = uAttrFn.create(attrZName.c_str(), attrZName.c_str(), MAngle(value.z));
    uAttrFn.setKeyable(isInput);
    uAttrFn.setStorable(isInput);
    
    attr.attr = attrFn.create(name, name, attr.attrX, attr.attrY, attr.attrZ);
    attrFn.setKeyable(isInput);
    attrFn.setStorable(isInput);
}

// Explicit specializations for getAttribute
template <typename TType>
inline TType getAttribute(MDataBlock& dataBlock, const Attribute& attribute);

template <>
inline double getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
    MDataHandle handle = dataBlock.inputValue(attribute);
    return handle.asDouble();
}

template <>
inline int getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
    MDataHandle handle = dataBlock.inputValue(attribute);
    return handle.asInt();
}

template <>
inline MAngle getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
    MDataHandle handle = dataBlock.inputValue(attribute);
    return handle.asAngle();
}

template <>
inline MVector getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
    MDataHandle handle = dataBlock.inputValue(attribute);
    return handle.asVector();
}

template <>
inline MMatrix getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
    MDataHandle handle = dataBlock.inputValue(attribute);
    return handle.asMatrix();
}

template <>
inline MEulerRotation getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
    MDataHandle handle = dataBlock.inputValue(attribute.attr);
    
    const double x = handle.child(attribute.attrX).asAngle().asRadians();
    const double y = handle.child(attribute.attrY).asAngle().asRadians();
    const double z = handle.child(attribute.attrZ).asAngle().asRadians();
    
    return MEulerRotation(x, y, z);
}

template <>
inline MQuaternion getAttribute(MDataBlock& dataBlock, const Attribute& attribute)
{
    MDataHandle handle = dataBlock.inputValue(attribute.attr);
    
    const double x = handle.child(attribute.attrX).asDouble();
    const double y = handle.child(attribute.attrY).asDouble();
    const double z = handle.child(attribute.attrZ).asDouble();
    const double w = handle.child(attribute.attrW).asDouble();
    
    return MQuaternion(x, y, z, w);
}

template <typename TInputType, typename TOutputType>
inline TOutputType getAttribute(MDataBlock& dataBlock, const Attribute& attribute);

template <>
inline double getAttribute<MAngle, double>(MDataBlock& dataBlock, const Attribute& attribute)
{
    MDataHandle handle = dataBlock.inputValue(attribute);
    return handle.asAngle().asRadians();
}

template <>
inline double getAttribute<double, double>(MDataBlock& dataBlock, const Attribute& attribute)
{
    MDataHandle handle = dataBlock.inputValue(attribute);
    return handle.asDouble();
}

template <typename TType>
inline void setAttribute(MDataBlock& dataBlock, const Attribute& attribute, TType value)
{
    MDataHandle handle = dataBlock.outputValue(attribute);
    handle.set(value);
    handle.setClean();
}

template <>
inline void setAttribute(MDataBlock& dataBlock, const Attribute& attribute, MEulerRotation value)
{
    MDataHandle outputXHandle = dataBlock.outputValue(attribute.attrX);
    outputXHandle.set(MAngle(value.x));
    outputXHandle.setClean();
    
    MDataHandle outputYHandle = dataBlock.outputValue(attribute.attrY);
    outputYHandle.set(MAngle(value.y));
    outputYHandle.setClean();
    
    MDataHandle outputZHandle = dataBlock.outputValue(attribute.attrZ);
    outputZHandle.set(MAngle(value.z));
    outputZHandle.setClean();
}

template <>
inline void setAttribute(MDataBlock& dataBlock, const Attribute& attribute, MQuaternion value)
{
    MDataHandle outputXHandle = dataBlock.outputValue(attribute.attrX);
    outputXHandle.set(value.x);
    outputXHandle.setClean();
    
    MDataHandle outputYHandle = dataBlock.outputValue(attribute.attrY);
    outputYHandle.set(value.y);
    outputYHandle.setClean();
    
    MDataHandle outputZHandle = dataBlock.outputValue(attribute.attrZ);
    outputZHandle.set(value.z);
    outputZHandle.setClean();
    
    MDataHandle outputWHandle = dataBlock.outputValue(attribute.attrW);
    outputWHandle.set(value.w);
    outputWHandle.setClean();
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

MAngle operator*(const MAngle& a, int b)
{
    return MAngle(a.asRadians() * b);
}

MAngle operator/(const MAngle& a, double b)
{
    return MAngle(a.asRadians() / b);
}

MAngle operator/(const MAngle& a, int b)
{
    return MAngle(a.asRadians() / b);
}

// Base node type definition used for all math nodes in this library
template<typename TClass, const char* TTypeName>
class BaseNode : public MPxNode
{
public:
    static void registerNode(class MFnPlugin& pluginFn, int typeId)
    {
        kTypeId = typeId;
        pluginFn.registerNode((std::string(NODE_NAME_PREFIX) + TTypeName).c_str(), typeId, []() -> void* { return new TClass(); }, TClass::initialize);
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
