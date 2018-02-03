// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TInOutAttrType, typename TInAttrType, typename TClass, const char* TTypeName>
class MultiplyNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAAttr_, "inputA", DefaultValue<TInOutAttrType>(0.0));
        createAttribute(inputBAttr_, "inputB", DefaultValue<TInAttrType>(0.0));
        createAttribute(outputAttr_, "output", DefaultValue<TInOutAttrType>(0.0), false);
        
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
            const auto inputAValue = getAttribute<TInOutAttrType>(dataBlock, inputAAttr_);
            const auto inputBValue = getAttribute<TInAttrType>(dataBlock, inputBAttr_);
            
            setAttribute(dataBlock, outputAttr_, TInOutAttrType(inputAValue * inputBValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAAttr_;
    static Attribute inputBAttr_;
    static Attribute outputAttr_;
};

template<typename TInOutAttrType, typename TInAttrType, typename TClass, const char* TTypeName>
Attribute MultiplyNode<TInOutAttrType, TInAttrType, TClass, TTypeName>::inputAAttr_;

template<typename TInOutAttrType, typename TInAttrType, typename TClass, const char* TTypeName>
Attribute MultiplyNode<TInOutAttrType, TInAttrType, TClass, TTypeName>::inputBAttr_;

template<typename TInOutAttrType, typename TInAttrType, typename TClass, const char* TTypeName>
Attribute MultiplyNode<TInOutAttrType, TInAttrType, TClass, TTypeName>::outputAttr_;

#define MULTIPLY_NODE(InOutAttrType, InAttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public MultiplyNode<InOutAttrType, InAttrType, NodeName, name##NodeName> {};

MULTIPLY_NODE(double, double, Multiply);
MULTIPLY_NODE(double, int, MultiplyByInt);
MULTIPLY_NODE(int, int, MultiplyInt);
MULTIPLY_NODE(MAngle, double, MultiplyAngle);
MULTIPLY_NODE(MAngle, int, MultiplyAngleByInt);
MULTIPLY_NODE(MMatrix, MMatrix, MultiplyMatrix);
MULTIPLY_NODE(MVector, double, MultiplyVector);
MULTIPLY_NODE(MVector, MMatrix, MultiplyVectorByMatrix);
MULTIPLY_NODE(MQuaternion, MQuaternion, MultiplyQuaternion);
