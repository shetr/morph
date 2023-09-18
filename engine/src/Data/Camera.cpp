#include "Camera.hpp"

namespace Morph {

mat4 PerspectiveCamera3D::ToMat(float aspectRatio)
{
    return glm::perspective(fov, aspectRatio, near, far);
}

}