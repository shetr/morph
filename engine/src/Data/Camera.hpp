
#ifndef MORPH_CAMERA_HPP
#define MORPH_CAMERA_HPP

#include <Core/Core.hpp>

namespace Morph {

struct Camera2D
{

    virtual mat3 ToMat(float aspectRatio) = 0;
};

struct OrthographicCamera2D : public Camera2D
{

};

struct Camera3D
{

    virtual mat4 ToMat(float aspectRatio) = 0;
};

struct PerspectiveCamera3D : public Camera3D
{
    float fov = glm::pi<float>() / 2.0f;
    float near = 0.1;
    float far = 100;

    mat4 ToMat(float aspectRatio) override;
};

struct OrthographicCamera3D : public Camera3D
{

};

}

#endif // MORPH_CAMERA_HPP