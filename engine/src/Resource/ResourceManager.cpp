#include "ResourceManager.hpp"

#include <stb/stb_image_write.h>
#include <stb/stb_image.h>

#include <fstream>
#include <string>
#include <iomanip>
#include <tuple>
#include <algorithm>
#include <sstream>

namespace Morph {


opt<Image2D> ResourceManager::LoadImage2D_PNG(string filename)
{
    uvec2 dim = uvec2(0);
    TextureSizedFormat format = TextureSizedFormat::RGB8;
    int channels = 0;
    stbi_set_flip_vertically_on_load(true);
    const void* stbi_data = stbi_load(filename.c_str(), (int*)&dim.x, (int*)&dim.y, &channels, 0);
    if(channels == 1) {
        format = TextureSizedFormat::R8;
    } else if(channels == 2) {
        format = TextureSizedFormat::RG8;
    } else if(channels == 3) {
        format = TextureSizedFormat::RGB8;
    } else if(channels == 4) {
        format = TextureSizedFormat::RGBA8;
    }
    if(stbi_data) {
        vector<u8> data((u8*)stbi_data, (u8*)stbi_data + dim.x*dim.y*channels);
        stbi_image_free((stbi_uc*)stbi_data);
        return Image2D({dim, data, format});
    }
    return {};

}

void ResourceManager::SaveImage2D_PNG(const Image2D& image, string filename)
{
    TextureDataFormat dataFormat = GraphicsEnums::GetTextureDataFormat(image.format);
    TextureFormat format = dataFormat.format;
    int channels = static_cast<int>(format)+1;
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filename.c_str(), image.dim.x, image.dim.y, channels, image.data.data(), channels * image.dim.x);
}

void ResourceManager::SaveTexture2D_PNG(const Texture2D& texture, string filename)
{
    uvec2 dim = texture.dim();
    TextureFormat format = texture.GetFormat();
    int channels = static_cast<int>(format)+1;
    vector<u8> data(dim.x * dim.y * channels);
    texture.GetData(data.data());
    stbi_flip_vertically_on_write(true);
    stbi_write_png(filename.c_str(), dim.x, dim.y, channels, data.data(), channels * dim.x);
}

opt<IndexedVerticesMesh3D<u32>> ResourceManager::LoadMesh3D_OBJ(string filename)
{
    std::ifstream ifs(filename);
    if(!ifs) {
        return {};
    }
    vector<vec3> inVertices;
    vector<vec3> inNormals;
    vector<vec2> inUvs;

    IndexedVerticesMesh3D<u32> mesh;
    map<tuple<int, int, int>, int> indicesTrans;

    string line;
    while(std::getline(ifs, line)){
        size_t spacePos = line.find(" ");
        if (spacePos != std::string::npos) {
            string lineType = line.substr(0, spacePos);
            string lineValues = line.substr(spacePos + 1);
            //std::cout << lineType << " ";
            std::stringstream ss(lineValues);
            if(lineType == "v") {
                glm::vec3 v;
                ss >> v.x;
                ss >> v.y;
                ss >> v.z;
                //std::cout << v.x << " " << v.y << " " << v.z << std::endl;
                inVertices.push_back(v);
            } else if(lineType == "vn") {
                glm::vec3 n;
                ss >> n.x;
                ss >> n.y;
                ss >> n.z;
                //std::cout << n.x << " " << n.y << " " << n.z << std::endl;
                inNormals.push_back(n);
            } else if(lineType == "vt") {
                glm::vec2 uv;
                ss >> uv.x;
                ss >> uv.y;
                //std::cout << uv.x << " " << uv.y << std::endl;
                inUvs.push_back(uv);
            } else if(lineType == "f") {
                string lineTriangle;
                vector<int> idxs;
                while(std::getline(ss, lineTriangle, ' ')) {
                    std::stringstream triss(lineTriangle);
                    string lineTrianglePoint;
                    if(lineTriangle.size() > 1) {
                        vector<int> vertexIdxs;
                        while(std::getline(triss, lineTrianglePoint, '/')) {
                            if(!lineTrianglePoint.empty()) {
                                std::stringstream vss(lineTrianglePoint);
                                int i;
                                vss >> i;
                                vertexIdxs.push_back(i-1);
                                //std::cout << i << "/";
                            }
                        }
                        int i0 = (vertexIdxs.size() < 1 ? 0 : vertexIdxs[0]);
                        int i1 = (vertexIdxs.size() < 2 ? i0 : vertexIdxs[1]);
                        int i2 = (vertexIdxs.size() < 3 ? i0 : vertexIdxs[2]);
                        std::tuple<int, int, int> vertexIdx = {i0, i1, i2};
                        int i;
                        auto it = indicesTrans.find(vertexIdx);
                        if(it != indicesTrans.end()) {
                            i = it->second;
                        } else {
                            i = mesh.vertices.size();
                            mesh.vertices.push_back({
                                i0 < inVertices.size() ? inVertices[i0] : vec3(0),
                                i1 < inUvs.size() ? inUvs[i1] : vec2(0),
                                i2 < inNormals.size() ? inNormals[i2] : vec3(0),
                                vec3(0)
                            });
                        }
                        idxs.push_back(i);
                    }
                    //std::cout << " ";
                }
                if(idxs.size() >= 3) {
                    for(int i = 2; i < idxs.size(); i++) {
                        mesh.indices.push_back(idxs[0]);
                        mesh.indices.push_back(idxs[i-1]);
                        mesh.indices.push_back(idxs[i]);
                    }
                }
                //std::cout << std::endl;
            }
        }

    }
    
    return mesh;
}


