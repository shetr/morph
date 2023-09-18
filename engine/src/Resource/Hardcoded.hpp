
#ifndef MORPH_HARDCODED_RESOURCES_HPP
#define MORPH_HARDCODED_RESOURCES_HPP

#include <Core/Core.hpp>
#include <Data/Mesh.hpp>

namespace Morph {

class HardcodedResources
{
private:
    static IndexedVerticesMesh3D<u32> s_cubeData;
    static IndexedVerticesMesh3D<u32> s_sphereData;  // FIXME: compute tangents
    static IndexedVerticesMesh3D<u32> s_quad3DData;
    static IndexedVerticesMesh2D<u32> s_quad2DData;
    static IndexedVerticesMesh2D<u32> s_screenQuad2DData;
public:
    static const IndexedVerticesMesh3D<u32>& CubeData() { return s_cubeData; }
    static const IndexedVerticesMesh3D<u32>& SphereData() { return s_sphereData; }
    static const IndexedVerticesMesh3D<u32>& Quad3DData() { return s_quad3DData; }
    static const IndexedVerticesMesh2D<u32>& Quad2DData() { return s_quad2DData; }
    static const IndexedVerticesMesh2D<u32>& ScreenQuad2DData() { return s_screenQuad2DData; }
};

}

#endif // MORPH_HARDCODED_RESOURCES_HPP