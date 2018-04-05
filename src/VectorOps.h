// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

class MVectorExt : public MVector
{
public:
    explicit MVectorExt(const MVector& vector) : MVector(vector) {}
    
    double vectorLengthSquared() const
    {
        return x * x + y * y + z * z;
    }
};


template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
class Vector2OpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", MVector::xAxis);
        createAttribute(input2Attr_, "input2", MVector::yAxis);
        createAttribute(outputAttr_, "output", DefaultValue<TOutAttrType>(), false);
        
        MPxNode::addAttribute(input1Attr_);
        MPxNode::addAttribute(input2Attr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(input1Attr_, outputAttr_);
        MPxNode::attributeAffects(input2Attr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto input1Value = getAttribute<MVector>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<MVector>(dataBlock, input2Attr_);
            
            setAttribute(dataBlock, outputAttr_, TOutAttrType((input1Value.*TOpFucPtr)(input2Value)));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute outputAttr_;
};

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute Vector2OpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::input1Attr_;

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute Vector2OpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::input2Attr_;

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute Vector2OpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::outputAttr_;

#define VECTOR2_OP_NODE(OutAttrType, NodeName, OpFuncPtrType, OpFucPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public Vector2OpNode<OutAttrType, NodeName, name##NodeName, OpFuncPtrType, OpFucPtr> {};

VECTOR2_OP_NODE(double, DotProduct, double (MVector::*)(const MVector&) const, &MVector::operator*);
VECTOR2_OP_NODE(MAngle, AngleBetweenVectors, double (MVector::*)(const MVector&) const, &MVector::angle);
VECTOR2_OP_NODE(MVector, CrossProduct, MVector (MVector::*)(const MVector&) const, &MVector::operator^);


template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
class VectorOpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1ttr_, "input", MVector::zero);
        createAttribute(outputAttr_, "output", DefaultValue<TOutAttrType>(), false);
        
        MPxNode::addAttribute(input1ttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(input1ttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputValue = static_cast<MVectorExt>(getAttribute<MVector>(dataBlock, input1ttr_));
            
            setAttribute(dataBlock, outputAttr_, TOutAttrType((inputValue.*TOpFucPtr)()));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute input1ttr_;
    static Attribute outputAttr_;
};

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute VectorOpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::input1ttr_;

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute VectorOpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::outputAttr_;

#define VECTOR_OP_NODE(OutAttrType, NodeName, OpFuncPtrType, OpFucPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public VectorOpNode<OutAttrType, NodeName, name##NodeName, OpFuncPtrType, OpFucPtr> {};

VECTOR_OP_NODE(double, VectorLength, double (MVector::*)() const, &MVector::length);
VECTOR_OP_NODE(double, VectorLengthSquared, double (MVectorExt::*)() const, &MVectorExt::vectorLengthSquared);
VECTOR_OP_NODE(MVector, NormalizeVector, MVector (MVector::*)() const, &MVector::normal);
