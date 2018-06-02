// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

template<typename TType>
inline double distance(const TType& value1, const TType& value2);

template<>
inline double distance(const MMatrix& value1, const MMatrix& value2)
{
    return (MVector(value1[3]) - MVector(value2[3])).length();
}

template<>
inline double distance(const MVector& value1, const MVector& value2)
{
    return (value1 - value2).length();
}


template<typename TAttrType, typename TClass, const char* TTypeName>
class DistanceNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<TAttrType>());
        createAttribute(input2Attr_, "input2", DefaultValue<TAttrType>());
        createAttribute(outputAttr_, "output", 0.0, false);
        
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
            
            setAttribute(dataBlock, outputAttr_, distance(input1Value, input2Value));
            
            return MS::kSuccess;
        }

        return MS::kUnknownParameter;
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute DistanceNode<TAttrType, TClass, TTypeName>::input1Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute DistanceNode<TAttrType, TClass, TTypeName>::input2Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute DistanceNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define DISTANCE_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public DistanceNode<AttrType, NodeName, name##NodeName> {};

DISTANCE_NODE(MVector, DistancePoints);
DISTANCE_NODE(MMatrix, DistanceTransforms);