void ResourceManager::SaveMesh3D_OBJ(const IndexedVerticesMesh3D<u32>& mesh, string filename)
{
    std::ofstream ofs(filename, std::ofstream::out);
    if(!ofs) {
        return;
    }

    ofs << std::fixed;
    
    for(const Mesh3DVertex& vertex: mesh.vertices) {
        vec3 pos = vertex.position;
        ofs << "v " << pos.x << " " << pos.y << " " << pos.z << "\n";
    }
    for(const Mesh3DVertex& vertex: mesh.vertices) {
        vec2 texCoord = vertex.texCoord;
        ofs << "vt " << texCoord.x << " " << texCoord.y << "\n";
    }
    for(const Mesh3DVertex& vertex: mesh.vertices) {
        vec3 normal = vertex.normal;
        ofs << "vn " << normal.x << " " << normal.y << " " << normal.z << "\n";
    }
    for(size_t i = 0; i < mesh.indices.size(); i+=3) {
        uint i0 = mesh.indices[i + 0];
        uint i1 = mesh.indices[i + 1];
        uint i2 = mesh.indices[i + 2];
        ofs << "f " <<i0<<"/"<<i0<<"/"<<i0<< " " <<i1<<"/"<<i1<<"/"<<i1<< " " <<i2<<"/"<<i2<<"/"<<i2<< "\n";
    }

    ofs.close();
}

struct NormalSum {
    vec3 normal = vec3(0);
    u32 count = 0;
};

IndexedVerticesMesh3D<u32> ResourceManager::RecalcualteNormals(IndexedVerticesMesh3D<u32> mesh)
{
    vector<NormalSum> normalSums(mesh.vertices.size());
    u32 currentTriangleVertexIndex = 0;
    for(u32 vertexIndex : mesh.indices) {
        u32 triangleIndex = 3 * (currentTriangleVertexIndex / 3);
        // front is CCW so normal should be facing out of the model
        vec3 v0v1 = mesh.vertices[triangleIndex+1].position - mesh.vertices[triangleIndex].position;
        vec3 v0v2 = mesh.vertices[triangleIndex+2].position - mesh.vertices[triangleIndex].position;
        vec3 normal = glm::cross(v0v1, v0v2);
        for(int i = 0; i < 3; ++i) {
            normalSums[triangleIndex+i].normal += normal;
            ++normalSums[triangleIndex+i].count;
        }
        ++currentTriangleVertexIndex;
    }

    u32 currentVertexIndex = 0;
    for(Mesh3DVertex& vertex : mesh.vertices) {
        vertex.normal = normalSums[currentVertexIndex].normal 
            / vec3(normalSums[currentVertexIndex].count);
        ++currentVertexIndex;
    }

    return mesh;
}

}