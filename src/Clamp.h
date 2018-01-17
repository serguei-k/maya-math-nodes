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
            MDataHandle inputHandle = dataBlock.inputValue(inputAttr_);
            const TAttrType inputValue = getAttribute<TAttrType>(inputHandle);
            
            MDataHandle inputAHandle = dataBlock.inputValue(inputAAttr_);
            const TAttrType inputAValue = getAttribute<TAttrType>(inputAHandle);
            
            MDataHandle inputBHandle = dataBlock.inputValue(inputBAttr_);
            const TAttrType inputBValue = getAttribute<TAttrType>(inputBHandle);
            
            MDataHandle outputHandle = dataBlock.outputValue(outputAttr_);
            outputHandle.set(TAttrType(std::max(inputAValue, std::min(inputValue, inputBValue))));
            outputHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAttr_;
    static MObject inputAAttr_;
    static MObject inputBAttr_;
    static MObject outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject ClampNode<TAttrType, TClass, TTypeName>::inputAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject ClampNode<TAttrType, TClass, TTypeName>::inputAAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject ClampNode<TAttrType, TClass, TTypeName>::inputBAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject ClampNode<TAttrType, TClass, TTypeName>::outputAttr_; // NOLINT

#define CLAMP_NODE(AttrType, NodeName)           \
    constexpr char name##NodeName[] = #NodeName; \
    class NodeName : public ClampNode<AttrType, NodeName, name##NodeName> {}; // NOLINT

CLAMP_NODE(double, Clamp);
CLAMP_NODE(int, ClampInt);
CLAMP_NODE(MAngle, ClampAngle);
