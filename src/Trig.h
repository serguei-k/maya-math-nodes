// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TInputAttrType, typename TOutputAttrType, typename TClass,
         const char* TTypeName, bool TSetLimits, double (*TTrigFuncPtr)(double)>
class TrigNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", CastTo<TInputAttrType>(0.0));
        createAttribute(outputAttr_, "output", CastTo<TOutputAttrType>(0.0), false);
        
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
            MDataHandle inputHandle = dataBlock.inputValue(inputAttr_);
            const double inputValue = getAttribute<TInputAttrType, double>(inputHandle);
            
            MDataHandle outputHandle = dataBlock.outputValue(outputAttr_);
            outputHandle.set(TOutputAttrType(TTrigFuncPtr(inputValue)));
            outputHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAttr_;
    static MObject outputAttr_;
};

template<typename TInputAttrType, typename TOutputAttrType, typename TClass,
         const char* TTypeName, bool TSetLimits, double (*TTrigFuncPtr)(double)>
MObject TrigNode<TInputAttrType, TOutputAttrType, TClass, TTypeName, TSetLimits, TTrigFuncPtr>::inputAttr_; // NOLINT

template<typename TInputAttrType, typename TOutputAttrType, typename TClass,
         const char* TTypeName, bool TSetLimits, double (*TTrigFuncPtr)(double)>
MObject TrigNode<TInputAttrType, TOutputAttrType, TClass, TTypeName, TSetLimits, TTrigFuncPtr>::outputAttr_; // NOLINT

#define TRIG_NODE(InputAttrType, OutputAttrType, NodeName, SetLimits, TrigFuncPtr) \
    constexpr char name##NodeName[] = #NodeName;                                   \
    class NodeName : public TrigNode<InputAttrType, OutputAttrType, NodeName, name##NodeName, SetLimits, TrigFuncPtr> {}; // NOLINT

#pragma clang diagnostic push
#pragma ide diagnostic ignored "TemplateArgumentsIssues"
TRIG_NODE(double, MAngle, AcosAngle, true, &std::acos);
TRIG_NODE(double, MAngle, AsinAngle, true, &std::asin);
TRIG_NODE(double, MAngle, AtanAngle, false, &std::atan);
TRIG_NODE(MAngle, double, Cos, false, &std::cos);
TRIG_NODE(MAngle, double, Sin, false, &std::sin);
TRIG_NODE(MAngle, double, Tan, false, &std::tan);
#pragma clang diagnostic pop
