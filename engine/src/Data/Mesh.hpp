
#ifndef MORPH_MESH_DATA_HPP
#define MORPH_MESH_DATA_HPP

#include <Core/Core.hpp>

#include "VertexData.hpp"

namespace Morph {

struct Mesh2DVertex
{
    vec2 position;
    vec2 texCoord;

    inline static const VertexLayout& GetLayout() {
        static VertexLayout vetexLayout;
        if(vetexLayout.ElementsCount() == 0) {
            vetexLayout.AddElement({ VertexType::FLOAT32, VertexElementCount::_2 });
            vetexLayout.AddElement({ VertexType::FLOAT32, VertexElementCount::_2 });
        }
        return vetexLayout;
    }
};

struct Mesh3DVertex
{
    vec3 position;
    vec2 texCoord;
    vec3 normal;
    vec3 tangent;

    inline static const VertexLayout& GetLayout() {
        static VertexLayout vetexLayout;
        if(vetexLayout.ElementsCount() == 0) {
            vetexLayout.AddElement({ VertexType::FLOAT32, VertexElementCount::_3 });
            vetexLayout.AddElement({ VertexType::FLOAT32, VertexElementCount::_2 });
            vetexLayout.AddElement({ VertexType::FLOAT32, VertexElementCount::_3 });
            vetexLayout.AddElement({ VertexType::FLOAT32, VertexElementCount::_3 });
        }
        return vetexLayout;
    }
};

template<typename INDEX_TYPE = u32>
using IndexedVerticesMesh2D = IndexedVertices<Mesh2DVertex, INDEX_TYPE>;
template<typename INDEX_TYPE = u32>
using IndexedVerticesMesh3D = IndexedVertices<Mesh3DVertex, INDEX_TYPE>;

using BasicStaticMesh2D = BasicStaticVertexData<PrimitiveType::TRIANGLES, Mesh2DVertex>;
using BasicStaticMesh3D = BasicStaticVertexData<PrimitiveType::TRIANGLES, Mesh3DVertex>;

template<typename INDEX_TYPE = u32>
using IndexedStaticMesh2D = IndexedStaticVertexData<PrimitiveType::TRIANGLES, Mesh2DVertex, INDEX_TYPE>;
template<typename INDEX_TYPE = u32>
using IndexedStaticMesh3D = IndexedStaticVertexData<PrimitiveType::TRIANGLES, Mesh3DVertex, INDEX_TYPE>;


}

#endif // MORPH_MESH_DATA_HPP