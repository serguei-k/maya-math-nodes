// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TType>
struct negate_fob
{
    TType operator() (const TType& arg) const { return -arg; }
    
    typedef TType argument_type;
    typedef TType result_type;
};

struct negate_bool_fob
{
    bool operator() (bool arg) const { return !arg; }
    
    typedef bool argument_type;
    typedef bool result_type;
};


template<typename TClass, const char* TTypeName, typename TOperator>
class NegateNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<typename TOperator::argument_type>());
        createAttribute(outputAttr_, "output", DefaultValue<typename TOperator::result_type>(), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            const auto inputValue = getAttribute<typename TOperator::argument_type>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, TOperator()(inputValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }
    
    MPlug passThroughToOne(const MPlug& plug) const override
    {
        if (plug == inputAttr_)
        {
            return MPlug(this->thisMObject(), outputAttr_);
        }
        
        return MPlug();
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};

template<typename TClass, const char* TTypeName, typename TOperator>
Attribute NegateNode<TClass, TTypeName, TOperator>::inputAttr_;

template<typename TClass, const char* TTypeName, typename TOperator>
Attribute NegateNode<TClass, TTypeName, TOperator>::outputAttr_;

#define NEGATE_NODE(NodeName, Operator) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public NegateNode<NodeName, name##NodeName, Operator> {};

NEGATE_NODE(Negate, negate_fob<double>);
NEGATE_NODE(NegateInt, negate_fob<int>);
NEGATE_NODE(NegateAngle, negate_fob<MAngle>);
NEGATE_NODE(NegateVector, negate_fob<MVector>);
NEGATE_NODE(NotBool, negate_bool_fob);
