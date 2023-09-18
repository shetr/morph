
#ifndef MORPH_RENDER_DATA_HPP
#define MORPH_RENDER_DATA_HPP

#include <Core/Core.hpp>
#include <Graphics/Context.hpp>

#include "VertexData.hpp"
#include "TextureUnits.hpp"

namespace Morph {

class RenderData
{
public:
    virtual VertexData& GetVertexData() const = 0;
};

}

#endif // MORPH_RENDER_DATA_HPP