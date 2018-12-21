// Copyright (c) 2018 Serguei Kalentchouk et al. All rights reserved.
// Use of this source code is governed by an MIT license that can be found in the LICENSE file.
#pragma once

#include <algorithm>
#include <numeric>

#include "Utils.h"

template<typename TType>
inline TType sum(const std::vector<TType>& values)
{
    if (values.empty()) return DefaultValue<TType>();
    
    return std::accumulate(values.begin(), values.end(), DefaultValue<TType>(),
                           [](const TType& a, const TType& b){ return a + b; });
}

template<typename TInType, typename TOutType>
inline TOutType average(const std::vector<TInType>& values)
{
    if (values.empty()) return DefaultValue<TOutType>();
    
    TInType sum = std::accumulate(values.begin(), values.end(), DefaultValue<TInType>(),
                                  [](const TInType& a, const TInType& b){ return a + b; });
    return TOutType(sum / int(values.size()));
}

template<typename TInType, typename TOutType>
inline TOutType average(const std::vector<TInType>& values, const std::vector<double>& weights)
{
    if (values.empty() || weights.empty()) return DefaultValue<TOutType>();
    
    const double weightSum = std::accumulate(weights.begin(), weights.end(), 0.0);
    if (almostEquals(weightSum, 0.0)) return DefaultValue<TOutType>();
    
    auto sum = DefaultValue<TInType>();
    for (unsigned i = 0u; i < values.size(); ++i)
    {
        sum = sum + values[i] * weights[i];
    }
    
    return TOutType(sum / weightSum);
}

template<>
inline double average(const std::vector<int>& values)
{
    if (values.empty()) return 0.0;
    
    const int sum = std::accumulate(values.begin(), values.end(), 0);
    return double(sum) / values.size();
}

