// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <algorithm>
#include <cmath>

#include "MinMax.h"
#include "Utils.h"

template<typename TAttrType, typename TClass, const char* TTypeName>
class ClampNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TAttrType>());
        createAttribute(inputMinAttr_, "inputMin", DefaultValue<TAttrType>());
        createAttribute(inputMaxAttr_, "inputMax", TAttrType(1.0));
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(inputMinAttr_);
        MPxNode::addAttribute(inputMaxAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        MPxNode::attributeAffects(inputMinAttr_, outputAttr_);
        MPxNode::attributeAffects(inputMaxAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const TAttrType inputValue = getAttribute<TAttrType>(dataBlock, inputAttr_);
            const TAttrType inputMinValue = getAttribute<TAttrType>(dataBlock, inputMinAttr_);
            const TAttrType inputMaxValue = getAttribute<TAttrType>(dataBlock, inputMaxAttr_);
            
            setAttribute(dataBlock, outputAttr_, TAttrType(std::max(inputMinValue, std::min(inputValue, inputMaxValue))));
            
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
    static Attribute inputMinAttr_;
    static Attribute inputMaxAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::inputMinAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::inputMaxAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute ClampNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define CLAMP_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public ClampNode<AttrType, NodeName, name##NodeName> {};

CLAMP_NODE(double, Clamp);
CLAMP_NODE(int, ClampInt);
CLAMP_NODE(MAngle, ClampAngle);


template <typename TType>
inline TType remapValue(const TType& value,
                        const TType& low1,
                        const TType& high1,
                        const TType& low2,
                        const TType& high2)
{
    const auto divisor = (high1 - low1);
    if (almostEquals(divisor, TType(0.0))) return TType(0);
    
    return low2 + (high2 - low2) * ((value - low1) / divisor);
}

template <>
inline int remapValue(const int& value,
                      const int& low1,
                      const int& high1,
                      const int& low2,
                      const int& high2)
{
    const float divisor = float(high1 - low1);
    if (almostEquals(divisor, 0.0f)) return 0;
    
    const float result = low2 + (high2 - low2) * ((value - low1) / divisor);
    return result > 0.0f ? int(result + 0.5f) : int(result - 0.5f);
}


template<typename TAttrType, typename TClass, const char* TTypeName>
class RemapNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TAttrType>());
        createAttribute(low1Attr_, "low1", DefaultValue<TAttrType>());
        createAttribute(high1Attr_, "high1", DefaultValue<TAttrType>());
        createAttribute(low2Attr_, "low2", DefaultValue<TAttrType>());
        createAttribute(high2Attr_, "high2", DefaultValue<TAttrType>());
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(low1Attr_);
        MPxNode::addAttribute(high1Attr_);
        MPxNode::addAttribute(low2Attr_);
        MPxNode::addAttribute(high2Attr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        MPxNode::attributeAffects(low1Attr_, outputAttr_);
        MPxNode::attributeAffects(high1Attr_, outputAttr_);
        MPxNode::attributeAffects(low2Attr_, outputAttr_);
        MPxNode::attributeAffects(high2Attr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const TAttrType inputValue = getAttribute<TAttrType>(dataBlock, inputAttr_);
            const TAttrType low1Value = getAttribute<TAttrType>(dataBlock, low1Attr_);
            const TAttrType high1Value = getAttribute<TAttrType>(dataBlock, high1Attr_);
            const TAttrType low2Value = getAttribute<TAttrType>(dataBlock, low2Attr_);
            const TAttrType high2Value = getAttribute<TAttrType>(dataBlock, high2Attr_);
            
            setAttribute(dataBlock, outputAttr_,
                         remapValue(inputValue, low1Value, high1Value, low2Value, high2Value));
            
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
    static Attribute low1Attr_;
    static Attribute high1Attr_;
    static Attribute low2Attr_;
    static Attribute high2Attr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute RemapNode<TAttrType, TClass, TTypeName>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute RemapNode<TAttrType, TClass, TTypeName>::low1Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute RemapNode<TAttrType, TClass, TTypeName>::high1Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute RemapNode<TAttrType, TClass, TTypeName>::low2Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute RemapNode<TAttrType, TClass, TTypeName>::high2Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute RemapNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define REMAP_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public RemapNode<AttrType, NodeName, name##NodeName> {};

REMAP_NODE(double, Remap);
REMAP_NODE(int, RemapInt);
REMAP_NODE(MAngle, RemapAngle);


TEMPLATE_PARAMETER_LINKAGE char smoothstepNodeName[] = "Smoothstep";
class Smoothstep : public BaseNode<Smoothstep, smoothstepNodeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", 0.0);
        createAttribute(outputAttr_, "output", 0.0, false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            auto inputValue = getAttribute<double>(dataBlock, inputAttr_);
            inputValue = std::max(0.0, std::min(inputValue, 1.0));
            
            setAttribute(dataBlock, outputAttr_, inputValue * inputValue * (3 - 2 * inputValue));
            
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

Attribute Smoothstep::inputAttr_;
Attribute Smoothstep::outputAttr_;
