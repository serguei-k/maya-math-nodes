// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <maya/MFnEnumAttribute.h>
#include <maya/MTransformationMatrix.h>

#include "Utils.h"

template <typename TInType, typename TOutType>
inline TOutType getRotation(const TInType& source, MEulerRotation::RotationOrder rotationOrder);

template <>
inline MEulerRotation getRotation(const MMatrix& source, MEulerRotation::RotationOrder rotationOrder)
{
    return MEulerRotation::decompose(source, rotationOrder);
}

template <>
inline MQuaternion getRotation(const MMatrix& source, MEulerRotation::RotationOrder)
{
    MTransformationMatrix xform(source);
    
    return xform.rotation();
}

template <>
inline MEulerRotation getRotation(const MQuaternion& source, MEulerRotation::RotationOrder rotationOrder)
{
    MEulerRotation rotation = source.asEulerRotation();
    rotation.reorderIt(rotationOrder);
    
    return rotation;
}

template <>
inline MQuaternion getRotation(const MEulerRotation& source, MEulerRotation::RotationOrder rotationOrder)
{
    return MEulerRotation(source.x, source.y, source.z, rotationOrder).asQuaternion();
}

template <>
inline MMatrix getRotation(const MEulerRotation& source, MEulerRotation::RotationOrder rotationOrder)
{
    return MEulerRotation(source.x, source.y, source.z, rotationOrder).asMatrix();
}

template <>
inline MMatrix getRotation(const MQuaternion& source, MEulerRotation::RotationOrder)
{
    return source.asMatrix();
}


