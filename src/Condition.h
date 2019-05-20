// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <algorithm>
#include <limits>
#include <maya/MFnEnumAttribute.h>

#include "Utils.h"

template<typename TAttrType, typename TClass, const char* TTypeName>
class ConditionNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<TAttrType>());
        createAttribute(input2Attr_, "input2", DefaultValue<TAttrType>());
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
            const auto input1Value = getAttribute<TAttrType>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<TAttrType>(dataBlock, input2Attr_);

            MDataHandle operationHandle = dataBlock.inputValue(operationAttr_);
            const auto operation = operationHandle.asShort();
            
            bool status;
            switch (operation)
            {
                default:
                case 0:
                {
                    status = almostEquals(input1Value, input2Value);
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
                    status = !almostEquals(input1Value, input2Value);
                }
                break;
                case 4:
                {
                    status = input1Value < input2Value || almostEquals(input1Value, input2Value);
                }
                break;
                case 5:
                {
                    status = input1Value > input2Value || almostEquals(input1Value, input2Value);
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
CONDITION_NODE(int, CompareInt);


template<typename TAttrType, typename TClass, const char* TTypeName>
class SelectNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<TAttrType>());
        createAttribute(input2Attr_, "input2", DefaultValue<TAttrType>());
        createAttribute(condition_, "condition", false);
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(), false);

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


template<typename TAttrType, typename TClass, const char* TTypeName>
class SelectArrayNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<TAttrType>(), true, true);
        createAttribute(input2Attr_, "input2", DefaultValue<TAttrType>(), true, true);
        createAttribute(condition_, "condition", false);
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(), false, true);
        
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
            const auto input1Value = getAttribute<std::vector<TAttrType>>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<std::vector<TAttrType>>(dataBlock, input2Attr_);
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
Attribute SelectArrayNode<TAttrType, TClass, TTypeName>::input1Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute SelectArrayNode<TAttrType, TClass, TTypeName>::input2Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute SelectArrayNode<TAttrType, TClass, TTypeName>::condition_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute SelectArrayNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define SELECT_ARRAY_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public SelectArrayNode<AttrType, NodeName, name##NodeName> {};

// TODO: add selector for euler and quaternion arrays
SELECT_ARRAY_NODE(double, SelectArray);
SELECT_ARRAY_NODE(int, SelectIntArray);
SELECT_ARRAY_NODE(MAngle, SelectAngleArray);
SELECT_ARRAY_NODE(MVector, SelectVectorArray);
SELECT_ARRAY_NODE(MMatrix, SelectMatrixArray);


template <typename TType>
inline bool logical_and(TType a, TType b)
{
    return a && b;
}

template <typename TType>
inline bool logical_or(TType a, TType b)
{
    return a || b;
}

template <typename TType>
inline bool logical_xor(TType a, TType b)
{
    return !a != !b;
}

template<typename TAttrType, typename TClass, const char* TTypeName, bool (*TFuncPtr)(TAttrType, TAttrType)>
class LogicalNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<TAttrType>());
        createAttribute(input2Attr_, "input2", DefaultValue<TAttrType>());
        createAttribute(outputAttr_, "output", DefaultValue<bool>(), false);
        
        MPxNode::addAttribute(input1Attr_);
        MPxNode::addAttribute(input2Attr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(input1Attr_, outputAttr_);
        MPxNode::attributeAffects(input2Attr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto input1Value = getAttribute<TAttrType>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<TAttrType>(dataBlock, input2Attr_);
            
            setAttribute(dataBlock, outputAttr_, TFuncPtr(input1Value, input2Value));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName, bool (*TFuncPtr)(TAttrType, TAttrType)>
Attribute LogicalNode<TAttrType, TClass, TTypeName, TFuncPtr>::input1Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName, bool (*TFuncPtr)(TAttrType, TAttrType)>
Attribute LogicalNode<TAttrType, TClass, TTypeName, TFuncPtr>::input2Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName, bool (*TFuncPtr)(TAttrType, TAttrType)>
Attribute LogicalNode<TAttrType, TClass, TTypeName, TFuncPtr>::outputAttr_;

#define LOGICAL_NODE(AttrType, NodeName, FuncPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public LogicalNode<AttrType, NodeName, name##NodeName, FuncPtr> {};

LOGICAL_NODE(bool, AndBool, &logical_and);
LOGICAL_NODE(bool, OrBool, &logical_or);
LOGICAL_NODE(bool, XorBool, &logical_xor);
LOGICAL_NODE(int, AndInt, &logical_and);
LOGICAL_NODE(int, OrInt, &logical_or);
LOGICAL_NODE(int, XorInt, &logical_xor);


#define SELECT_GEOM_NODE_TEMPLATE template<typename TClass, const char* TTypeName, MFnData::Type TGeomType>

#define  SELECT_GEOM_NODE_ATTRIBUTE SELECT_GEOM_NODE_TEMPLATE \
    Attribute SelectGeometryNode<TClass, TTypeName, TGeomType>

#define SELECT_GEOM_NODE(NodeName, GeomType) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public SelectGeometryNode<NodeName, name##NodeName, GeomType> {};

SELECT_GEOM_NODE_TEMPLATE
class SelectGeometryNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", TGeomType);
        createAttribute(input2Attr_, "input2", TGeomType);
        createAttribute(condition_, "condition", false);
        createAttribute(outputAttr_, "output", TGeomType, false);
        
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
            const auto input1Value = getAttribute(dataBlock, input1Attr_, TGeomType);
            const auto input2Value = getAttribute(dataBlock, input2Attr_, TGeomType);
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

SELECT_GEOM_NODE_ATTRIBUTE::input1Attr_;
SELECT_GEOM_NODE_ATTRIBUTE::input2Attr_;
SELECT_GEOM_NODE_ATTRIBUTE::condition_;
SELECT_GEOM_NODE_ATTRIBUTE::outputAttr_;

SELECT_GEOM_NODE(SelectCurve, MFnData::kNurbsCurve);
SELECT_GEOM_NODE(SelectMesh, MFnData::kMesh);
SELECT_GEOM_NODE(SelectSurface, MFnData::kNurbsSurface);
