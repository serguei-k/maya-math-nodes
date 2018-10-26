// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

namespace std_ext
{

template <typename TType>
inline typename std::enable_if<std::is_pod<TType>::value, TType>::type
abs_t(TType value)
{
    return std::abs(value);
}

template <typename TType>
inline typename std::enable_if<!std::is_pod<TType>::value, TType>::type
abs_t(const TType& value);

template <>
inline MAngle abs_t<MAngle>(const MAngle& value)
{
    return MAngle(std::abs(value.asRadians()));
}

}

template<typename TAttrType, typename TClass, const char* TTypeName>
class AbsoluteNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TAttrType>());
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            const auto inputAttrValue = getAttribute<TAttrType>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, std_ext::abs_t(inputAttrValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }
    
    MPlug passThroughToOne(const MPlug& plug) const override
    {
        if (plug == inputAttr_)
        {
            return MPlug(this->thisMObject(), outputAttr_);
        }
        
        return MPlug();
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute AbsoluteNode<TAttrType, TClass, TTypeName>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute AbsoluteNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define ABSOLUTE_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public AbsoluteNode<AttrType, NodeName, name##NodeName> {};

ABSOLUTE_NODE(double, Absolute);
ABSOLUTE_NODE(int, AbsoluteInt);
ABSOLUTE_NODE(MAngle, AbsoluteAngle);