template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName>
class GetRotationNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TInAttrType>());
        createAttribute(outputAttr_, "output", DefaultValue<TOutAttrType>(), false);
        
        createRotationOrderAttribute(rotationOrderAttr_);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(rotationOrderAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        MPxNode::attributeAffects(rotationOrderAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputValue = getAttribute<TInAttrType>(dataBlock, inputAttr_);
            
            MDataHandle rotOrderHandle = dataBlock.inputValue(rotationOrderAttr_);
            const auto rotationOrder = MEulerRotation::RotationOrder(rotOrderHandle.asShort());
            
            const auto rotation = getRotation<TInAttrType, TOutAttrType>(inputValue, rotationOrder);
            setAttribute(dataBlock, outputAttr_, rotation);
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute rotationOrderAttr_;
    static Attribute outputAttr_;
};

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName>
Attribute GetRotationNode<TInAttrType, TOutAttrType, TClass, TTypeName>::inputAttr_;

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName>
Attribute GetRotationNode<TInAttrType, TOutAttrType, TClass, TTypeName>::rotationOrderAttr_;

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName>
Attribute GetRotationNode<TInAttrType, TOutAttrType, TClass, TTypeName>::outputAttr_;

#define GET_ROTATION_NODE(InAttrType, OutAttrType, NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public GetRotationNode<InAttrType, OutAttrType, NodeName, name##NodeName> {};

GET_ROTATION_NODE(MMatrix, MEulerRotation, RotationFromMatrix);
GET_ROTATION_NODE(MQuaternion, MEulerRotation, RotationFromQuaternion);
GET_ROTATION_NODE(MMatrix, MQuaternion, QuaternionFromMatrix);
GET_ROTATION_NODE(MEulerRotation, MQuaternion, QuaternionFromRotation);
GET_ROTATION_NODE(MEulerRotation, MMatrix, MatrixFromRotation);
GET_ROTATION_NODE(MQuaternion, MMatrix, MatrixFromQuaternion);


inline MVector getTranslationFromMatrix(const MMatrix& matrix)
{
    return MVector(matrix[3][0], matrix[3][1], matrix[3][2]);
}

inline MVector getScaleFromMatrix(const MMatrix& matrix)
{
    MTransformationMatrix xform(matrix);
    
    double3 scale {1.0, 1.0, 1.0};
    xform.getScale(scale, MSpace::kTransform);
    
    return MVector(scale);
}

template<typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
class GetVectorNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<MMatrix>());
        createAttribute(outputAttr_, "output", DefaultValue<MVector>(), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputValue = getAttribute<MMatrix>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, (*TOpFucPtr)(inputValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};

template<typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
Attribute GetVectorNode<TClass, TTypeName, TOpFuncPtrType, TOpFucPtr>::inputAttr_;

template<typename TClass, const char* TTypeName, typename TOpFuncPtrType, TOpFuncPtrType TOpFucPtr>
Attribute GetVectorNode<TClass, TTypeName, TOpFuncPtrType, TOpFucPtr>::outputAttr_;

#define GET_VECTOR_NODE(NodeName, OpFuncPtrType, OpFucPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public GetVectorNode<NodeName, name##NodeName, OpFuncPtrType, OpFucPtr> {};

GET_VECTOR_NODE(TranslationFromMatrix, MVector (*)(const MMatrix&), &getTranslationFromMatrix);
GET_VECTOR_NODE(ScaleFromMatrix, MVector (*)(const MMatrix&), &getScaleFromMatrix);


template<typename TClass, const char* TTypeName>
class GetMatrixNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(translationAttr_, "translation", DefaultValue<MVector>());
        createAttribute(rotationAttr_, "rotation", DefaultValue<MEulerRotation>());
        createAttribute(scaleAttr_, "scale", DefaultValue<MVector>(1.0, 1.0, 1.0));
        createAttribute(outputAttr_, "output", DefaultValue<MMatrix>(), false);
        
        createRotationOrderAttribute(rotationOrderAttr_, 1);
        
        MPxNode::addAttribute(translationAttr_);
        MPxNode::addAttribute(rotationAttr_);
        MPxNode::addAttribute(scaleAttr_);
        MPxNode::addAttribute(rotationOrderAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(translationAttr_, outputAttr_);
        MPxNode::attributeAffects(rotationAttr_, outputAttr_);
        MPxNode::attributeAffects(scaleAttr_, outputAttr_);
        MPxNode::attributeAffects(rotationOrderAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto translationValue = getAttribute<MVector>(dataBlock, translationAttr_);
            auto rotationValue = getAttribute<MEulerRotation>(dataBlock, rotationAttr_);
            const auto scaleValue = getAttribute<MVector>(dataBlock, scaleAttr_);
            
            MDataHandle rotOrderHandle = dataBlock.inputValue(rotationOrderAttr_);
            const auto rotationOrder = MTransformationMatrix::RotationOrder(rotOrderHandle.asShort());
            
            MTransformationMatrix xform;
            xform.setTranslation(translationValue, MSpace::kTransform);
            
            double3 rotation {rotationValue.x, rotationValue.y, rotationValue.z};
            xform.setRotation(rotation, rotationOrder);
            
            double3 scale {1.0, 1.0, 1.0};
            scaleValue.get(scale);
            xform.setScale(scale, MSpace::kTransform);
            
            setAttribute(dataBlock, outputAttr_, xform.asMatrix());
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute translationAttr_;
    static Attribute rotationAttr_;
    static Attribute scaleAttr_;
    static Attribute rotationOrderAttr_;
    static Attribute outputAttr_;
};

template<typename TClass, const char* TTypeName>
Attribute GetMatrixNode<TClass, TTypeName>::translationAttr_;

template<typename TClass, const char* TTypeName>
Attribute GetMatrixNode<TClass, TTypeName>::rotationAttr_;

template<typename TClass, const char* TTypeName>
Attribute GetMatrixNode<TClass, TTypeName>::scaleAttr_;

template<typename TClass, const char* TTypeName>
Attribute GetMatrixNode<TClass, TTypeName>::rotationOrderAttr_;

template<typename TClass, const char* TTypeName>
Attribute GetMatrixNode<TClass, TTypeName>::outputAttr_;

#define GET_MATRIX_NODE(NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public GetMatrixNode<NodeName, name##NodeName> {};

GET_MATRIX_NODE(MatrixFromTRS);


template<typename TClass, const char* TTypeName>
class GetAxisNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<MMatrix>());
        createAttribute(outputAttr_, "output", DefaultValue<MVector>(), false);
    
        MFnEnumAttribute attrFn;
        axisAttr_ = attrFn.create("axis", "axis");
        attrFn.addField("x", 0);
        attrFn.addField("y", 1);
        attrFn.addField("z", 2);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(axisAttr_);
        MPxNode::addAttribute(outputAttr_);
    
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        MPxNode::attributeAffects(axisAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputValue = getAttribute<MMatrix>(dataBlock, inputAttr_);
    
            MDataHandle axisHandle = dataBlock.inputValue(axisAttr_);
            const auto axis = axisHandle.asShort();
            
            setAttribute(dataBlock, outputAttr_, MVector(inputValue[axis][0], inputValue[axis][1], inputValue[axis][2]));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute axisAttr_;
    static Attribute outputAttr_;
};

template<typename TClass, const char* TTypeName>
Attribute GetAxisNode<TClass, TTypeName>::inputAttr_;

template<typename TClass, const char* TTypeName>
Attribute GetAxisNode<TClass, TTypeName>::axisAttr_;

template<typename TClass, const char* TTypeName>
Attribute GetAxisNode<TClass, TTypeName>::outputAttr_;

#define GET_AXIS_NODE(NodeName) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public GetAxisNode<NodeName, name##NodeName> {};

GET_AXIS_NODE(AxisFromMatrix);


TEMPLATE_PARAMETER_LINKAGE char MatrixFromDirectionNodeName[] = "MatrixFromDirection";
class MatrixFromDirection : public BaseNode<MatrixFromDirection, MatrixFromDirectionNodeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(directionAttr_, "direction", DefaultValue<MVector>(1.0, 0.0, 0.0));
        createAttribute(upAttr_, "up", DefaultValue<MVector>(0.0, 1.0, 0.0));
        createAttribute(outputAttr_, "output", DefaultValue<MMatrix>(), false);
        
        MFnEnumAttribute attrFn;
        alignmentAttr_ = attrFn.create("alignment", "alignment");
        attrFn.addField("xy", 0);
        attrFn.addField("xz", 1);
        attrFn.addField("yx", 2);
        attrFn.addField("yz", 3);
        attrFn.addField("zx", 4);
        attrFn.addField("zy", 5);
        
        MPxNode::addAttribute(directionAttr_);
        MPxNode::addAttribute(upAttr_);
        MPxNode::addAttribute(alignmentAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(directionAttr_, outputAttr_);
        MPxNode::attributeAffects(upAttr_, outputAttr_);
        MPxNode::attributeAffects(alignmentAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto directionValue = getAttribute<MVector>(dataBlock, directionAttr_).normal();
            auto upValue = getAttribute<MVector>(dataBlock, upAttr_).normal();
            
            if (directionValue.isParallel(upValue))
            {
                setAttribute(dataBlock, outputAttr_, MMatrix::identity);
                MGlobal::displayWarning("Direction and up vectors cannot be parallel!");
                
                return MS::kSuccess;
            }
            
            MDataHandle alignmentHandle = dataBlock.inputValue(alignmentAttr_);
            const auto alignmentValue = alignmentHandle.asShort();
            
            const auto cross = directionValue ^ upValue;
            upValue = cross ^ directionValue;
            
            double xformData[4][4] = {{1.0, 0.0, 0.0, 0.0},
                                      {0.0, 1.0, 0.0, 0.0},
                                      {0.0, 0.0, 1.0, 0.0},
                                      {0.0, 0.0, 0.0, 1.0}};
            
            unsigned crossAxis = 0;
            if (alignmentValue == 0 || alignmentValue == 1)
            {
                directionValue.get(xformData[0]);
                upValue.get(xformData[alignmentValue == 0 ? 1 : 2]);
                cross.get(xformData[alignmentValue == 0 ? 2 : 1]);
                crossAxis = alignmentValue == 0 ? 2 : 1;
            }
            else if (alignmentValue == 2 || alignmentValue == 3)
            {
                directionValue.get(xformData[1]);
                upValue.get(xformData[alignmentValue == 2 ? 0 : 2]);
                cross.get(xformData[alignmentValue == 2 ? 2 : 0]);
                crossAxis = alignmentValue == 2 ? 2 : 0;
            }
            else
            {
                directionValue.get(xformData[2]);
                upValue.get(xformData[alignmentValue == 4 ? 0 : 1]);
                cross.get(xformData[alignmentValue == 4 ? 1 : 0]);
                crossAxis = alignmentValue == 4 ? 1 : 0;
            }
            
            MMatrix xform(xformData);
            if (xform.det3x3() < 0)
            {
                (-cross).get(xform[crossAxis]);
            }
            
            setAttribute(dataBlock, outputAttr_, xform);
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute directionAttr_;
    static Attribute upAttr_;
    static Attribute alignmentAttr_;
    static Attribute outputAttr_;
};

Attribute MatrixFromDirection::directionAttr_;
Attribute MatrixFromDirection::upAttr_;
Attribute MatrixFromDirection::alignmentAttr_;
Attribute MatrixFromDirection::outputAttr_;
