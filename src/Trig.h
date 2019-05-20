// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

inline double cosAngle(MAngle a)
{
    return std::cos(a.asRadians());
}

inline double sinAngle(MAngle a)
{
    return std::sin(a.asRadians());
}

inline double tanAngle(MAngle a)
{
    return std::tan(a.asRadians());
}

#define TRIG_NODE_TEMPLATE template<typename TInputAttrType, \
    typename TOutputAttrType, \
    typename TClass, \
    const char* TTypeName, \
    bool TSetLimits, \
    double (*TTrigFuncPtr)(TInputAttrType)>

#define TRIG_NODE_ATTRIBUTE TRIG_NODE_TEMPLATE \
    Attribute TrigNode<TInputAttrType, TOutputAttrType, TClass, TTypeName, TSetLimits, TTrigFuncPtr>

#define TRIG_NODE(InputAttrType, OutputAttrType, NodeName, SetLimits, TrigFuncPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public TrigNode<InputAttrType, OutputAttrType, NodeName, name##NodeName, SetLimits, TrigFuncPtr> {};

TRIG_NODE_TEMPLATE
class TrigNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TInputAttrType>());
        createAttribute(outputAttr_, "output", DefaultValue<TOutputAttrType>(), false);
        
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
            const auto inputValue = getAttribute<TInputAttrType>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, TTrigFuncPtr(inputValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};

TRIG_NODE_ATTRIBUTE::inputAttr_;
TRIG_NODE_ATTRIBUTE::outputAttr_;

TRIG_NODE(double, MAngle, Acos, true, &std::acos);
TRIG_NODE(double, MAngle, Asin, true, &std::asin);
TRIG_NODE(double, MAngle, Atan, false, &std::atan);
TRIG_NODE(MAngle, double, CosAngle, false, &cosAngle);
TRIG_NODE(MAngle, double, SinAngle, false, &sinAngle);
TRIG_NODE(MAngle, double, TanAngle, false, &tanAngle);


TEMPLATE_PARAMETER_LINKAGE char Atan2NodeName[] = "Atan2";
class Atan2 : public BaseNode<Atan2, Atan2NodeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", 0.0);
        createAttribute(input2Attr_, "input2", 1.0);
        createAttribute(outputAttr_, "output", MAngle(0.0), false);
        
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

Attribute Atan2::input1Attr_;
Attribute Atan2::input2Attr_;
Attribute Atan2::outputAttr_;
