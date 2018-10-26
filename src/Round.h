// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

namespace std_ext
{

template <typename TType>
inline typename std::enable_if<std::is_pod<TType>::value, TType>::type
ceil_t(TType value)
{
    return std::ceil(value);
}

template <typename TType>
inline typename std::enable_if<!std::is_pod<TType>::value, TType>::type
ceil_t(const TType& value);

template <>
inline MAngle ceil_t<MAngle>(const MAngle& value)
{
    return MAngle(std::ceil(value.asDegrees()), MAngle::kDegrees);
}


template <typename TType>
inline typename std::enable_if<std::is_pod<TType>::value, TType>::type
floor_t(TType value)
{
    return std::floor(value);
}

template <typename TType>
inline typename std::enable_if<!std::is_pod<TType>::value, TType>::type
floor_t(const TType& value);

template <>
inline MAngle floor_t<MAngle>(const MAngle& value)
{
    return MAngle(std::floor(value.asDegrees()), MAngle::kDegrees);
}


template <typename TType>
inline typename std::enable_if<std::is_pod<TType>::value, TType>::type
round_t(TType value)
{
    return std::round(value);
}

template <typename TType>
inline typename std::enable_if<!std::is_pod<TType>::value, TType>::type
round_t(const TType& value);

template <>
inline MAngle round_t<MAngle>(const MAngle& value)
{
    return MAngle(std::round(value.asDegrees()), MAngle::kDegrees);
}

}

template<typename TAttrType, typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
class RoundNode : public BaseNode<TClass, TTypeName>
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
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputValue = getAttribute<TAttrType>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, TAttrType((*TOpFucPtr)(inputValue)));
            
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

template<typename TAttrType, typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
Attribute RoundNode<TAttrType, TClass, TTypeName, TOpFuncPtrType, TOpFucPtr>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
Attribute RoundNode<TAttrType, TClass, TTypeName, TOpFuncPtrType, TOpFucPtr>::outputAttr_;

#define ROUND_NODE(AttrType, NodeName, OpFuncPtrType, OpFucPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public RoundNode<AttrType, NodeName, name##NodeName, OpFuncPtrType, OpFucPtr> {};

ROUND_NODE(double, Ceil, double (*)(double), &std_ext::ceil_t);
ROUND_NODE(MAngle, CeilAngle, MAngle (*)(const MAngle&), &std_ext::ceil_t);
ROUND_NODE(double, Floor, double (*)(double), &std_ext::floor_t);
ROUND_NODE(MAngle, FloorAngle, MAngle (*)(const MAngle&), &std_ext::floor_t);
ROUND_NODE(double, Round, double (*)(double), &std_ext::round_t);
ROUND_NODE(MAngle, RoundAngle, MAngle (*)(const MAngle&), &std_ext::round_t);
