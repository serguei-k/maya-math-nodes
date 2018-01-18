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
        createAttribute(inputAttr_, "input", DefaultValue<double>(0.0));
        createAttribute(exponentAttr_, "exponent", DefaultValue<double>(2.0));
        createAttribute(outputAttr_, "output", DefaultValue<double>(0.0), false);
        
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
            MDataHandle inputHandle = dataBlock.inputValue(inputAttr_);
            const double inputValue = getAttribute<double>(inputHandle);
    
            MDataHandle exponentHandle = dataBlock.inputValue(exponentAttr_);
            const double exponentValue = getAttribute<double>(exponentHandle);
            
            MDataHandle outputHandle = dataBlock.outputValue(outputAttr_);
            outputHandle.set(std::pow(inputValue, exponentValue));
            outputHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAttr_;
    static MObject exponentAttr_;
    static MObject outputAttr_;
};

template<typename TClass, const char* TTypeName>
MObject PowerNode<TClass, TTypeName>::inputAttr_; // NOLINT

template<typename TClass, const char* TTypeName>
MObject PowerNode<TClass, TTypeName>::exponentAttr_; // NOLINT

template<typename TClass, const char* TTypeName>
MObject PowerNode<TClass, TTypeName>::outputAttr_; // NOLINT

#define POWER_NODE(NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public PowerNode<NodeName, name##NodeName> {}; // NOLINT

POWER_NODE(Power);


template<typename TClass, const char* TTypeName>
class SquareRootNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<double>(0.0));
        createAttribute(outputAttr_, "output", DefaultValue<double>(0.0), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            MDataHandle inputHandle = dataBlock.inputValue(inputAttr_);
            const double inputValue = getAttribute<double>(inputHandle);
            
            MDataHandle outputHandle = dataBlock.outputValue(outputAttr_);
            outputHandle.set(std::sqrt(inputValue));
            outputHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAttr_;
    static MObject outputAttr_;
};

template<typename TClass, const char* TTypeName>
MObject SquareRootNode<TClass, TTypeName>::inputAttr_; // NOLINT

template<typename TClass, const char* TTypeName>
MObject SquareRootNode<TClass, TTypeName>::outputAttr_; // NOLINT

#define SQUARE_ROOT_NODE(NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public SquareRootNode<NodeName, name##NodeName> {}; // NOLINT

SQUARE_ROOT_NODE(SquareRoot);
