// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <maya/MFnEnumAttribute.h>
#include <maya/MTransformationMatrix.h>

#include "Utils.h"

inline MEulerRotation getRotation(const MMatrix& matrix, MEulerRotation::RotationOrder rotationOrder)
{
    MTransformationMatrix xform(matrix);
    MEulerRotation rotation = xform.eulerRotation();
    rotation.reorderIt(rotationOrder);
    
    return rotation;
}

inline MEulerRotation getRotation(const MQuaternion& quaternion, MEulerRotation::RotationOrder rotationOrder)
{
    MEulerRotation rotation = quaternion.asEulerRotation();
    rotation.reorderIt(rotationOrder);
    
    return rotation;
}

template<typename TAttrType, typename TClass, const char* TTypeName>
class GetRotationNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TAttrType>(0.0));
        createAttribute(outputAttr_, outputAttrX_, outputAttrY_, outputAttrZ_, "output", MEulerRotation::identity, false);
        
        MFnEnumAttribute attrFn;
        rotationOrderAttr_ = attrFn.create("rotationOrder", "rotationOrder");
        attrFn.addField("xyz", 0);
        attrFn.addField("yzx", 1);
        attrFn.addField("zxy", 2);
        attrFn.addField("xzy", 3);
        attrFn.addField("yxz", 4);
        attrFn.addField("zyx", 5);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(rotationOrderAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        MPxNode::attributeAffects(rotationOrderAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            MDataHandle inputHandle = dataBlock.inputValue(inputAttr_);
            const auto inputValue = getAttribute<TAttrType>(inputHandle);
            
            MDataHandle rotOrderHandle = dataBlock.inputValue(rotationOrderAttr_);
            const auto rotationOrder = MEulerRotation::RotationOrder(rotOrderHandle.asShort());
            
            MEulerRotation rotation = getRotation(inputValue, rotationOrder);
            
            MDataHandle outputXHandle = dataBlock.outputValue(outputAttrX_);
            outputXHandle.set(MAngle(rotation.x));
            outputXHandle.setClean();
    
            MDataHandle outputYHandle = dataBlock.outputValue(outputAttrY_);
            outputYHandle.set(MAngle(rotation.y));
            outputYHandle.setClean();
    
            MDataHandle outputZHandle = dataBlock.outputValue(outputAttrZ_);
            outputZHandle.set(MAngle(rotation.z));
            outputZHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAttr_;
    static MObject rotationOrderAttr_;
    static MObject outputAttr_;
    static MObject outputAttrX_;
    static MObject outputAttrY_;
    static MObject outputAttrZ_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject GetRotationNode<TAttrType, TClass, TTypeName>::inputAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject GetRotationNode<TAttrType, TClass, TTypeName>::rotationOrderAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject GetRotationNode<TAttrType, TClass, TTypeName>::outputAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject GetRotationNode<TAttrType, TClass, TTypeName>::outputAttrX_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject GetRotationNode<TAttrType, TClass, TTypeName>::outputAttrY_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName>
MObject GetRotationNode<TAttrType, TClass, TTypeName>::outputAttrZ_; // NOLINT

#define GET_ROTATION_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public GetRotationNode<AttrType, NodeName, name##NodeName> {}; // NOLINT

GET_ROTATION_NODE(MMatrix, RotationFromMatrix);
GET_ROTATION_NODE(MQuaternion, RotationFromQuaternion);
