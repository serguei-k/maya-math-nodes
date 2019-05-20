// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TClass, const char* TTypeName>
class PowerNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", 0.0);
        createAttribute(exponentAttr_, "exponent", 2.0);
        createAttribute(outputAttr_, "output", 0.0, false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(exponentAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        MPxNode::attributeAffects(exponentAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            const auto inputValue = getAttribute<double>(dataBlock, inputAttr_);
            const auto exponentValue = getAttribute<double>(dataBlock, exponentAttr_);
            
            setAttribute(dataBlock, outputAttr_, std::pow(inputValue, exponentValue));
            
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
    static Attribute exponentAttr_;
    static Attribute outputAttr_;
};

template<typename TClass, const char* TTypeName>
Attribute PowerNode<TClass, TTypeName>::inputAttr_;

template<typename TClass, const char* TTypeName>
Attribute PowerNode<TClass, TTypeName>::exponentAttr_;

template<typename TClass, const char* TTypeName>
Attribute PowerNode<TClass, TTypeName>::outputAttr_;

#define POWER_NODE(NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public PowerNode<NodeName, name##NodeName> {};

POWER_NODE(Power);


template<typename TClass, const char* TTypeName>
class SquareRootNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", 0.0);
        createAttribute(outputAttr_, "output", 0.0, false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            const auto inputValue = getAttribute<double>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, std::sqrt(inputValue));
            
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

template<typename TClass, const char* TTypeName>
Attribute SquareRootNode<TClass, TTypeName>::inputAttr_;

template<typename TClass, const char* TTypeName>
Attribute SquareRootNode<TClass, TTypeName>::outputAttr_;

#define SQUARE_ROOT_NODE(NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public SquareRootNode<NodeName, name##NodeName> {};

SQUARE_ROOT_NODE(SquareRoot);
