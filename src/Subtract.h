// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TInOutAttrType, typename TInAttrType, typename TClass, const char* TTypeName>
class SubtractNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<TInOutAttrType>());
        createAttribute(input2Attr_, "input2", DefaultValue<TInAttrType>());
        createAttribute(outputAttr_, "output", DefaultValue<TInOutAttrType>(), false);
        
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
            const TInOutAttrType input1Value = getAttribute<TInOutAttrType>(dataBlock, input1Attr_);
            const TInAttrType input2Value = getAttribute<TInAttrType>(dataBlock, input2Attr_);
            
            setAttribute(dataBlock, outputAttr_, TInOutAttrType(input1Value - input2Value));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }
    
    MPlug passThroughToOne(const MPlug& plug) const override
    {
        if (plug == input1Attr_)
        {
            return MPlug(this->thisMObject(), outputAttr_);
        }
        
        return MPlug();
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute outputAttr_;
};

template<typename TInOutAttrType, typename TInAttrType, typename TClass, const char* TTypeName>
Attribute SubtractNode<TInOutAttrType, TInAttrType, TClass, TTypeName>::input1Attr_;

template<typename TInOutAttrType, typename TInAttrType, typename TClass, const char* TTypeName>
Attribute SubtractNode<TInOutAttrType, TInAttrType, TClass, TTypeName>::input2Attr_;

template<typename TInOutAttrType, typename TInAttrType, typename TClass, const char* TTypeName>
Attribute SubtractNode<TInOutAttrType, TInAttrType, TClass, TTypeName>::outputAttr_;

#define SUBTRACT_NODE(InOutAttrType, InAttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public SubtractNode<InOutAttrType, InAttrType, NodeName, name##NodeName> {};

SUBTRACT_NODE(double, double, Subtract);
SUBTRACT_NODE(int, int, SubtractInt);
SUBTRACT_NODE(MAngle, MAngle, SubtractAngle);
SUBTRACT_NODE(MVector, MVector, SubtractVector);
