// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <algorithm>

#include "MinMax.h"
#include "Utils.h"

template<typename TAttrType, typename TClass, const char* TTypeName>
class ClampNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputMinAttr_, "input", DefaultValue<TAttrType>(0.0));
        createAttribute(inputMinAttr_, "inputMin", DefaultValue<TAttrType>(0.0));
        createAttribute(inputMaxAttr_, "inputMax", DefaultValue<TAttrType>(1.0));
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(0.0), false);
        
        MPxNode::addAttribute(inputMinttr_);
        MPxNode::addAttribute(inputMinAttr_);
        MPxNode::addAttribute(inputMaxAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputMinttr_, outputAttr_);
        MPxNode::attributeAffects(inputMinAttr_, outputAttr_);
        MPxNode::attributeAffects(inputMaxAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const TAttrType inputValue = getAttribute<TAttrType>(dataBlock, inputMinttr_);
            const TAttrType inputMinValue = getAttribute<TAttrType>(dataBlock, inputMinAttr_);
            const TAttrType inputMaxValue = getAttribute<TAttrType>(dataBlock, inputMaxAttr_);
            
            setAttribute(dataBlock, outputAttr_, TAttrType(std::max(inputMinValue, std::min(inputValue, inputMaxValue))));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputMinttr_;
    static Attribute inputMinAttr_;
    static Attribute inputMaxAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::inputMinttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::inputMinAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::inputMaxAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define CLAMP_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public ClampNode<AttrType, NodeName, name##NodeName> {};

CLAMP_NODE(double, Clamp);
CLAMP_NODE(int, ClampInt);
CLAMP_NODE(MAngle, ClampAngle);
