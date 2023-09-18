
#ifndef MORPH_RESOURCE_MANAGER_HPP
#define MORPH_RESOURCE_MANAGER_HPP

#include <Core/Core.hpp>

#include <Graphics/Texture.hpp>
#include <Data/Mesh.hpp>
#include <Data/Image.hpp>

namespace Morph {

class ResourceManager
{
public:
    static opt<Image2D> LoadImage2D_PNG(string filename);
    static void SaveImage2D_PNG(const Image2D& image, string filename);
    static void SaveTexture2D_PNG(const Texture2D& texture, string filename);
    static opt<IndexedVerticesMesh3D<u32>> LoadMesh3D_OBJ(string filename);
    static void SaveMesh3D_OBJ(const IndexedVerticesMesh3D<u32>& mesh, string filename);
    static IndexedVerticesMesh3D<u32> RecalcualteNormals(IndexedVerticesMesh3D<u32> mesh);
};

}

#endif // MORPH_RESOURCE_MANAGER_HPP