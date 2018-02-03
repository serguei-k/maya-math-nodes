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
        createAttribute(inputAAttr_, "input", DefaultValue<TAttrType>(0.0));
        createAttribute(inputAAttr_, "inputA", DefaultValue<TAttrType>(0.0));
        createAttribute(inputBAttr_, "inputB", DefaultValue<TAttrType>(1.0));
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(0.0), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(inputAAttr_);
        MPxNode::addAttribute(inputBAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        MPxNode::attributeAffects(inputAAttr_, outputAttr_);
        MPxNode::attributeAffects(inputBAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const TAttrType inputValue = getAttribute<TAttrType>(dataBlock, inputAttr_);
            const TAttrType inputAValue = getAttribute<TAttrType>(dataBlock, inputAAttr_);
            const TAttrType inputBValue = getAttribute<TAttrType>(dataBlock, inputBAttr_);
            
            setAttribute(dataBlock, outputAttr_, TAttrType(std::max(inputAValue, std::min(inputValue, inputBValue))));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute inputAAttr_;
    static Attribute inputBAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::inputAAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::inputBAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define CLAMP_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public ClampNode<AttrType, NodeName, name##NodeName> {};

CLAMP_NODE(double, Clamp);
CLAMP_NODE(int, ClampInt);
CLAMP_NODE(MAngle, ClampAngle);
