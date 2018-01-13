// Copyright 2018 Serguei Kalentchouk, et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <cmath>

#include <maya/MAngle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPxNode.h>

#define TRIG_NODE(NodeName, NodeTypeId, TrigFuncPtr)    \
    constexpr char name##NodeName[] = #NodeName;        \
    class NodeName : public TrigNode<NodeName, name##NodeName, NodeTypeId, TrigFuncPtr> {}; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, double (*TTrigFuncPtr)(double)>
class TrigNode : public MPxNode
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
        MFnNumericAttribute numericAttrFn;
        MFnUnitAttribute unitAttrFn;
        
        inputAttribute_ = unitAttrFn.create("input", "input", MAngle(0.0));
        unitAttrFn.setKeyable(true);
        
        outputAttribute_ = numericAttrFn.create("output", "output", MFnNumericData::kDouble, 0.0);
        
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
            const MAngle inputAttrValue = inputAttrHandle.asAngle();
            
            MDataHandle outputAttrHandle = dataBlock.outputValue(outputAttribute_);
            outputAttrHandle.setDouble(TTrigFuncPtr(inputAttrValue.asRadians()));
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
MObject TrigNode<TClass, TTypeName, TTypeId, TTrigFuncPtr>::inputAttribute_; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, double (*TTrigFuncPtr)(double)>
MObject TrigNode<TClass, TTypeName, TTypeId, TTrigFuncPtr>::outputAttribute_; // NOLINT


#define TRIG_INVERSE_NODE(NodeName, NodeTypeId, TrigFuncPtr)    \
    constexpr char name##NodeName[] = #NodeName;                \
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
        MFnNumericAttribute numericAttrFn;
        MFnUnitAttribute unitAttrFn;
        
        inputAttribute_ = numericAttrFn.create("input", "input", MFnNumericData::kDouble, 0.0);
        numericAttrFn.setKeyable(true);
        
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
// TODO: replace with real ids
TRIG_NODE(Cos, 1001, &std::cos);
TRIG_NODE(Sin, 1002, &std::sin);
TRIG_NODE(Tan, 1003, &std::tan);
TRIG_INVERSE_NODE(Acos, 1004, &std::acos);
TRIG_INVERSE_NODE(Asin, 1005, &std::asin);
TRIG_INVERSE_NODE(Atan, 1006, &std::atan);
#pragma clang diagnostic pop
