// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <maya/MFnEnumAttribute.h>
#include <maya/MTransformationMatrix.h>

#include "Utils.h"

template<typename TType>
inline TType lerp(const TType& value1, const TType& value2, double alpha)
{
    return value1 + (value2 - value1) * alpha;
}

template<>
inline MMatrix lerp(const MMatrix& value1, const MMatrix& value2, double alpha)
{
    MTransformationMatrix outXform(MMatrix::identity);
    const MTransformationMatrix xform1(value1);
    const MTransformationMatrix xform2(value2);
    
    double3 scale1, scale2, outScale;
    xform1.getScale(scale1, MSpace::kWorld);
    xform2.getScale(scale2, MSpace::kWorld);
    
    outScale[0] = lerp(scale1[0], scale2[0], alpha);
    outScale[1] = lerp(scale1[1], scale2[1], alpha);
    outScale[2] = lerp(scale1[2], scale2[2], alpha);
    
    double3 shear1, shear2, outShear;
    xform1.getShear(shear1, MSpace::kWorld);
    xform2.getShear(shear2, MSpace::kWorld);
    
    outShear[0] = lerp(shear1[0], shear2[0], alpha);
    outShear[1] = lerp(shear1[1], shear2[1], alpha);
    outShear[2] = lerp(shear1[2], shear2[2], alpha);
    
    MQuaternion outQuaternion = slerp(xform1.rotation(), xform2.rotation(), alpha);
    
    outXform.setScale(outScale, MSpace::kWorld);
    outXform.setShear(outShear, MSpace::kWorld);
    outXform.setRotationQuaternion(outQuaternion.x, outQuaternion.y, outQuaternion.z, outQuaternion.w);
    outXform.setTranslation(lerp(xform1.getTranslation(MSpace::kWorld),
                                 xform2.getTranslation(MSpace::kWorld),
                                 alpha), MSpace::kWorld);
    
    return outXform.asMatrix();
}

template<typename TAttrType, typename TClass, const char* TTypeName>
class LerpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<TAttrType>());
        createAttribute(input2Attr_, "input2", DefaultValue<TAttrType>());
        createAttribute(alphaAttr_, "alpha", 0.5);
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(), false);
        
        MFnNumericAttribute attrFn(alphaAttr_);
        attrFn.setMin(0.0);
        attrFn.setMax(1.0);
        
        MPxNode::addAttribute(input1Attr_);
        MPxNode::addAttribute(input2Attr_);
        MPxNode::addAttribute(alphaAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(input1Attr_, outputAttr_);
        MPxNode::attributeAffects(input2Attr_, outputAttr_);
        MPxNode::attributeAffects(alphaAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto input1Value = getAttribute<TAttrType>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<TAttrType>(dataBlock, input2Attr_);
            const auto alphaValue = getAttribute<double>(dataBlock, alphaAttr_);
            
            setAttribute(dataBlock, outputAttr_, lerp(input1Value, input2Value, alphaValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute alphaAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute LerpNode<TAttrType, TClass, TTypeName>::input1Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute LerpNode<TAttrType, TClass, TTypeName>::input2Attr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute LerpNode<TAttrType, TClass, TTypeName>::alphaAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName>
Attribute LerpNode<TAttrType, TClass, TTypeName>::outputAttr_;

#define LERP_NODE(AttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public LerpNode<AttrType, NodeName, name##NodeName> {};

LERP_NODE(double, Lerp);
LERP_NODE(MAngle, LerpAngle);
LERP_NODE(MVector, LerpVector);
LERP_NODE(MMatrix, LerpMatrix);


template<typename TClass, const char* TTypeName>
class SlerpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(input1Attr_, "input1", DefaultValue<MQuaternion>());
        createAttribute(input2Attr_, "input2", DefaultValue<MQuaternion>());
        createAttribute(alphaAttr_, "alpha", 0.5);
        createAttribute(outputAttr_, "output", DefaultValue<MQuaternion>(), false);
        
        MFnNumericAttribute attrFn(alphaAttr_);
        attrFn.setMin(0.0);
        attrFn.setMax(1.0);
        
        MFnEnumAttribute eAttrFn;
        interpTypeAttr_ = eAttrFn.create("interpolationType", "interpolationType", 0);
        eAttrFn.addField("Short", 0);
        eAttrFn.addField("Long", 1);
        
        MPxNode::addAttribute(input1Attr_);
        MPxNode::addAttribute(input2Attr_);
        MPxNode::addAttribute(alphaAttr_);
        MPxNode::addAttribute(interpTypeAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(input1Attr_, outputAttr_);
        MPxNode::attributeAffects(input2Attr_, outputAttr_);
        MPxNode::attributeAffects(alphaAttr_, outputAttr_);
        MPxNode::attributeAffects(interpTypeAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto input1Value = getAttribute<MQuaternion>(dataBlock, input1Attr_);
            const auto input2Value = getAttribute<MQuaternion>(dataBlock, input2Attr_);
            const auto alphaValue = getAttribute<double>(dataBlock, alphaAttr_);
            
            MDataHandle interpTypeHandle = dataBlock.inputValue(interpTypeAttr_);
            const auto interpType = interpTypeHandle.asShort() * -1;
            
            setAttribute(dataBlock, outputAttr_, slerp(input1Value, input2Value, alphaValue, interpType));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute input1Attr_;
    static Attribute input2Attr_;
    static Attribute alphaAttr_;
    static Attribute interpTypeAttr_;
    static Attribute outputAttr_;
};

template<typename TClass, const char* TTypeName>
Attribute SlerpNode<TClass, TTypeName>::input1Attr_;

template<typename TClass, const char* TTypeName>
Attribute SlerpNode<TClass, TTypeName>::input2Attr_;

template<typename TClass, const char* TTypeName>
Attribute SlerpNode<TClass, TTypeName>::alphaAttr_;

template<typename TClass, const char* TTypeName>
Attribute SlerpNode<TClass, TTypeName>::interpTypeAttr_;

template<typename TClass, const char* TTypeName>
Attribute SlerpNode<TClass, TTypeName>::outputAttr_;

#define SLERP_NODE(NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public SlerpNode<NodeName, name##NodeName> {};

SLERP_NODE(SlerpQuaternion);