template<>
inline double average(const std::vector<int>& values, const std::vector<double>& weights)
{
    if (values.empty() || weights.empty()) return 0.0;
    
    const double weightSum = std::accumulate(weights.begin(), weights.end(), 0.0);
    if (almostEquals(weightSum, 0.0)) return 0.0;
    
    double sum = 0.0;
    for (unsigned i = 0u; i < values.size(); ++i)
    {
        sum = sum + values[i] * weights[i];
    }
    
    return sum / weightSum;
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

template<>
inline MEulerRotation average(const std::vector<MEulerRotation>& values, const std::vector<double>& weights)
{
    if (values.empty() || weights.empty()) return MEulerRotation::identity;
    
    const double weightSum = std::accumulate(weights.begin(), weights.end(), 0.0);
    if (almostEquals(weightSum, 0.0)) return MEulerRotation::identity;
    
    MQuaternion sum;
    for (unsigned i = 0u; i < values.size(); ++i)
    {
        sum = sum + values[i].asQuaternion().log() * weights[i];
    }
    
    const MQuaternion average(sum.x / weightSum, sum.y / weightSum, sum.z / weightSum, sum.w / weightSum);
    
    return average.exp().asEulerRotation();
}

template<>
inline MQuaternion average(const std::vector<MQuaternion>& values)
{
    if (values.empty()) return MQuaternion::identity;
    
    MQuaternion sum;
    for (const auto& rotation : values)
    {
        sum = sum + rotation.log();
    }
    
    const auto count = values.size();
    const MQuaternion average(sum.x / count, sum.y / count, sum.z / count, sum.w / count);
    
    return average.exp();
}

template<>
inline MQuaternion average(const std::vector<MQuaternion>& values, const std::vector<double>& weights)
{
    if (values.empty() || weights.empty()) return MQuaternion::identity;
    
    const double weightSum = std::accumulate(weights.begin(), weights.end(), 0.0);
    if (almostEquals(weightSum, 0.0)) return MQuaternion::identity;
    
    MQuaternion sum;
    for (unsigned i = 0u; i < values.size(); ++i)
    {
        sum = sum + values[i].log() * weights[i];
    }
    
    const MQuaternion average(sum.x / weightSum, sum.y / weightSum, sum.z / weightSum, sum.w / weightSum);
    
    return average.exp();
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

template<>
inline MMatrix average(const std::vector<MMatrix>& values, const std::vector<double>& weights)
{
    if (values.empty() || weights.empty()) return MMatrix::identity;
    
    const double weightSum = std::accumulate(weights.begin(), weights.end(), 0.0);
    if (almostEquals(weightSum, 0.0)) return MMatrix::identity;
    
    MQuaternion rotation;
    MVector position, scale, shear;
    
    for (unsigned i = 0u; i < values.size(); ++i) {
        const MTransformationMatrix xform(values[i]);
        
        double3 scaleData = {1.0, 1.0, 1.0};
        double3 shearData = {0.0, 0.0, 0.0};
        
        xform.getScale(scaleData, MSpace::kObject);
        xform.getShear(shearData, MSpace::kObject);
        
        if (scaleData[0] != 0.0)
        {
            scaleData[0] = std::log(std::abs(scaleData[0])) * weights[i];
        }
        
        if (scaleData[1] != 0.0)
        {
            scaleData[1] = std::log(std::abs(scaleData[1])) * weights[i];
        }
        
        if (scaleData[2] != 0.0)
        {
            scaleData[2] = std::log(std::abs(scaleData[2])) * weights[i];
        }
        
        scale += MVector(scaleData);
        shear += MVector(shearData);
        
        rotation = rotation + MatrixToQuaternion(values[i]).log() * weights[i];
        position += xform.getTranslation(MSpace::kWorld) * weights[i];
    }
    
    const MVector scaleAverage = scale / weightSum;
    const MVector shearAverage = shear / weightSum;
    
    double3 scaleData = {1.0, 1.0, 1.0};
    double3 shearData = {0.0, 0.0, 0.0};
    
    scaleAverage.get(scaleData);
    shearAverage.get(shearData);
    
    scaleData[0] = std::exp(scaleData[0]);
    scaleData[1] = std::exp(scaleData[1]);
    scaleData[2] = std::exp(scaleData[2]);
    
    const MVector positionAverage = position / weightSum;
    const auto rotationAverage = MQuaternion(rotation.x / weightSum,
                                             rotation.y / weightSum,
                                             rotation.z / weightSum,
                                             rotation.w / weightSum).exp();
    
    MTransformationMatrix xform;
    xform.setScale(scaleData, MSpace::kObject);
    xform.setShear(shearData, MSpace::kObject);
    xform.setRotationQuaternion(rotationAverage.x, rotationAverage.y, rotationAverage.z, rotationAverage.w);
    xform.setTranslation(positionAverage, MSpace::kWorld);
    
    return xform.asMatrix();
}

inline std::vector<double> normalize(const std::vector<double>& values)
{
    std::vector<double> out;
    if (values.empty()) return out;
    
    const double s = sum(values);
    if (almostEquals(s, 0.0)) return values;
    
    out.reserve(values.size());
    for (const auto& value : values)
    {
        out.push_back(value / s);
    }
    
    return out;
}

inline std::vector<double> clamp(const std::vector<double>& values)
{
    std::vector<double> out;
    if (values.empty()) return out;
    
    out.reserve(values.size());
    for (const auto& value : values)
    {
        out.push_back(std::max(0.0, std::min(value, 1.0)));
    }
    
    return out;
}

inline std::vector<double> normalizeWeights(const std::vector<double>& values)
{
    std::vector<double> out = clamp(values);
    if (out.empty()) return out;
    
    const double s = sum(out);
    if (s < 1.0) return out;
    
    for (auto& item : out)
    {
        item /= s;
    }
    
    return out;
}

template<typename TType>
TType min_array_element(const std::vector<TType>& values)
{
    if (values.empty()) return DefaultValue<TType>();
    return *std::min_element(values.begin(), values.end());
}

template<>
MAngle min_array_element(const std::vector<MAngle>& values)
{
    if (values.empty()) return MAngle();
    
    std::vector<double> angleValues;
    angleValues.reserve(values.size());
    
    for (const auto& value : values)
    {
        angleValues.push_back(value.asRadians());
    }
    
    return MAngle(*std::min_element(angleValues.begin(), angleValues.end()));
}

template<typename TType>
TType max_array_element(const std::vector<TType>& values)
{
    if (values.empty()) return DefaultValue<TType>();
    return *std::max_element(values.begin(), values.end());
}

template<>
MAngle max_array_element(const std::vector<MAngle>& values)
{
    if (values.empty()) return MAngle();
    
    std::vector<double> angleValues;
    angleValues.reserve(values.size());
    
    for (const auto& value : values)
    {
        angleValues.push_back(value.asRadians());
    }
    
    return MAngle(*std::max_element(angleValues.begin(), angleValues.end()));
}


template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName,
    TOutAttrType (*TFuncPtr)(const std::vector<TInAttrType>&)>
class ArrayOpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TInAttrType>(), true, true);
        createAttribute(outputAttr_, "output", DefaultValue<TOutAttrType>(), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto inputValue = getAttribute<std::vector<TInAttrType>>(dataBlock, inputAttr_);
            
            setAttribute(dataBlock, outputAttr_, TFuncPtr(inputValue));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute outputAttr_;
};

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName,
    TOutAttrType (*TFuncPtr)(const std::vector<TInAttrType>&)>
Attribute ArrayOpNode<TInAttrType, TOutAttrType, TClass, TTypeName, TFuncPtr>::inputAttr_;

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName,
    TOutAttrType (*TFuncPtr)(const std::vector<TInAttrType>&)>
Attribute ArrayOpNode<TInAttrType, TOutAttrType, TClass, TTypeName, TFuncPtr>::outputAttr_;


#define ARRAY_OP_NODE(InAttrType, OutAttrType, NodeName, FuncPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public ArrayOpNode<InAttrType, OutAttrType, NodeName, name##NodeName, FuncPtr> {};

ARRAY_OP_NODE(double, double, Average, &average);
ARRAY_OP_NODE(int, double, AverageInt, &average);
ARRAY_OP_NODE(MAngle, MAngle, AverageAngle, &average);
ARRAY_OP_NODE(MVector, MVector, AverageVector, &average);
ARRAY_OP_NODE(MEulerRotation, MEulerRotation, AverageRotation, &average);
ARRAY_OP_NODE(MMatrix, MMatrix, AverageMatrix, &average);
ARRAY_OP_NODE(MQuaternion, MQuaternion, AverageQuaternion, &average);

ARRAY_OP_NODE(double, double, Sum, &sum);
ARRAY_OP_NODE(int, int, SumInt, &sum);
ARRAY_OP_NODE(MAngle, MAngle, SumAngle, &sum);
ARRAY_OP_NODE(MVector, MVector, SumVector, &sum);

ARRAY_OP_NODE(double, double, MaxElement, &max_array_element);
ARRAY_OP_NODE(int, int, MaxIntElement, &max_array_element);
ARRAY_OP_NODE(MAngle, MAngle, MaxAngleElement, &max_array_element);
ARRAY_OP_NODE(double, double, MinElement, &min_array_element);
ARRAY_OP_NODE(int, int, MinIntElement, &min_array_element);
ARRAY_OP_NODE(MAngle, MAngle, MinAngleElement, &min_array_element);


template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName,
    TOutAttrType (*TFuncPtr)(const std::vector<TInAttrType>&, const std::vector<double>&)>
class ArrayWeightedOpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(valueAttr_, "value", DefaultValue<TInAttrType>(), true);
        createAttribute(weightAttr_, "weight", 1.0, true);
        createCompoundAttribute(inputAttr_, {valueAttr_, weightAttr_}, "input", true, true);
        createAttribute(outputAttr_, "output", DefaultValue<TOutAttrType>(), false);
        
        MPxNode::addAttribute(inputAttr_);
        MPxNode::addAttribute(outputAttr_);
        
        MPxNode::attributeAffects(valueAttr_, outputAttr_);
        MPxNode::attributeAffects(weightAttr_, outputAttr_);
        MPxNode::attributeAffects(inputAttr_, outputAttr_);
        
        return MS::kSuccess;
    }
    
    MStatus compute(const MPlug& plug, MDataBlock& dataBlock) override
    {
        if (plug == outputAttr_ || (plug.isChild() && plug.parent() == outputAttr_))
        {
            const auto values = getAttribute<std::vector<TInAttrType>>(dataBlock, inputAttr_, valueAttr_);
            const auto weights = getAttribute<std::vector<double>>(dataBlock, inputAttr_, weightAttr_);
            
            setAttribute(dataBlock, outputAttr_, TFuncPtr(values, weights));
            
            return MS::kSuccess;
        }
        
        return MS::kUnknownParameter;
    }

