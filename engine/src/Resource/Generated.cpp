#include "Generated.hpp"

namespace Morph {


IndexedVerticesMesh3D<u32> GeneratedResources::Sphere(uint segmentation, float radius, bool cosUvs)
{
    return GeneratedResources::Sphere(segmentation, segmentation, radius, cosUvs);
}
IndexedVerticesMesh3D<u32> GeneratedResources::Sphere(uint meridianCount, uint parallelCount, float radius, bool cosUvs)
{
    int phiCount = meridianCount + 2; // horizontal angle
    int thetaCount = parallelCount + 2; // vertical angle
    int vertexCount = thetaCount*phiCount;
    int quadIndicesCount = 6*(parallelCount - 1)*phiCount;
    int triIndicesCount = 3*phiCount;
    int indicesCount = quadIndicesCount + 2*triIndicesCount;
    int startQuadsIndices = triIndicesCount;
    int startUpTriIndices = startQuadsIndices + quadIndicesCount;
    
    IndexedVerticesMesh3D<u32> mesh {
        vector<Mesh3DVertex>(vertexCount),
        vector<u32>(indicesCount)
    };

    for(int t = 0; t < thetaCount; t++) {
        float tRatio = ((float)t) / (thetaCount-1);
        float theta = glm::pi<float>() * tRatio;
        float sinTheta = glm::sin(theta);
        float mCosTheta = -glm::cos(theta);
        float uvY = cosUvs ? (mCosTheta + 1) / 2 : tRatio;
        for(int p = 0; p < phiCount; p++) {
            Mesh3DVertex& vertex = mesh.vertices[p + phiCount*t];

            float pRatio = ((float)p) / (phiCount-1);
            float phi = 2 * glm::pi<float>() * pRatio;
            float sinPhi = glm::sin(phi);
            float cosPhi = glm::cos(phi);

            vec3 normal = vec3(sinTheta * cosPhi, mCosTheta, sinTheta * sinPhi);

            vertex.position = radius * normal;
            vertex.normal = normal;
            vertex.texCoord = vec2(pRatio, uvY); // FIXME: weird uv
            vertex.tangent = vec3(0); // FIXME: compute tangent
        }
    }
    for(int v = 0; v < phiCount; v++) { // bottom triangles
        int i = 3*v;
        MakeTriangle(&mesh.indices[i], v, v + phiCount, (v + 1) % phiCount + phiCount);
    }
    for(int t = 1; t < thetaCount - 2; t++) { // quad indices
        for(int p = 0; p < phiCount; p++) {
            int v = p + phiCount*t;
            int vp1 = (p + 1)% phiCount + phiCount*t;
            int i = (v - phiCount) * 6 + startQuadsIndices;
            MakeQuad(&mesh.indices[i], v, v + phiCount, vp1 + phiCount, vp1);
        }
    }
    for(int p = 0; p < phiCount; p++) { // top triangles
        int i = startUpTriIndices + 3*p;
        int v = (thetaCount - 2)*phiCount + p;
        int vp1 = (thetaCount - 2)*phiCount + (p + 1) % phiCount;
        MakeTriangle(&mesh.indices[i], vp1, v, v + phiCount);
    }
    return mesh;
}
IndexedVerticesMesh3D<u32> GeneratedResources::Cylinder(uint segmentation) // FIXME: no sharp edges
{
    const float radius = 0.5;
    int discVertexCount = segmentation + 1;
    int vertexCount = 4*discVertexCount;
    int triDiscIndicesCount = 3*discVertexCount;
    int quadIndicesCount = 6*discVertexCount;
    int indicesCount = 2*triDiscIndicesCount + quadIndicesCount;

    int startQuads = discVertexCount;
    int startUpTri = 3 * discVertexCount;
    int startQuadsIndices = triDiscIndicesCount;
    int startUpTriIndices = startQuadsIndices + quadIndicesCount;

    IndexedVerticesMesh3D<u32> mesh {
        vector<Mesh3DVertex>(vertexCount),
        vector<u32>(indicesCount)
    };

    for(int p = 0; p < discVertexCount; p++) { // bottom triangles
        Mesh3DVertex& vertex = mesh.vertices[p];
        
        vertex.position = vec3(0, -0.5, 0);
        vertex.normal = vec3(0, -1, 0);
        vertex.texCoord = vec2(0.5, 0); // FIXME: weird uv
        vertex.tangent = vec3(0); // FIXME: compute tangent

        // indices 
        int i = 3*p;
        MakeTriangle(&mesh.indices[i], p, p + discVertexCount, (p + 1) % discVertexCount + discVertexCount);
    }
    for(int t = 0; t < 2; t++) { // quads
        float h = 2*t - 1.0f;
        float y = h * radius;
        for(int p = 0; p < discVertexCount; p++) {
            Mesh3DVertex& vertex = mesh.vertices[p + t * discVertexCount + startQuads];
            float pRatio = ((float)p) / (discVertexCount-1);
            float phi = 2 * glm::pi<float>() * pRatio;
            vec3 normal = glm::vec3(glm::cos(phi), 0, glm::sin(phi));
            
            vertex.position = radius * normal + vec3(0, y, 0);
            vertex.normal = normal;
            vertex.texCoord = vec2(pRatio, t); // FIXME: weird uv
            vertex.tangent = vec3(0); // FIXME: compute tangent

            // indices
            if(t == 0) {
                int v = p + discVertexCount;
                int vp1 = (p + 1) % discVertexCount + discVertexCount;
                int i = (v - discVertexCount) * 6 + startQuadsIndices;
                MakeQuad(&mesh.indices[i], v, v + discVertexCount, vp1 + discVertexCount, vp1);
            }
        }
    }
    for(int p = 0; p < discVertexCount; p++) { // top triangles
        Mesh3DVertex& vertex = mesh.vertices[p + startUpTri];
        
        vertex.position = vec3(0, 0.5, 0);
        vertex.normal = vec3(0, 1, 0);
        vertex.texCoord = vec2(0.5, 1); // FIXME: weird uv
        vertex.tangent = vec3(0); // FIXME: compute tangent
        
        // indices
        int i = startUpTriIndices + 3*p;
        int v = 2*discVertexCount + p;
        int vp1 = 2*discVertexCount + (p + 1) % discVertexCount;
        MakeTriangle(&mesh.indices[i], vp1, v, v + discVertexCount);
    }

    return mesh;
}
IndexedVerticesMesh3D<u32> GeneratedResources::Cone(uint segmentation) // FIXME: no sharp edges
{
    const float radius = 0.5;
    int discVertexCount = segmentation + 1;
    int vertexCount = 3*discVertexCount;
    int triDiscIndicesCount = 3*discVertexCount;
    int indicesCount = 2*triDiscIndicesCount;

    int startUpTri = 2 * discVertexCount;
    int startUpTriIndices = triDiscIndicesCount;

    IndexedVerticesMesh3D<u32> mesh {
        vector<Mesh3DVertex>(vertexCount),
        vector<u32>(indicesCount)
    };

    for(int p = 0; p < discVertexCount; p++) { // bottom triangles
        Mesh3DVertex& vertex = mesh.vertices[p];
        
        vertex.position = vec3(0, -0.5, 0);
        vertex.normal = vec3(0, -1, 0);
        vertex.texCoord = vec2(0.5, 0); // FIXME: weird uv
        vertex.tangent = vec3(0); // FIXME: compute tangent

        // indices 
        int i = 3*p;
        MakeTriangle(&mesh.indices[i], p, p + discVertexCount, (p + 1) % discVertexCount + discVertexCount);
    }
    for(int p = 0; p < discVertexCount; p++) {
        Mesh3DVertex& vertex = mesh.vertices[p + discVertexCount];
        float pRatio = ((float)p) / (discVertexCount-1);
        float phi = 2 * glm::pi<float>() * pRatio;
        vec3 normal = glm::vec3(glm::cos(phi), 0, glm::sin(phi));
        
        vertex.position = radius * normal + vec3(0, -radius, 0);
        vertex.normal = normal;
        vertex.texCoord = vec2(pRatio, 0); // FIXME: weird uv
        vertex.tangent = vec3(0); // FIXME: compute tangent
    }
    for(int p = 0; p < discVertexCount; p++) { // top triangles
        Mesh3DVertex& vertex = mesh.vertices[p + startUpTri];
        
        vertex.position = vec3(0, 0.5, 0);
        vertex.normal = vec3(0, 1, 0);
        vertex.texCoord = vec2(0.5, 1); // FIXME: weird uv
        vertex.tangent = vec3(0); // FIXME: compute tangent
        
        // indices
        int i = startUpTriIndices + 3*p;
        int v = discVertexCount + p;
        int vp1 = discVertexCount + (p + 1) % discVertexCount;
        MakeTriangle(&mesh.indices[i], vp1, v, v + discVertexCount);
    }

    return mesh;
}



void GeneratedResources::MakeTriangle(u32* outIndices, u32 v0, u32 v1, u32 v2)
{
    outIndices[0] = v0;
    outIndices[1] = v1;
    outIndices[2] = v2;
}
void GeneratedResources::MakeQuad(u32* outIndices, u32 v0, u32 v1, u32 v2, u32 v3)
{
    outIndices[0] = v0;
    outIndices[1] = v1;
    outIndices[2] = v2;
    outIndices[3] = v0;
    outIndices[4] = v2;
    outIndices[5] = v3;
}

}