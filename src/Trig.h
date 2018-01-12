// Copyright 2018 Serguei Kalentchouk, et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <cmath>

#include <maya/MAngle.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPxNode.h>

#define TRIG_INVERSE_NODE(NodeName, NodeTypeId, TrigFuncPtr)                                        \
    constexpr char name##NodeName[] = #NodeName;                                                    \
    class NodeName : public TrigInverseNode<NodeName, name##NodeName, NodeTypeId, TrigFuncPtr> {}; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, double (*TTrigFuncPtr)(double)>
class TrigInverseNode : public MPxNode
{
public:
    static constexpr const char* kTypeName = TTypeName;
    static constexpr int kTypeId = TTypeId;
    
    static void* create()
    {
        return new TClass();
    }
    
    static MStatus initialize()
    {
        MFnEnumAttribute enumAttrFn;
        MFnNumericAttribute numericAttrFn;
        MFnUnitAttribute unitAttrFn;
        
        inputAttribute_ = numericAttrFn.create("input", "input", MFnNumericData::kDouble, 0.0);
        outputAttribute_ = unitAttrFn.create("output", "output", MAngle(0.0));
        
        addAttribute(inputAttribute_);
        addAttribute(outputAttribute_);
        
        attributeAffects(inputAttribute_, outputAttribute_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttribute_)
        {
            MDataHandle inputAttrHandle = dataBlock.inputValue(inputAttribute_);
            const double inputAttrValue = inputAttrHandle.asDouble();
            
            MAngle angle(TTrigFuncPtr(inputAttrValue));
            
            MDataHandle outputAttrHandle = dataBlock.outputValue(outputAttribute_);
            outputAttrHandle.setMAngle(angle);
            outputAttrHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAttribute_;
    static MObject outputAttribute_;
};

template<class TClass, const char* TTypeName, int TTypeId, double (*TTrigFuncPtr)(double)>
MObject TrigInverseNode<TClass, TTypeName, TTypeId, TTrigFuncPtr>::inputAttribute_; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, double (*TTrigFuncPtr)(double)>
MObject TrigInverseNode<TClass, TTypeName, TTypeId, TTrigFuncPtr>::outputAttribute_; // NOLINT

#pragma clang diagnostic push
#pragma ide diagnostic ignored "TemplateArgumentsIssues"
TRIG_INVERSE_NODE(Acos, 0001, &std::acos);
TRIG_INVERSE_NODE(Asin, 0003, &std::asin);
TRIG_INVERSE_NODE(Atan, 0002, &std::atan);
#pragma clang diagnostic pop