private:
    static Attribute inputAttr_;
    static Attribute valueAttr_;
    static Attribute weightAttr_;
    static Attribute outputAttr_;
};

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName,
    TOutAttrType (*TFuncPtr)(const std::vector<TInAttrType>&, const std::vector<double>&)>
Attribute ArrayWeightedOpNode<TInAttrType, TOutAttrType, TClass, TTypeName, TFuncPtr>::inputAttr_;

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName,
    TOutAttrType (*TFuncPtr)(const std::vector<TInAttrType>&, const std::vector<double>&)>
Attribute ArrayWeightedOpNode<TInAttrType, TOutAttrType, TClass, TTypeName, TFuncPtr>::valueAttr_;

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName,
    TOutAttrType (*TFuncPtr)(const std::vector<TInAttrType>&, const std::vector<double>&)>
Attribute ArrayWeightedOpNode<TInAttrType, TOutAttrType, TClass, TTypeName, TFuncPtr>::weightAttr_;

template<typename TInAttrType, typename TOutAttrType, typename TClass, const char* TTypeName,
    TOutAttrType (*TFuncPtr)(const std::vector<TInAttrType>&, const std::vector<double>&)>
Attribute ArrayWeightedOpNode<TInAttrType, TOutAttrType, TClass, TTypeName, TFuncPtr>::outputAttr_;


