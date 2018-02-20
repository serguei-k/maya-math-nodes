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
        createAttribute(input1ttr_, "input", DefaultValue<TInputAttrType>(0.0));
        createAttribute(outputAttr_, "output", DefaultValue<TOutputAttrType>(0.0), false);
        
        if (TSetLimits)
        {
            MFnNumericAttribute attrFn(input1ttr_);
            attrFn.setMin(-1.0);
            attrFn.setMax(1.0);
        }
        
        MPxNode::addAttribute(input1ttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(input1ttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            const auto inputValue = getAttribute<TInputAttrType, double>(dataBlock, input1ttr_);
            
            setAttribute(dataBlock, outputAttr_, TOutputAttrType(TTrigFuncPtr(inputValue)));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute input1ttr_;
    static Attribute outputAttr_;
};

template<typename TInputAttrType, typename TOutputAttrType, typename TClass,
         const char* TTypeName, bool TSetLimits, TrigFuncPtr TTrigFuncPtr>
Attribute TrigNode<TInputAttrType, TOutputAttrType, TClass, TTypeName, TSetLimits, TTrigFuncPtr>::input1ttr_;

template<typename TInputAttrType, typename TOutputAttrType, typename TClass,
         const char* TTypeName, bool TSetLimits, TrigFuncPtr TTrigFuncPtr>
Attribute TrigNode<TInputAttrType, TOutputAttrType, TClass, TTypeName, TSetLimits, TTrigFuncPtr>::outputAttr_;

#define TRIG_NODE(InputAttrType, OutputAttrType, NodeName, SetLimits, TrigFuncPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public TrigNode<InputAttrType, OutputAttrType, NodeName, name##NodeName, SetLimits, TrigFuncPtr> {};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "TemplateArgumentsIssues"
TRIG_NODE(double, MAngle, Acos, true, &std::acos);
TRIG_NODE(double, MAngle, Asin, true, &std::asin);
TRIG_NODE(double, MAngle, Atan, false, &std::atan);
TRIG_NODE(MAngle, double, CosAngle, false, &std::cos);
TRIG_NODE(MAngle, double, SinAngle, false, &std::sin);
TRIG_NODE(MAngle, double, TanAngle, false, &std::tan);
#pragma clang diagnostic pop


template<typename TClass, const char* TTypeName>
class Atan2TrigNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<double >(0.0));
        createAttribute(input2Attr_, "input2", DefaultValue<double >(1.0));
        createAttribute(outputAttr_, "output", DefaultValue<MAngle>(0.0), false);
        
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
            const auto input1Value = getAttribute<double>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<double>(dataBlock, input2Attr_);
            
            if (input1Value == 0 && input2Value == 0)
            {
                MGlobal::displayError("Both inputs to atan2 function cannot be 0!");
                return MS::kFailure;
            }
            
            setAttribute(dataBlock, outputAttr_, MAngle(std::atan2(input1Value, input2Value)));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute outputAttr_;
};

template<typename TClass, const char* TTypeName>
Attribute Atan2TrigNode<TClass, TTypeName>::input1Attr_;

template<typename TClass, const char* TTypeName>
Attribute Atan2TrigNode<TClass, TTypeName>::input2Attr_;

template<typename TClass, const char* TTypeName>
Attribute Atan2TrigNode<TClass, TTypeName>::outputAttr_;

#define ATAN2_TRIG_NODE(NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public Atan2TrigNode<NodeName, name##NodeName> {};

ATAN2_TRIG_NODE(Atan2);
