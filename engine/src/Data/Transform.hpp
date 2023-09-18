
#ifndef MORPH_TRANSFORM_HPP
#define MORPH_TRANSFORM_HPP

#include <Core/Core.hpp>

namespace Morph {

struct Transform
{
    virtual mat4 ToMat() = 0;
    virtual mat4 ToInv() = 0;
};

class TransformNode
{
private:
    ref<Transform> m_transform;
    opt<ref<TransformNode>> m_parent;
public:
    TransformNode(Transform& transform) : m_transform(transform) {}
    TransformNode(Transform& transform, opt<ref<TransformNode>> parent)
        : m_transform(transform), m_parent(parent) {}

    inline void SetParent(opt<ref<TransformNode>> parent) { m_parent = parent; }

    mat4 worldTransform();
    mat4 worldTransformInv();
};

struct TransformPosScaleRot : public Transform
{
    mat4 ToMat() override;
    mat4 ToInv() override;

    virtual mat4 GetTranslation() = 0;
    virtual mat4 GetTranslationInv() = 0;
    virtual mat4 GetScale() = 0;
    virtual mat4 GetScaleInv() = 0;
    virtual mat4 GetRotation() = 0;
    virtual mat4 GetRotationInv() = 0;
};

struct TransformPosScaleNoRot : public TransformPosScaleRot
{
    vec3 pos = vec3(0);
    vec3 scale = vec3(1);

    mat4 GetTranslation() override;
    mat4 GetTranslationInv() override;
    mat4 GetScale() override;
    mat4 GetScaleInv() override;
    virtual mat4 GetRotation() override;
    virtual mat4 GetRotationInv() override;
};

struct TransformRotAngleAxis : public TransformPosScaleNoRot
{
    float rotAngle = 0;
    vec3 rotAxis  = vec3(0, 0, 1);
    
    mat4 GetRotation() override;
    mat4 GetRotationInv() override;
};

struct TransformRotEuler : public TransformPosScaleNoRot
{
    vec3 rot = vec3(0);
    
    mat4 GetRotation() override;
    mat4 GetRotationInv() override;
};

struct TransformRotQuat : public TransformPosScaleNoRot
{
    quat rot = quat();
    
    mat4 GetRotation() override;
    mat4 GetRotationInv() override;
};

}

#endif // MORPH_TRANSFORM_HPP