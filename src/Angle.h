// Copyright 2018 Serguei Kalentchouk, et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <cmath>

#include <maya/MAngle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MPxNode.h>

namespace
{

MAngle addAngles(const MAngle& angleA, const MAngle& angleB)
{
    return MAngle(angleA.asRadians() + angleB.asRadians());
}

MAngle divideAngle(const MAngle& angle, double value)
{
    return MAngle(angle.asRadians() / value);
}

MAngle multiplyAngle(const MAngle& angle, double value)
{
    return MAngle(angle.asRadians() * value);
}

MAngle subtractAngles(const MAngle& angleA, const MAngle& angleB)
{
    return MAngle(angleA.asRadians() - angleB.asRadians());
}

}

#define ANGLE_ANGLE_NODE(NodeName, NodeTypeId, TAngleOpFuncPtr)   \
    constexpr char name##NodeName[] = #NodeName;                  \
    class NodeName : public AngleAngleNode<NodeName, name##NodeName, NodeTypeId, TAngleOpFuncPtr> {}; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, MAngle (*TAngleOpFuncPtr)(const MAngle&, const MAngle&)>
class AngleAngleNode : public MPxNode
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
        MFnUnitAttribute unitAttrFn;
        
        inputAAttribute_ = unitAttrFn.create("inputA", "inputA", MAngle(0.0));
        unitAttrFn.setKeyable(true);
        
        inputBAttribute_ = unitAttrFn.create("inputB", "inputB", MAngle(0.0));
        unitAttrFn.setKeyable(true);
        
        outputAttribute_ = unitAttrFn.create("output", "output", MAngle(0.0));
        
        addAttribute(inputAAttribute_);
        addAttribute(inputBAttribute_);
        addAttribute(outputAttribute_);
        
        attributeAffects(inputAAttribute_, outputAttribute_);
        attributeAffects(inputBAttribute_, outputAttribute_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttribute_)
        {
            MDataHandle inputAAttrHandle = dataBlock.inputValue(inputAAttribute_);
            const MAngle inputAAttrValue = inputAAttrHandle.asAngle();
    
            MDataHandle inputBAttrHandle = dataBlock.inputValue(inputBAttribute_);
            const MAngle inputBAttrValue = inputBAttrHandle.asAngle();
            
            MDataHandle outputAttrHandle = dataBlock.outputValue(outputAttribute_);
            outputAttrHandle.setMAngle(TAngleOpFuncPtr(inputAAttrValue, inputBAttrValue));
            outputAttrHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAAttribute_;
    static MObject inputBAttribute_;
    static MObject outputAttribute_;
};

template<class TClass, const char* TTypeName, int TTypeId, MAngle (*TAngleOpFuncPtr)(const MAngle&, const MAngle&)>
MObject AngleAngleNode<TClass, TTypeName, TTypeId, TAngleOpFuncPtr>::inputAAttribute_; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, MAngle (*TAngleOpFuncPtr)(const MAngle&, const MAngle&)>
MObject AngleAngleNode<TClass, TTypeName, TTypeId, TAngleOpFuncPtr>::inputBAttribute_; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, MAngle (*TAngleOpFuncPtr)(const MAngle&, const MAngle&)>
MObject AngleAngleNode<TClass, TTypeName, TTypeId, TAngleOpFuncPtr>::outputAttribute_; // NOLINT


#define ANGLE_DOUBLE_NODE(NodeName, NodeTypeId, TAngleOpFuncPtr)   \
    constexpr char name##NodeName[] = #NodeName;                  \
    class NodeName : public AngleDoubleNode<NodeName, name##NodeName, NodeTypeId, TAngleOpFuncPtr> {}; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, MAngle (*TAngleOpFuncPtr)(const MAngle&, double)>
class AngleDoubleNode : public MPxNode
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
        
        inputAAttribute_ = unitAttrFn.create("inputA", "inputA", MAngle(0.0));
        unitAttrFn.setKeyable(true);
        
        inputBAttribute_ = numericAttrFn.create("inputB", "inputB", MFnNumericData::kDouble, 0.0);
        numericAttrFn.setKeyable(true);
        
        outputAttribute_ = unitAttrFn.create("output", "output", MAngle(0.0));
        
        addAttribute(inputAAttribute_);
        addAttribute(inputBAttribute_);
        addAttribute(outputAttribute_);
        
        attributeAffects(inputAAttribute_, outputAttribute_);
        attributeAffects(inputBAttribute_, outputAttribute_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttribute_)
        {
            MDataHandle inputAAttrHandle = dataBlock.inputValue(inputAAttribute_);
            const MAngle inputAAttrValue = inputAAttrHandle.asAngle();
            
            MDataHandle inputBAttrHandle = dataBlock.inputValue(inputBAttribute_);
            const double inputBAttrValue = inputBAttrHandle.asDouble();
            
            MDataHandle outputAttrHandle = dataBlock.outputValue(outputAttribute_);
            outputAttrHandle.setMAngle(TAngleOpFuncPtr(inputAAttrValue, inputBAttrValue));
            outputAttrHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAAttribute_;
    static MObject inputBAttribute_;
    static MObject outputAttribute_;
};

template<class TClass, const char* TTypeName, int TTypeId, MAngle (*TAngleOpFuncPtr)(const MAngle&, double)>
MObject AngleDoubleNode<TClass, TTypeName, TTypeId, TAngleOpFuncPtr>::inputAAttribute_; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, MAngle (*TAngleOpFuncPtr)(const MAngle&, double)>
MObject AngleDoubleNode<TClass, TTypeName, TTypeId, TAngleOpFuncPtr>::inputBAttribute_; // NOLINT

template<class TClass, const char* TTypeName, int TTypeId, MAngle (*TAngleOpFuncPtr)(const MAngle&, double)>
MObject AngleDoubleNode<TClass, TTypeName, TTypeId, TAngleOpFuncPtr>::outputAttribute_; // NOLINT

#pragma clang diagnostic push
#pragma ide diagnostic ignored "TemplateArgumentsIssues"
// TODO: replace with real ids
ANGLE_ANGLE_NODE(AddAngle, 1007, &addAngles);
ANGLE_ANGLE_NODE(SubtractAngle, 1008, &subtractAngles);
ANGLE_DOUBLE_NODE(DivideAngle, 1009, &divideAngle);
ANGLE_DOUBLE_NODE(MultiplyAngle, 1010, &multiplyAngle);
#pragma clang diagnostic pop
