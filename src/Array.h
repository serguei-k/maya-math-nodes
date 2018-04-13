// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <numeric>

#include "Utils.h"

template<typename TType>
inline TType sum(const std::vector<TType>& values)
{
    if (values.empty()) return DefaultValue<TType>();
    
    return std::accumulate(values.begin(), values.end(), DefaultValue<TType>(),
                           [](const TType& a, const TType& b){ return a + b; });
}

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
    
    MQuaternion sum;
    for (const auto& rotation : values)
    {
        sum = sum + rotation.asQuaternion().log();
    }
    
    const auto count = values.size();
    const MQuaternion average(sum.x / count, sum.y / count, sum.z / count, sum.w / count);
    
    return average.exp().asEulerRotation();
}

// Alternative matrix to quaternion conversion to overcome some numerical instability
// observed with the output from MTransformationMatrix.rotation()
inline MQuaternion MatrixToQuaternion(const MMatrix& matrix)
{
    // w, x, y, z
    double out[4];
    
    double in[4][4];
    matrix.get(in);
    
    out[0] = 0.5 * std::sqrt(std::max(0.0, 1.0 + in[0][0] + in[1][1] + in[2][2]));
    out[1] = 0.5 * std::sqrt(std::max(0.0, 1.0 + in[0][0] - in[1][1] - in[2][2]));
    out[2] = 0.5 * std::sqrt(std::max(0.0, 1.0 - in[0][0] + in[1][1] - in[2][2]));
    out[3] = 0.5 * std::sqrt(std::max(0.0, 1.0 - in[0][0] - in[1][1] + in[2][2]));
    
    out[1] = copysign(out[1], in[2][1] - in[1][2]);
    out[2] = copysign(out[2], in[0][2] - in[2][0]);
    out[3] = copysign(out[3], in[1][0] - in[0][1]);
    
    return MQuaternion(out[1], out[2], out[3], out[0]).normal().conjugate();
}

// Note: this algorithm does not consider negative scaling
template<>
inline MMatrix average(const std::vector<MMatrix>& values)
{
    if (values.empty()) return MMatrix::identity;

    MQuaternion rotation;
    MVector position, scale, shear;
    
    for (const auto& matrix : values) {
        const MTransformationMatrix xform(matrix);
        
        double3 scaleData = {1.0, 1.0, 1.0};
        double3 shearData = {0.0, 0.0, 0.0};
        
        xform.getScale(scaleData, MSpace::kObject);
        xform.getShear(shearData, MSpace::kObject);
        
        if (scaleData[0] != 0.0)
        {
            scaleData[0] = std::log(std::abs(scaleData[0]));
        }
        
        if (scaleData[1] != 0.0)
        {
            scaleData[1] = std::log(std::abs(scaleData[1]));
        }
        
        if (scaleData[2] != 0.0)
        {
            scaleData[2] = std::log(std::abs(scaleData[2]));
        }
        
        scale += MVector(scaleData);
        shear += MVector(shearData);
        
        rotation = rotation + MatrixToQuaternion(matrix).log();
        position += xform.getTranslation(MSpace::kWorld);
    }

    const auto count = values.size();
    const MVector scaleAverage = scale / count;
    const MVector shearAverage = shear / count;
    
    double3 scaleData = {1.0, 1.0, 1.0};
    double3 shearData = {0.0, 0.0, 0.0};
    
    scaleAverage.get(scaleData);
    shearAverage.get(shearData);
    
    scaleData[0] = std::exp(scaleData[0]);
    scaleData[1] = std::exp(scaleData[1]);
    scaleData[2] = std::exp(scaleData[2]);
    
    const MVector positionAverage = position / count;
    const auto rotationAverage = MQuaternion(rotation.x / count,
                                             rotation.y / count,
                                             rotation.z / count,
                                             rotation.w / count).exp();
    
    MTransformationMatrix xform;
    xform.setScale(scaleData, MSpace::kObject);
    xform.setShear(shearData, MSpace::kObject);
    xform.setRotationQuaternion(rotationAverage.x, rotationAverage.y, rotationAverage.z, rotationAverage.w);
    xform.setTranslation(positionAverage, MSpace::kWorld);
    
    return xform.asMatrix();
}


template<typename TAttrType, typename TClass, const char* TTypeName,
        TAttrType (*TFuncPtr)(const std::vector<TAttrType>&)>
class ArrayOpNode : public BaseNode<TClass, TTypeName>
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
            
            setAttribute(dataBlock, outputAttr_, TFuncPtr(inputValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};

template<typename TAttrType, typename TClass, const char* TTypeName, TAttrType (*TFuncPtr)(const std::vector<TAttrType>&)>
Attribute ArrayOpNode<TAttrType, TClass, TTypeName, TFuncPtr>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName, TAttrType (*TFuncPtr)(const std::vector<TAttrType>&)>
Attribute ArrayOpNode<TAttrType, TClass, TTypeName, TFuncPtr>::outputAttr_;


#define ARRAY_OP_NODE(AttrType, NodeName, FuncPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public ArrayOpNode<AttrType, NodeName, name##NodeName, FuncPtr> {};

ARRAY_OP_NODE(double, Average, &average);
ARRAY_OP_NODE(int, AverageInt, &average);
ARRAY_OP_NODE(MAngle, AverageAngle, &average);
ARRAY_OP_NODE(MVector, AverageVector, &average);
ARRAY_OP_NODE(MEulerRotation, AverageRotation, &average);
ARRAY_OP_NODE(MMatrix, AverageMatrix, &average);

ARRAY_OP_NODE(double, Sum, &sum);
ARRAY_OP_NODE(int, SumInt, &sum);
ARRAY_OP_NODE(MAngle, SumAngle, &sum);
ARRAY_OP_NODE(MVector, SumVector, &sum);
