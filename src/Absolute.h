// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

namespace std_ext
{
using namespace std;

inline MAngle abs(const MAngle& angle)
{
    return MAngle(std::abs(angle.asRadians()));
}

}

template<typename TAttrType, typename TClass, const char* TTypeName, int TTypeId>
class AbsoluteNode : public BaseNode<TClass, TTypeName, TTypeId>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", TAttrType(0.0));
        createAttribute(outputAttr_, "output", TAttrType(0.0), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_)
        {
            MDataHandle inputAttrHandle = dataBlock.inputValue(inputAttr_);
            const TAttrType inputAttrValue = getAttribute<TAttrType>(inputAttrHandle);
            
            MDataHandle outputAttrHandle = dataBlock.outputValue(outputAttr_);
            outputAttrHandle.set(std_ext::abs(inputAttrValue));
            outputAttrHandle.setClean();
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static MObject inputAttr_;
    static MObject outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName, int TTypeId>
MObject AbsoluteNode<TAttrType, TClass, TTypeName, TTypeId>::inputAttr_; // NOLINT

template<typename TAttrType, typename TClass, const char* TTypeName, int TTypeId>
MObject AbsoluteNode<TAttrType, TClass, TTypeName, TTypeId>::outputAttr_; // NOLINT

#define ABSOLUTE_NODE(AttrType, NodeName, NodeTypeId) \
    constexpr char name##NodeName[] = #NodeName;      \
    class NodeName : public AbsoluteNode<AttrType, NodeName, name##NodeName, NodeTypeId> {}; // NOLINT

ABSOLUTE_NODE(double, Absolute, 1001);
ABSOLUTE_NODE(int, AbsoluteInt, 1002);
ABSOLUTE_NODE(MAngle, AbsoluteAngle, 1003);
