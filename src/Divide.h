// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <__functional_base>

#include "Utils.h"

template<typename TPrimary, typename TSecondary>
struct divides_fob : std::binary_function<TPrimary, TSecondary, TPrimary>
{
    TPrimary operator() (const TPrimary& arg1, const TSecondary& arg2) const { return arg1 / arg2; }
};

template<typename TPrimary, typename TSecondary>
struct modulus_fob : std::binary_function<TPrimary, TSecondary, TPrimary>
{
    TPrimary operator() (const TPrimary& arg1, const TSecondary& arg2) const { return arg1 % arg2; }
};


template<typename TClass, const char* TTypeName, typename TOperator>
class DivideNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<typename TOperator::first_argument_type>(0.0));
        createAttribute(input2Attr_, "input2", DefaultValue<typename TOperator::second_argument_type>(0.0));
        createAttribute(outputAttr_, "output", DefaultValue<typename TOperator::result_type>(0.0), false);
        
        MPxNode::addAttribute(input1Attr_);
        MPxNode::addAttribute(input2Attr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(input1Attr_, outputAttr_);
        MPxNode::attributeAffects(input2Attr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            const auto input1Value = getAttribute<typename TOperator::first_argument_type>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<typename TOperator::second_argument_type>(dataBlock, input2Attr_);
            
            setAttribute(dataBlock, outputAttr_, TOperator()(input1Value, input2Value));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute outputAttr_;
};

template<typename TClass, const char* TTypeName, typename TOperator>
Attribute DivideNode<TClass, TTypeName, TOperator>::input1Attr_;

template<typename TClass, const char* TTypeName, typename TOperator>
Attribute DivideNode<TClass, TTypeName, TOperator>::input2Attr_;

template<typename TClass, const char* TTypeName, typename TOperator>
Attribute DivideNode<TClass, TTypeName, TOperator>::outputAttr_;

#define COMMA ,
#define DIVIDE_NODE(NodeName, Operator) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public DivideNode<NodeName, name##NodeName, Operator> {};

DIVIDE_NODE(Divide, divides_fob<double COMMA double>);
DIVIDE_NODE(DivideByInt, divides_fob<double COMMA int> );
DIVIDE_NODE(DivideAngle, divides_fob<MAngle COMMA double>);
DIVIDE_NODE(DivideAngleByInt, divides_fob<MAngle COMMA int>);
DIVIDE_NODE(ModulusInt, modulus_fob<int COMMA int>);
