// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <algorithm>
#include <limits>
#include <maya/MFnEnumAttribute.h>

#include "Utils.h"

inline bool equals(double a, double b)
{
    return std::abs(a - b) <= std::numeric_limits<double>::epsilon() * std::abs(a + b);
}

template<typename TAttrType, typename TClass, const char* TTypeName>
class ConditionNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<TAttrType>(0.0));
        createAttribute(input2Attr_, "input2", DefaultValue<TAttrType>(0.0));
        createAttribute(outputAttr_, "output", true, false);

        MFnEnumAttribute attrFn;
        operationAttr_ = attrFn.create("operation", "operation");
        attrFn.addField("Equal", 0);
        attrFn.addField("Less Than", 1);
        attrFn.addField("Greater Than", 2);
        attrFn.addField("Not Equal", 3);
        attrFn.addField("Less Than or Equal", 4);
        attrFn.addField("Greater Than or Equal", 5);

        MPxNode::addAttribute(input1Attr_);
        MPxNode::addAttribute(input2Attr_);
        MPxNode::addAttribute(operationAttr_);
        MPxNode::addAttribute(outputAttr_);

        MPxNode::attributeAffects(input1Attr_, outputAttr_);
        MPxNode::attributeAffects(input2Attr_, outputAttr_);
        MPxNode::attributeAffects(operationAttr_, outputAttr_);

        return MS::kSuccess;
    }

    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto input1Value = getAttribute<TAttrType, double>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<TAttrType, double>(dataBlock, input2Attr_);

            MDataHandle operationHandle = dataBlock.inputValue(operationAttr_);
            const auto operation = operationHandle.asShort();
            
            bool status;
            switch (operation)
            {
                default:
                case 0:
                {
                    status = equals(input1Value, input2Value);
                }
                break;
                case 1:
                {
                    status = input1Value < input2Value;
                }
                break;
                case 2:
                {
                    status = input1Value > input2Value;
                }
                break;
                case 3:
                {
                    status = !equals(input1Value, input2Value);
                }
                break;
                case 4:
                {
                    status = input1Value < input2Value || equals(input1Value, input2Value);
                }
                break;
                case 5:
                {
                    status = input1Value > input2Value || equals(input1Value, input2Value);
                }
            }

            setAttribute(dataBlock, outputAttr_, status);

            return MS::kSuccess;
        }

        return MS::kUnknownParameter;
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute operationAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ConditionNode<TAttrType, TClass, TTypeName>::input1Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ConditionNode<TAttrType, TClass, TTypeName>::input2Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ConditionNode<TAttrType, TClass, TTypeName>::operationAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ConditionNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define CONDITION_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public ConditionNode<AttrType, NodeName, name##NodeName> {};

CONDITION_NODE(double, Compare);
CONDITION_NODE(MAngle, CompareAngle);


template<typename TAttrType, typename TClass, const char* TTypeName>
class SelectNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<TAttrType>(0.0));
        createAttribute(input2Attr_, "input2", DefaultValue<TAttrType>(0.0));
        createAttribute(condition_, "condition", false);
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(0.0), false);

        MPxNode::addAttribute(input1Attr_);
        MPxNode::addAttribute(input2Attr_);
        MPxNode::addAttribute(condition_);
        MPxNode::addAttribute(outputAttr_);

        MPxNode::attributeAffects(input1Attr_, outputAttr_);
        MPxNode::attributeAffects(input2Attr_, outputAttr_);
        MPxNode::attributeAffects(condition_, outputAttr_);

        return MS::kSuccess;
    }

    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto input1Value = getAttribute<TAttrType>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<TAttrType>(dataBlock, input2Attr_);
            const auto conditionValue = getAttribute<bool>(dataBlock, condition_);

            setAttribute(dataBlock, outputAttr_, conditionValue ? input2Value : input1Value);

            return MS::kSuccess;
        }

        return MS::kUnknownParameter;
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute condition_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute SelectNode<TAttrType, TClass, TTypeName>::input1Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute SelectNode<TAttrType, TClass, TTypeName>::input2Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute SelectNode<TAttrType, TClass, TTypeName>::condition_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute SelectNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define SELECT_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public SelectNode<AttrType, NodeName, name##NodeName> {};

SELECT_NODE(double, Select);
SELECT_NODE(int, SelectInt);
SELECT_NODE(MAngle, SelectAngle);
SELECT_NODE(MVector, SelectVector);
SELECT_NODE(MMatrix, SelectMatrix);
SELECT_NODE(MEulerRotation, SelectRotation);
SELECT_NODE(MQuaternion, SelectQuaternion);
