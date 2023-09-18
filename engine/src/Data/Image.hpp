
#ifndef MORPH_IMAGE_HPP
#define MORPH_IMAGE_HPP

#include <Core/Core.hpp>
#include <Graphics/Enums.hpp>

namespace Morph {

struct Image1D
{
    uint dim;
    vector<u8> data;
    TextureSizedFormat format;
};

struct Image2D
{
    uvec2 dim;
    vector<u8> data;
    TextureSizedFormat format;
};

struct Image3D
{
    uvec3 dim;
    vector<u8> data;
    TextureSizedFormat format;
};

}

#endif // MORPH_IMAGE_HPP