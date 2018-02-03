// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <maya/MFnEnumAttribute.h>
#include <maya/MTransformationMatrix.h>

#include "Utils.h"

template <typename TInType, typename TOutType>
inline TOutType getRotation(const TInType& source, MEulerRotation::RotationOrder rotationOrder);

template <>
inline MEulerRotation getRotation(const MMatrix& source, MEulerRotation::RotationOrder rotationOrder)
{
    MTransformationMatrix xform(source);
    MEulerRotation rotation = xform.eulerRotation();
    rotation.reorderIt(rotationOrder);
    
    return rotation;
}

template <>
inline MQuaternion getRotation(const MMatrix& source, MEulerRotation::RotationOrder)
{
    MTransformationMatrix xform(source);
    
    return xform.rotation();
}

template <>
inline MEulerRotation getRotation(const MQuaternion& source, MEulerRotation::RotationOrder rotationOrder)
{
    MEulerRotation rotation = source.asEulerRotation();
    rotation.reorderIt(rotationOrder);
    
    return rotation;
}

template <>
inline MEulerRotation getRotation(const MEulerRotation& source, MEulerRotation::RotationOrder rotationOrder)
{
    return MEulerRotation(source.x, source.y, source.z, rotationOrder);
}

template <>
inline MQuaternion getRotation(const MEulerRotation& source, MEulerRotation::RotationOrder rotationOrder)
{
    return MEulerRotation(source.x, source.y, source.z, rotationOrder).asQuaternion();
}

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName>
class GetRotationNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TInAttrType>(0.0));
        createAttribute(outputAttr_, "output", DefaultValue<TOutAttrType>(0.0), false);
        
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
            const auto inputValue = getAttribute<TInAttrType>(dataBlock, inputAttr_);
            
            MDataHandle rotOrderHandle = dataBlock.inputValue(rotationOrderAttr_);
            const auto rotationOrder = MEulerRotation::RotationOrder(rotOrderHandle.asShort());
            
            const auto rotation = getRotation<TInAttrType, TOutAttrType>(inputValue, rotationOrder);
            setAttribute(dataBlock, outputAttr_, rotation);
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute rotationOrderAttr_;
    static Attribute outputAttr_;
};

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName>
Attribute GetRotationNode<TInAttrType, TOutAttrType, TClass, TTypeName>::inputAttr_;

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName>
Attribute GetRotationNode<TInAttrType, TOutAttrType, TClass, TTypeName>::rotationOrderAttr_;

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName>
Attribute GetRotationNode<TInAttrType, TOutAttrType, TClass, TTypeName>::outputAttr_;

#define GET_ROTATION_NODE(InAttrType, OutAttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public GetRotationNode<InAttrType, OutAttrType, NodeName, name##NodeName> {};

GET_ROTATION_NODE(MMatrix, MEulerRotation, RotationFromMatrix);
GET_ROTATION_NODE(MQuaternion, MEulerRotation, RotationFromQuaternion);
GET_ROTATION_NODE(MMatrix, MQuaternion, QuaternionFromMatrix);
GET_ROTATION_NODE(MEulerRotation, MQuaternion, QuaternionFromRotation);
