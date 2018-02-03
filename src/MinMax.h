// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

namespace std
{

template <>
inline const MAngle& max(const MAngle& a, const MAngle& b)
{
    return (a.asRadians() > b.asRadians()) ? a : b;
}

template <>
inline const MAngle& min(const MAngle& a, const MAngle& b)
{
    return (a.asRadians() < b.asRadians()) ? a : b;
}

}

template<typename TAttrType, typename TClass, const char* TTypeName,
         const TAttrType& (*TOpFuncPtr)(const TAttrType&, const TAttrType&)>
class MinMaxNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAAttr_, "inputA", DefaultValue<TAttrType>(0.0));
        createAttribute(inputBAttr_, "inputB", DefaultValue<TAttrType>(1.0));
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(0.0), false);
        
        MPxNode::addAttribute(inputBAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAAttr_, outputAttr_);
        MPxNode::attributeAffects(inputBAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputAValue = getAttribute<TAttrType>(dataBlock, inputAAttr_);
            const auto inputBValue = getAttribute<TAttrType>(dataBlock, inputBAttr_);
            
            setAttribute(dataBlock, outputAttr_, TOpFuncPtr(inputAValue, inputBValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAAttr_;
    static Attribute inputBAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName,
         const TAttrType& (*TOpFuncPtr)(const TAttrType&, const TAttrType&)>
Attribute MinMaxNode<TAttrType, TClass, TTypeName, TOpFuncPtr>::inputAAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName,
         const TAttrType& (*TOpFuncPtr)(const TAttrType&, const TAttrType&)>
Attribute MinMaxNode<TAttrType, TClass, TTypeName, TOpFuncPtr>::inputBAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName,
         const TAttrType& (*TOpFuncPtr)(const TAttrType&, const TAttrType&)>
Attribute MinMaxNode<TAttrType, TClass, TTypeName, TOpFuncPtr>::outputAttr_;

#define MIN_MAX_NODE(AttrType, NodeName, OpFuncPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public MinMaxNode<AttrType, NodeName, name##NodeName, OpFuncPtr> {};

#pragma clang diagnostic push
#pragma ide diagnostic ignored "TemplateArgumentsIssues"
MIN_MAX_NODE(double, Max, &std::max);
MIN_MAX_NODE(double, Min, &std::min);
MIN_MAX_NODE(int, MaxInt, &std::max);
MIN_MAX_NODE(int, MinInt, &std::min);
MIN_MAX_NODE(MAngle, MaxAngle, &std::max);
MIN_MAX_NODE(MAngle, MinAngle, &std::min);
#pragma clang diagnostic pop
