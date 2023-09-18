#include "Transform.hpp"

namespace Morph {

mat4 TransformPosScaleRot::ToMat()
{
    return GetTranslation() * GetRotation() * GetScale();
}
mat4 TransformPosScaleRot::ToInv()
{
    return GetScaleInv() * GetRotationInv() * GetTranslationInv();
}


mat4 TransformNode::worldTransform()
{
    if(m_parent.has_value()) {
        TransformNode& parent = m_parent.value();
        return parent.worldTransform() * m_transform->ToMat();
    }
    return m_transform->ToMat();
}
mat4 TransformNode::worldTransformInv()
{
    if(m_parent.has_value()) {
        TransformNode& parent = m_parent.value();
        return m_transform->ToInv() * parent.worldTransform();
    }
    return m_transform->ToInv();
}

mat4 TransformPosScaleNoRot::GetTranslation()
{
    return glm::translate(pos);
}
mat4 TransformPosScaleNoRot::GetTranslationInv()
{
    return glm::translate(-pos);
}
mat4 TransformPosScaleNoRot::GetScale()
{
    return glm::scale(scale);
}
mat4 TransformPosScaleNoRot::GetScaleInv()
{
    return glm::scale(vec3(1) / scale);
}
mat4 TransformPosScaleNoRot::GetRotation()
{
    return glm::identity<mat4>();
}
mat4 TransformPosScaleNoRot::GetRotationInv()
{
    return glm::identity<mat4>();
}

mat4 TransformRotAngleAxis::GetRotation()
{
    return glm::rotate(rotAngle, rotAxis);
}
mat4 TransformRotAngleAxis::GetRotationInv()
{
    return glm::rotate(-rotAngle, rotAxis);
}

mat4 TransformRotEuler::GetRotation()
{
    return glm::yawPitchRoll(rot.y, rot.x, rot.z);
}
mat4 TransformRotEuler::GetRotationInv()
{
    return glm::transpose(glm::yawPitchRoll(rot.y, rot.x, rot.z));
}

mat4 TransformRotQuat::GetRotation()
{
    return glm::toMat4(rot);
}
mat4 TransformRotQuat::GetRotationInv()
{
    return glm::toMat4(glm::inverse(rot));
}

}