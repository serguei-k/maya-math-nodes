// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <maya/MGlobal.h>

#include "Utils.h"

typedef double (*TrigFuncPtr)(double);

template<typename TInputAttrType, typename TOutputAttrType, typename TClass,
         const char* TTypeName, bool TSetLimits, TrigFuncPtr TTrigFuncPtr>
class TrigNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TInputAttrType>(0.0));
        createAttribute(outputAttr_, "output", DefaultValue<TOutputAttrType>(0.0), false);
        
        if (TSetLimits)
        {
            MFnNumericAttribute attrFn(inputAttr_);
            attrFn.setMin(-1.0);
            attrFn.setMax(1.0);
        }
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            const auto inputValue = getAttribute<TInputAttrType, double>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, TOutputAttrType(TTrigFuncPtr(inputValue)));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};

template<typename TInputAttrType, typename TOutputAttrType, typename TClass,
         const char* TTypeName, bool TSetLimits, TrigFuncPtr TTrigFuncPtr>
Attribute TrigNode<TInputAttrType, TOutputAttrType, TClass, TTypeName, TSetLimits, TTrigFuncPtr>::inputAttr_;

template<typename TInputAttrType, typename TOutputAttrType, typename TClass,
         const char* TTypeName, bool TSetLimits, TrigFuncPtr TTrigFuncPtr>
Attribute TrigNode<TInputAttrType, TOutputAttrType, TClass, TTypeName, TSetLimits, TTrigFuncPtr>::outputAttr_;

#define TRIG_NODE(InputAttrType, OutputAttrType, NodeName, SetLimits, TrigFuncPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public TrigNode<InputAttrType, OutputAttrType, NodeName, name##NodeName, SetLimits, TrigFuncPtr> {};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "TemplateArgumentsIssues"
TRIG_NODE(double, MAngle, AcosAngle, true, &std::acos);
TRIG_NODE(double, MAngle, AsinAngle, true, &std::asin);
TRIG_NODE(double, MAngle, AtanAngle, false, &std::atan);
TRIG_NODE(MAngle, double, Cos, false, &std::cos);
TRIG_NODE(MAngle, double, Sin, false, &std::sin);
TRIG_NODE(MAngle, double, Tan, false, &std::tan);
#pragma clang diagnostic pop


template<typename TClass, const char* TTypeName>
class Atan2TrigNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAAttr_, "inputA", DefaultValue<double >(0.0));
        createAttribute(inputBAttr_, "inputB", DefaultValue<double >(1.0));
        createAttribute(outputAttr_, "output", DefaultValue<MAngle>(0.0), false);
        
        MPxNode::addAttribute(inputAAttr_);
        MPxNode::addAttribute(inputBAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAAttr_, outputAttr_);
        MPxNode::attributeAffects(inputBAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            const auto inputAValue = getAttribute<double>(dataBlock, inputAAttr_);
            const auto inputBValue = getAttribute<double>(dataBlock, inputBAttr_);
            
            if (inputAValue == 0 && inputBValue == 0)
            {
                MGlobal::displayError("Both inputs to atan2 function cannot be 0!");
                return MS::kFailure;
            }
            
            setAttribute(dataBlock, outputAttr_, MAngle(std::atan2(inputAValue, inputBValue)));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAAttr_;
    static Attribute inputBAttr_;
    static Attribute outputAttr_;
};

template<typename TClass, const char* TTypeName>
Attribute Atan2TrigNode<TClass, TTypeName>::inputAAttr_;

template<typename TClass, const char* TTypeName>
Attribute Atan2TrigNode<TClass, TTypeName>::inputBAttr_;

template<typename TClass, const char* TTypeName>
Attribute Atan2TrigNode<TClass, TTypeName>::outputAttr_;

#define ATAN2_TRIG_NODE(NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public Atan2TrigNode<NodeName, name##NodeName> {};

ATAN2_TRIG_NODE(Atan2Angle);
