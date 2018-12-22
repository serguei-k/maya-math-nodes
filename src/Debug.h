// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include "Utils.h"

static inline void
logToConsole(const MString& name, double value)
{
    MGlobal::displayInfo(name + ": " + value);
}

static inline void
logToConsole(const MString& name, int value)
{
    MGlobal::displayInfo(name + ": " + value);
}

static inline void
logToConsole(const MString& name, const MAngle& value)
{
    MGlobal::displayInfo(name + ": " + value.asDegrees());
}

static inline void
logToConsole(const MString& name, const MVector& value)
{
    MGlobal::displayInfo(name + ": (" + value.x + ", " + value.y + ", " + value.z + ")");
}

static inline void
logToConsole(const MString& name, const MEulerRotation& value)
{
    MGlobal::displayInfo(name + ": (" + value.x + ", " + value.y + ", " + value.z + ")");
}

static inline void
logToConsole(const MString& name, const MQuaternion& value)
{
    MGlobal::displayInfo(name + ": (" + value.x + ", " + value.y + ", " + value.z + ", " + value.w + ")");
}

static inline void
logToConsole(const MString& name, const MMatrix& value)
{
    auto data = name + ":\n(";
    
    for (auto i = 0u; i < 4; ++i)
    {
        if (i != 0)
        {
            data += ",\n ";
        }
        
        data += value[i][0];
        data += ", ";
        data += value[i][1];
        data += ", ";
        data += value[i][2];
        data += ", ";
        data += value[i][3];
    }
    
    data += ")";
    
    MGlobal::displayInfo(data);
}


template<typename TAttrType, typename TClass, const char* TTypeName>
class DebugLogNode : public BaseNode<TClass, TTypeName>
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
            setAttribute(dataBlock, outputAttr_, inputValue);
            
            logToConsole(this->name(), inputValue);
            
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
Attribute DebugLogNode<TAttrType, TClass, TTypeName>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute DebugLogNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define DEBUG_LOG_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public DebugLogNode<AttrType, NodeName, name##NodeName> {};

DEBUG_LOG_NODE(double, DebugLog);
DEBUG_LOG_NODE(int, DebugLogInt);
DEBUG_LOG_NODE(MAngle, DebugLogAngle);
DEBUG_LOG_NODE(MVector, DebugLogVector);
DEBUG_LOG_NODE(MEulerRotation, DebugLogRotation);
DEBUG_LOG_NODE(MQuaternion, DebugLogQuaternion);
DEBUG_LOG_NODE(MMatrix, DebugLogMatrix);
