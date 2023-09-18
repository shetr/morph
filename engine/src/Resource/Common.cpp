#include "Common.hpp"

#include "Hardcoded.hpp"

namespace Morph {


CommonResources::CommonResources(GraphicsContext& context, const ResourceStorage& storage, GraphicsProgramCompiler& progCompiler)
    :
    m_cubeMesh(context, HardcodedResources::CubeData()),
    m_sphereMesh(context, HardcodedResources::SphereData()),
    m_quad3DMesh(context, HardcodedResources::Quad3DData()),
    m_quad2DMesh(context, HardcodedResources::Quad2DData()),
    m_screenQuad2DMesh(context, HardcodedResources::ScreenQuad2DData()),
    m_whiteTexture1D(context.CreateTexture1D(1, &m_white, TextureSizedFormat::RGBA8, m_textureSettings)),
    m_whiteTexture2D(context.CreateTexture2D(uvec2(1), &m_white, TextureSizedFormat::RGBA8, m_textureSettings)),
    m_whiteTexture3D(context.CreateTexture3D(uvec3(1), &m_white, TextureSizedFormat::RGBA8, m_textureSettings)),
    m_blackTexture1D(context.CreateTexture1D(1, &m_black, TextureSizedFormat::RGBA8, m_textureSettings)),
    m_blackTexture2D(context.CreateTexture2D(uvec2(1), &m_black, TextureSizedFormat::RGBA8, m_textureSettings)),
    m_blackTexture3D(context.CreateTexture3D(uvec3(1), &m_black, TextureSizedFormat::RGBA8, m_textureSettings)),
    m_phongForwardProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/PhongForward.glsl"))),
    m_PBRForwardProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/PBRForward.glsl"))),
    m_screenFillProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/ScreenFill.glsl"))),
    m_applyMaskProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/ApplyMask.glsl"))),
    m_blendProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/Blend.glsl"))),
    m_meshFillProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/MeshFill.glsl"))),
    m_meshFillTransProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/MeshFillTrans.glsl"))),
    m_meshNormalsProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/MeshNormals.glsl"))),
    m_meshTexCoordsProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/MeshTexCoords.glsl"))),
    m_meshDepthProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/MeshDepth.glsl"))),
    m_meshMaskProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/MeshMask.glsl"))),
    m_meshLightDirProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/MeshLightDir.glsl"))),
    m_meshTextureProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/MeshTexture.glsl"))),
    m_sphereTextureProgram(value_or_panic(progCompiler.CompileRenderProgramRes("engine:shaders/programs/SphereTexture.glsl")))
{    
}

}