// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TAttrType, typename TClass, const char* TTypeName>
class LerpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAAttr_, "inputA", DefaultValue<TAttrType>(0.0));
        createAttribute(inputBAttr_, "inputB", DefaultValue<TAttrType>(0.0));
        createAttribute(alphaAttr_, "alpha", 0.5);
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(0.0), false);
        
        MFnNumericAttribute attrFn(alphaAttr_);
        attrFn.setMin(0.0);
        attrFn.setMax(1.0);
        
        MPxNode::addAttribute(inputAAttr_);
        MPxNode::addAttribute(inputBAttr_);
        MPxNode::addAttribute(alphaAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAAttr_, outputAttr_);
        MPxNode::attributeAffects(inputBAttr_, outputAttr_);
        MPxNode::attributeAffects(alphaAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputAValue = getAttribute<TAttrType>(dataBlock, inputAAttr_);
            const auto inputBValue = getAttribute<TAttrType>(dataBlock, inputBAttr_);
            const auto alphaValue = getAttribute<double>(dataBlock, alphaAttr_);
            
            setAttribute(dataBlock, outputAttr_, TAttrType(inputAValue + (inputBValue - inputAValue) * alphaValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAAttr_;
    static Attribute inputBAttr_;
    static Attribute alphaAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute LerpNode<TAttrType, TClass, TTypeName>::inputAAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute LerpNode<TAttrType, TClass, TTypeName>::inputBAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute LerpNode<TAttrType, TClass, TTypeName>::alphaAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute LerpNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define LERP_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public LerpNode<AttrType, NodeName, name##NodeName> {};

LERP_NODE(double, Lerp);
LERP_NODE(MAngle, LerpAngle);
LERP_NODE(MVector, LerpVector);
