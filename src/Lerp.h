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
        createAttribute(inputAAttr_, "inputA", CastTo<TAttrType>(0.0));
        createAttribute(inputBAttr_, "inputB", CastTo<TAttrType>(0.0));
        createAttribute(alphaAttr_, "alpha", 0.5);
        createAttribute(outputAttr_, "output", CastTo<TAttrType>(0.0), false);
        
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
            MDataHandle inputAHandle = dataBlock.inputValue(inputAAttr_);
            const TAttrType inputAValue = getAttribute<TAttrType>(inputAHandle);
            
            MDataHandle inputBHandle = dataBlock.inputValue(inputBAttr_);
            const TAttrType inputBValue = getAttribute<TAttrType>(inputBHandle);
            
            MDataHandle alphaHandle = dataBlock.inputValue(alphaAttr_);
            const double alphaValue = getAttribute<double>(alphaHandle);
            
            MDataHandle outputHandle = dataBlock.outputValue(outputAttr_);
            outputHandle.set(TAttrType(inputAValue + alphaValue * (inputBValue - inputAValue)));
            outputHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAAttr_;
    static MObject inputBAttr_;
    static MObject alphaAttr_;
    static MObject outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject LerpNode<TAttrType, TClass, TTypeName>::inputAAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject LerpNode<TAttrType, TClass, TTypeName>::inputBAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject LerpNode<TAttrType, TClass, TTypeName>::alphaAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject LerpNode<TAttrType, TClass, TTypeName>::outputAttr_; // NOLINT

#define LERP_NODE(AttrType, NodeName)            \
    constexpr char name##NodeName[] = #NodeName; \
    class NodeName : public LerpNode<AttrType, NodeName, name##NodeName> {}; // NOLINT

LERP_NODE(double, Lerp);
LERP_NODE(MAngle, LerpAngle);
LERP_NODE(MVector, LerpVector);
