// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <numeric>

#include "Utils.h"

template<typename TType>
inline TType average(const std::vector<TType>& values)
{
    if (values.empty()) return DefaultValue<TType>();
    
    TType sum = std::accumulate(values.begin(), values.end(), DefaultValue<TType>(),
                                [](const TType& a, const TType& b){ return a + b; });
    return TType(sum / int(values.size()));
}

template<>
inline MEulerRotation average(const std::vector<MEulerRotation>& values)
{
    if (values.empty()) return MEulerRotation::identity;
    
    MVector axes;
    for (const auto& rotation : values)
    {
        MVector axis = MVector::zAxis;
        double angle = 0.0;
        
        rotation.asQuaternion().getAxisAngle(axis, angle);
        axes += axis.normal() * angle;
    }
    axes /= values.size();
    
    const MQuaternion average(axes.length(), axes.normal());
    return average.asEulerRotation();
}

template<typename TAttrType, typename TClass, const char* TTypeName>
class AverageNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TAttrType>(), true, true);
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
            const auto inputValue = getAttribute<std::vector<TAttrType>>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, average(inputValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute AverageNode<TAttrType, TClass, TTypeName>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute AverageNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define AVERAGE_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public AverageNode<AttrType, NodeName, name##NodeName> {};

AVERAGE_NODE(double, Average);
AVERAGE_NODE(int, AverageInt);
AVERAGE_NODE(MAngle, AverageAngle);
AVERAGE_NODE(MVector, AverageVector);
AVERAGE_NODE(MEulerRotation, AverageRotation);