#define ARRAY_WEIGHTED_OP_NODE(InAttrType, OutAttrType, NodeName, FuncPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public ArrayWeightedOpNode<InAttrType, OutAttrType, NodeName, name##NodeName, FuncPtr> {};

ARRAY_WEIGHTED_OP_NODE(double, double, WeightedAverage, &average);
ARRAY_WEIGHTED_OP_NODE(int, double, WeightedAverageInt, &average);
ARRAY_WEIGHTED_OP_NODE(MAngle, MAngle, WeightedAverageAngle, &average);
ARRAY_WEIGHTED_OP_NODE(MVector, MVector, WeightedAverageVector, &average);
ARRAY_WEIGHTED_OP_NODE(MMatrix, MMatrix, WeightedAverageMatrix, &average);
ARRAY_WEIGHTED_OP_NODE(MEulerRotation, MEulerRotation, WeightedAverageRotation, &average);
ARRAY_WEIGHTED_OP_NODE(MQuaternion, MQuaternion, WeightedAverageQuaternion, &average);


template<typename TAttrType, typename TClass, const char* TTypeName,
    std::vector<TAttrType> (*TFuncPtr)(const std::vector<TAttrType>&)>
class ArrayMapOpNode : public BaseNode<TClass, TTypeName>
{
public:
    static MStatus initialize()
    {
        createAttribute(inputAttr_, "input", DefaultValue<TAttrType>(), true, true);
        createAttribute(outputAttr_, "output", DefaultValue<TAttrType>(), false, true);
        
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

template<typename TAttrType, typename TClass, const char* TTypeName,
    std::vector<TAttrType> (*TFuncPtr)(const std::vector<TAttrType>&)>
Attribute ArrayMapOpNode<TAttrType, TClass, TTypeName, TFuncPtr>::inputAttr_;

template<typename TAttrType, typename TClass, const char* TTypeName,
    std::vector<TAttrType> (*TFuncPtr)(const std::vector<TAttrType>&)>
Attribute ArrayMapOpNode<TAttrType, TClass, TTypeName, TFuncPtr>::outputAttr_;

#define ARRAY_MAP_OP_NODE(AttrType, NodeName, FuncPtr) \
    TEMPLATE_PARAMETER_LINKAGE char name##NodeName[] = #NodeName; \
    class NodeName : public ArrayMapOpNode<AttrType, NodeName, name##NodeName, FuncPtr> {};

ARRAY_MAP_OP_NODE(double, NormalizeArray, &normalize);
ARRAY_MAP_OP_NODE(double, NormalizeWeightsArray, &normalizeWeights);
