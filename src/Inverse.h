// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TAttrType, typename TClass, const char* TTypeName>
class InverseNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TAttrType>(0.0));
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(0.0), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            MDataHandle inputHandle = dataBlock.inputValue(inputAttr_);
            const TAttrType inputValue = getAttribute<TAttrType>(inputHandle);
            
            MDataHandle outputHandle = dataBlock.outputValue(outputAttr_);
            outputHandle.set(inputValue.inverse());
            outputHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }
    
    void postConstructor() override
    {
        if (std::is_same<TAttrType, MVector>::value || std::is_same<TAttrType, MQuaternion>::value)
        {
            setAttributeAlias(MPxNode::thisMObject(), inputAttr_);
            setAttributeAlias(MPxNode::thisMObject(), outputAttr_);
        }
    }

private:
    static MObject inputAttr_;
    static MObject outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject InverseNode<TAttrType, TClass, TTypeName>::inputAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject InverseNode<TAttrType, TClass, TTypeName>::outputAttr_; // NOLINT

#define INVERSE_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public InverseNode<AttrType, NodeName, name##NodeName> {}; // NOLINT

INVERSE_NODE(MMatrix, InverseMatrix);
INVERSE_NODE(MQuaternion, InverseQuaternion);
