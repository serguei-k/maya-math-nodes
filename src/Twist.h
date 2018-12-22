// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <maya/MFnEnumAttribute.h>
#include <maya/MTransformationMatrix.h>

#include "Convert.h"
#include "Utils.h"

template<typename TInAttrType, typename TClass, const char* TTypeName>
class GetTwistNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TInAttrType>());
        createAttribute(outputAttr_, "output", DefaultValue<MAngle>(), false);
        
        MFnEnumAttribute attrFn;
        axisAttr_ = attrFn.create("axis", "axis");
        attrFn.addField("x", 0);
        attrFn.addField("y", 1);
        attrFn.addField("z", 2);
        
        createRotationOrderAttribute(rotationOrderAttr_);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(axisAttr_);
        MPxNode::addAttribute(rotationOrderAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        MPxNode::attributeAffects(axisAttr_, outputAttr_);
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
            
            MDataHandle axisHandle = dataBlock.inputValue(axisAttr_);
            const auto axis = axisHandle.asShort();
            
            auto quaternion = getRotation<TInAttrType, MQuaternion>(inputValue, rotationOrder);
            
            double angle = 0.0;
            switch (axis)
            {
                case 0:
                {
                    quaternion.y = 0.0;
                    quaternion.z = 0.0;
                    angle = quaternion.asEulerRotation().x;
                    break;
                }
                case 1:
                {
                    quaternion.x = 0.0;
                    quaternion.z = 0.0;
                    angle = quaternion.asEulerRotation().y;
                    break;
                }
                case 2:
                {
                    quaternion.x = 0.0;
                    quaternion.y = 0.0;
                    angle = quaternion.asEulerRotation().z;
                    break;
                }
                default:
                    break;
            }
            
            setAttribute(dataBlock, outputAttr_, angle);
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute axisAttr_;
    static Attribute rotationOrderAttr_;
    static Attribute outputAttr_;
};

template<typename TInAttrType, typename TClass, const char* TTypeName>
Attribute GetTwistNode<TInAttrType, TClass, TTypeName>::inputAttr_;

template<typename TInAttrType, typename TClass, const char* TTypeName>
Attribute GetTwistNode<TInAttrType, TClass, TTypeName>::axisAttr_;

template<typename TInAttrType, typename TClass, const char* TTypeName>
Attribute GetTwistNode<TInAttrType, TClass, TTypeName>::rotationOrderAttr_;

template<typename TInAttrType, typename TClass, const char* TTypeName>
Attribute GetTwistNode<TInAttrType, TClass, TTypeName>::outputAttr_;

#define GET_TWIST_NODE(InAttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public GetTwistNode<InAttrType, NodeName, name##NodeName> {};

GET_TWIST_NODE(MEulerRotation, TwistFromRotation);
GET_TWIST_NODE(MMatrix, TwistFromMatrix);
