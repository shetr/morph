#include "Hardcoded.hpp"

namespace Morph {


IndexedVerticesMesh3D<u32> HardcodedResources::s_cubeData = {
    vector<Mesh3DVertex>({ // FIXME: compute tangents
        Mesh3DVertex({vec3( 0.5, -0.5, -0.5), vec2(0, 0), vec3( 1,  0,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5,  0.5, -0.5), vec2(0, 1), vec3( 1,  0,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5,  0.5,  0.5), vec2(1, 1), vec3( 1,  0,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5, -0.5,  0.5), vec2(1, 0), vec3( 1,  0,  0), vec3( 0,  0,  0)}),

        Mesh3DVertex({vec3(-0.5, -0.5,  0.5), vec2(0, 0), vec3(-1,  0,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5,  0.5,  0.5), vec2(0, 1), vec3(-1,  0,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5,  0.5, -0.5), vec2(1, 1), vec3(-1,  0,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5, -0.5, -0.5), vec2(1, 0), vec3(-1,  0,  0), vec3( 0,  0,  0)}),

        Mesh3DVertex({vec3( 0.5, -0.5,  0.5), vec2(0, 0), vec3( 0,  0,  1), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5,  0.5,  0.5), vec2(0, 1), vec3( 0,  0,  1), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5,  0.5,  0.5), vec2(1, 1), vec3( 0,  0,  1), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5, -0.5,  0.5), vec2(1, 0), vec3( 0,  0,  1), vec3( 0,  0,  0)}),

        Mesh3DVertex({vec3(-0.5, -0.5, -0.5), vec2(0, 0), vec3( 0,  0, -1), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5,  0.5, -0.5), vec2(0, 1), vec3( 0,  0, -1), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5,  0.5, -0.5), vec2(1, 1), vec3( 0,  0, -1), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5, -0.5, -0.5), vec2(1, 0), vec3( 0,  0, -1), vec3( 0,  0,  0)}),

        Mesh3DVertex({vec3(-0.5,  0.5, -0.5), vec2(0, 0), vec3( 0,  1,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5,  0.5,  0.5), vec2(0, 1), vec3( 0,  1,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5,  0.5,  0.5), vec2(1, 1), vec3( 0,  1,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5,  0.5, -0.5), vec2(1, 0), vec3( 0,  1,  0), vec3( 0,  0,  0)}),

        Mesh3DVertex({vec3( 0.5, -0.5, -0.5), vec2(0, 0), vec3( 0, -1,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5, -0.5,  0.5), vec2(0, 1), vec3( 0, -1,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5, -0.5,  0.5), vec2(1, 1), vec3( 0, -1,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5, -0.5, -0.5), vec2(1, 0), vec3( 0, -1,  0), vec3( 0,  0,  0)})
    }),
    vector<u32>({
         0,  1,  2,  0,  2,  3,
         4,  5,  6,  4,  6,  7,
         8,  9, 10,  8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    })
};
IndexedVerticesMesh3D<u32> HardcodedResources::s_quad3DData = {
    vector<Mesh3DVertex>({ // FIXME: compute tangents
        Mesh3DVertex({vec3(-0.5, 0, -0.5), vec2(0, 0), vec3( 0,  1,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3(-0.5, 0,  0.5), vec2(0, 1), vec3( 0,  1,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5, 0,  0.5), vec2(1, 1), vec3( 0,  1,  0), vec3( 0,  0,  0)}),
        Mesh3DVertex({vec3( 0.5, 0, -0.5), vec2(1, 0), vec3( 0,  1,  0), vec3( 0,  0,  0)})
    }),
    vector<u32>({
         0,  1,  2,  0,  2,  3
    })
};
IndexedVerticesMesh2D<u32> HardcodedResources::s_quad2DData = {
    vector<Mesh2DVertex>({
        Mesh2DVertex({vec2(-0.5, -0.5), vec2(0, 0)}),
        Mesh2DVertex({vec2( 0.5, -0.5), vec2(1, 0)}),
        Mesh2DVertex({vec2( 0.5,  0.5), vec2(1, 1)}),
        Mesh2DVertex({vec2(-0.5,  0.5), vec2(0, 1)})
    }),
    vector<u32>({
         0,  1,  2,  0,  2,  3
    })
};
IndexedVerticesMesh2D<u32> HardcodedResources::s_screenQuad2DData = {
    vector<Mesh2DVertex>({
        Mesh2DVertex({vec2(-1, -1), vec2(0, 0)}),
        Mesh2DVertex({vec2( 1, -1), vec2(1, 0)}),
        Mesh2DVertex({vec2( 1,  1), vec2(1, 1)}),
        Mesh2DVertex({vec2(-1,  1), vec2(0, 1)})
    }),
    vector<u32>({
         0,  1,  2,  0,  2,  3
    })
};

}