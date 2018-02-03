// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
class Vector2OpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAAttr_, "inputA", MVector::xAxis);
        createAttribute(inputBAttr_, "inputB", MVector::yAxis);
        createAttribute(outputAttr_, "output", DefaultValue<TOutAttrType>(0.0), false);
        
        MPxNode::addAttribute(inputAAttr_);
        MPxNode::addAttribute(inputBAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAAttr_, outputAttr_);
        MPxNode::attributeAffects(inputBAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const MVector inputAValue = getAttribute<MVector>(dataBlock, inputAAttr_);
            const MVector inputBValue = getAttribute<MVector>(dataBlock, inputBAttr_);
            
            MDataHandle outputHandle = dataBlock.outputValue(outputAttr_);
            outputHandle.set(TOutAttrType((inputAValue.*TOpFucPtr)(inputBValue)));
            outputHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }
    
    void postConstructor() override
    {
        setAttributeAlias(MPxNode::thisMObject(), inputAAttr_);
        setAttributeAlias(MPxNode::thisMObject(), inputBAttr_);
        setAttributeAlias(MPxNode::thisMObject(), outputAttr_);
    }

private:
    static Attribute inputAAttr_;
    static Attribute inputBAttr_;
    static Attribute outputAttr_;
};

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute Vector2OpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::inputAAttr_; // NOLINT

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute Vector2OpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::inputBAttr_; // NOLINT

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute Vector2OpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::outputAttr_; // NOLINT

#define VECTOR2_OP_NODE(OutAttrType, NodeName, OpFuncPtrType, OpFucPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public Vector2OpNode<OutAttrType, NodeName, name##NodeName, OpFuncPtrType, OpFucPtr> {}; // NOLINT

VECTOR2_OP_NODE(double, DotProduct, double (MVector::*)(const MVector&) const, &MVector::operator*);
VECTOR2_OP_NODE(MAngle, AngleBetweenVectors, double (MVector::*)(const MVector&) const, &MVector::angle);
VECTOR2_OP_NODE(MVector, CrossProduct, MVector (MVector::*)(const MVector&) const, &MVector::operator^);


template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
class VectorOpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", MVector::zero);
        createAttribute(outputAttr_, "output", DefaultValue<TOutAttrType>(0.0), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputValue = getAttribute<MVector>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, TOutAttrType((inputValue.*TOpFucPtr)()));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }
    
    void postConstructor() override
    {
        setAttributeAlias(MPxNode::thisMObject(), inputAttr_);
        setAttributeAlias(MPxNode::thisMObject(), outputAttr_);
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};


template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute VectorOpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::inputAttr_;

template<typename TOutAttrType, typename TClass, const char* TTypeName, typename TOpFuncType, TOpFuncType TOpFucPtr>
Attribute VectorOpNode<TOutAttrType, TClass, TTypeName, TOpFuncType, TOpFucPtr>::outputAttr_;

#define VECTOR_OP_NODE(OutAttrType, NodeName, OpFuncPtrType, OpFucPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public VectorOpNode<OutAttrType, NodeName, name##NodeName, OpFuncPtrType, OpFucPtr> {};

VECTOR_OP_NODE(double, VectorLength, double (MVector::*)() const, &MVector::length);
VECTOR_OP_NODE(MVector, NormalizeVector, MVector (MVector::*)() const, &MVector::normal);
