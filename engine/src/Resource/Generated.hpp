
#ifndef MORPH_GENERATED_RESOURCES_HPP
#define MORPH_GENERATED_RESOURCES_HPP

#include <Core/Core.hpp>
#include <Data/Mesh.hpp>

namespace Morph {

class GeneratedResources
{
public:
    static IndexedVerticesMesh3D<u32> Sphere(uint segmentation, float radius = 0.5, bool cosUvs = false);
    static IndexedVerticesMesh3D<u32> Sphere(uint meridianCount, uint parallelCount, float radius = 0.5, bool cosUvs = false);
    static IndexedVerticesMesh3D<u32> Cylinder(uint segmentation);
    static IndexedVerticesMesh3D<u32> Cone(uint segmentation);

private:
    static void MakeTriangle(u32* outIndices, u32 v0, u32 v1, u32 v2);
    static void MakeQuad(u32* outIndices, u32 v0, u32 v1, u32 v2, u32 v3);
};

}

#endif // MORPH_GENERATED_RESOURCES_HPP