
#ifndef MORPH_COMMON_RESOURCES_HPP
#define MORPH_COMMON_RESOURCES_HPP

#include <Core/Core.hpp>
#include <Graphics/Context.hpp>
#include <Data/Mesh.hpp>

#include "Storage.hpp"
#include "GraphicsProgramCompiler.hpp"

namespace Morph {

class CommonResources
{
private:
    u32 m_white = 0xFFFFFFFF;
    u32 m_black = 0;
    TextureSettings m_textureSettings = {
        TextureMinFilterSetting(TextureMinFilter::NEAREST),
        TextureMagFilterSetting(TextureMagFilter::NEAREST),
        TextureWrap2DSetting(TextureWrapType::REPEAT)
    };
    IndexedStaticMesh3D<u32> m_cubeMesh;
    IndexedStaticMesh3D<u32> m_sphereMesh;
    IndexedStaticMesh3D<u32> m_quad3DMesh;
    IndexedStaticMesh2D<u32> m_quad2DMesh;
    IndexedStaticMesh2D<u32> m_screenQuad2DMesh;
    Texture1D m_whiteTexture1D;
    Texture2D m_whiteTexture2D;
    Texture3D m_whiteTexture3D;
    Texture1D m_blackTexture1D;
    Texture2D m_blackTexture2D;
    Texture3D m_blackTexture3D;
    RenderProgram m_phongForwardProgram;
    RenderProgram m_PBRForwardProgram;
    RenderProgram m_screenFillProgram;
    RenderProgram m_applyMaskProgram;
    RenderProgram m_blendProgram;
    RenderProgram m_meshFillProgram;
    RenderProgram m_meshFillTransProgram;
    RenderProgram m_meshNormalsProgram;
    RenderProgram m_meshTexCoordsProgram;
    RenderProgram m_meshDepthProgram;
    RenderProgram m_meshMaskProgram;
    RenderProgram m_meshLightDirProgram;
    RenderProgram m_meshTextureProgram;
    RenderProgram m_sphereTextureProgram;

public:
    CommonResources(GraphicsContext& context, const ResourceStorage& storage, GraphicsProgramCompiler& progCompiler);
    IndexedStaticMesh3D<u32>& CubeMesh() { return m_cubeMesh; }
    IndexedStaticMesh3D<u32>& SphereMesh() { return m_sphereMesh; }
    IndexedStaticMesh3D<u32>& Quad3DMesh() { return m_quad3DMesh; }
    IndexedStaticMesh2D<u32>& Quad2DMesh() { return m_quad2DMesh; }
    IndexedStaticMesh2D<u32>& ScreenQuad2DMesh() { return m_screenQuad2DMesh; }
    Texture1D& WhiteTexture1D() { return m_whiteTexture1D; }
    Texture2D& WhiteTexture2D() { return m_whiteTexture2D; }
    Texture3D& WhiteTexture3D() { return m_whiteTexture3D; }
    Texture1D& BlackTexture1D() { return m_blackTexture1D; }
    Texture2D& BlackTexture2D() { return m_blackTexture2D; }
    Texture3D& BlackTexture3D() { return m_blackTexture3D; }
    RenderProgram& PhongForwardProgram() { return m_phongForwardProgram; }
    RenderProgram& PBRForwardProgram() { return m_PBRForwardProgram; }
    RenderProgram& ScreenFillProgram() { return m_screenFillProgram; }
    RenderProgram& ApplyMaskProgram() { return m_applyMaskProgram; }
    RenderProgram& BlendProgram() { return m_blendProgram; }
    RenderProgram& MeshFillProgram() { return m_meshFillProgram; }
    RenderProgram& MeshFillTransProgram() { return m_meshFillTransProgram; }
    RenderProgram& MeshNormalsProgram() { return m_meshNormalsProgram; }
    RenderProgram& MeshTexCoordsProgram() { return m_meshTexCoordsProgram; }
    RenderProgram& MeshDepthProgram() { return m_meshDepthProgram; }
    RenderProgram& MeshMaskProgram() { return m_meshMaskProgram; }
    RenderProgram& MeshLightDirProgram() { return m_meshLightDirProgram; }
    RenderProgram& MeshTextureProgram() { return m_meshTextureProgram; }
    RenderProgram& SphereTextureProgram() { return m_sphereTextureProgram; }
};

}

#endif // MORPH_COMMON_RESOURCES_HPP