#include "Enums.hpp"

#include "OpenGL.hpp"

namespace Morph {

unord_map<TextureSizedFormat, TextureDataFormat> GraphicsEnums::s_sizedFormatToDataFormat = {
    { TextureSizedFormat::R8, { TextureFormat::R, TextureDataType::UINT8 } },
    { TextureSizedFormat::R8_S, { TextureFormat::R, TextureDataType::INT8 } },
    { TextureSizedFormat::R16, { TextureFormat::R, TextureDataType::UINT16 } },
    { TextureSizedFormat::R16_S, { TextureFormat::R, TextureDataType::INT16 } },
    { TextureSizedFormat::RG8, { TextureFormat::RG, TextureDataType::UINT8 } },
    { TextureSizedFormat::RG8_S, { TextureFormat::RG, TextureDataType::INT8 } },
    { TextureSizedFormat::RG16, { TextureFormat::RG, TextureDataType::UINT16 } },
    { TextureSizedFormat::RG16_S, { TextureFormat::RG, TextureDataType::INT16 } },
    { TextureSizedFormat::R3_G3_B2, { TextureFormat::RGB, TextureDataType::UINT_3_3_2 } },
    { TextureSizedFormat::RGB8, { TextureFormat::RGB, TextureDataType::UINT8 } },
    { TextureSizedFormat::RGB8_S, { TextureFormat::RGB, TextureDataType::INT8 } },
    { TextureSizedFormat::RGB16_S, { TextureFormat::RGB, TextureDataType::INT16 } },
    { TextureSizedFormat::RGBA4, { TextureFormat::RGBA, TextureDataType::UINT_4_4_4_4 } },
    { TextureSizedFormat::RGB5_A1, { TextureFormat::RGBA, TextureDataType::UINT_5_5_5_1 } },
    { TextureSizedFormat::RGBA8, { TextureFormat::RGBA, TextureDataType::UINT8 } },
    { TextureSizedFormat::RGBA8_S, { TextureFormat::RGBA, TextureDataType::INT8 } },
    { TextureSizedFormat::RGB10_A2, { TextureFormat::RGBA, TextureDataType::UINT_10_10_10_2 } },
    { TextureSizedFormat::RGB10_A2UI, { TextureFormat::RGBA_INT, TextureDataType::UINT_10_10_10_2 } },
    { TextureSizedFormat::RGBA16, { TextureFormat::RGBA, TextureDataType::UINT16 } },
    { TextureSizedFormat::SRGB8, { TextureFormat::RGB, TextureDataType::UINT8 } },
    { TextureSizedFormat::SRGB8_A8, { TextureFormat::RGBA, TextureDataType::UINT8 } },
    { TextureSizedFormat::R16F, { TextureFormat::R, TextureDataType::FLOAT16 } },
    { TextureSizedFormat::RG16F, { TextureFormat::RG, TextureDataType::FLOAT16 } },
    { TextureSizedFormat::RGB16F, { TextureFormat::RGB, TextureDataType::FLOAT16 } },
    { TextureSizedFormat::RGBA16F, { TextureFormat::RGBA, TextureDataType::FLOAT16 } },
    { TextureSizedFormat::R32F, { TextureFormat::R, TextureDataType::FLOAT32 } },
    { TextureSizedFormat::RG32F, { TextureFormat::RG, TextureDataType::FLOAT32 } },
    { TextureSizedFormat::RGB32F, { TextureFormat::RGB, TextureDataType::FLOAT32 } },
    { TextureSizedFormat::RGBA32F, { TextureFormat::RGBA, TextureDataType::FLOAT32 } },
    { TextureSizedFormat::R8I, { TextureFormat::R_INT, TextureDataType::INT8 } },
    { TextureSizedFormat::R8UI, { TextureFormat::R_INT, TextureDataType::UINT8 } },
    { TextureSizedFormat::R16I, { TextureFormat::R_INT, TextureDataType::INT16 } },
    { TextureSizedFormat::R16UI, { TextureFormat::R_INT, TextureDataType::UINT16 } },
    { TextureSizedFormat::R32I, { TextureFormat::R_INT, TextureDataType::INT32 } },
    { TextureSizedFormat::R32UI, { TextureFormat::R_INT, TextureDataType::UINT32 } },
    { TextureSizedFormat::RG8I, { TextureFormat::RG_INT, TextureDataType::INT8 } },
    { TextureSizedFormat::RG8UI, { TextureFormat::RG_INT, TextureDataType::UINT8 } },
    { TextureSizedFormat::RG16I, { TextureFormat::RG_INT, TextureDataType::INT16 } },
    { TextureSizedFormat::RG16UI, { TextureFormat::RG_INT, TextureDataType::UINT16 } },
    { TextureSizedFormat::RG32I, { TextureFormat::RG_INT, TextureDataType::INT32 } },
    { TextureSizedFormat::RG32UI, { TextureFormat::RG_INT, TextureDataType::UINT32 } },
    { TextureSizedFormat::RGB8I, { TextureFormat::RGB_INT, TextureDataType::INT8 } },
    { TextureSizedFormat::RGB8UI, { TextureFormat::RGB_INT, TextureDataType::UINT8 } },
    { TextureSizedFormat::RGB16I, { TextureFormat::RGB_INT, TextureDataType::INT16 } },
    { TextureSizedFormat::RGB16UI, { TextureFormat::RGB_INT, TextureDataType::UINT16 } },
    { TextureSizedFormat::RGB32I, { TextureFormat::RGB_INT, TextureDataType::INT32 } },
    { TextureSizedFormat::RGB32UI, { TextureFormat::RGB_INT, TextureDataType::UINT32 } },
    { TextureSizedFormat::RGBA8I, { TextureFormat::RGBA_INT, TextureDataType::INT8 } },
    { TextureSizedFormat::RGBA8UI, { TextureFormat::RGBA_INT, TextureDataType::UINT8 } },
    { TextureSizedFormat::RGBA16I, { TextureFormat::RGBA_INT, TextureDataType::INT16 } },
    { TextureSizedFormat::RGBA16UI, { TextureFormat::RGBA_INT, TextureDataType::UINT16 } },
    { TextureSizedFormat::RGBA32I, { TextureFormat::RGBA_INT, TextureDataType::INT32 } },
    { TextureSizedFormat::RGBA32UI, { TextureFormat::RGBA_INT, TextureDataType::UINT32 } },
    { TextureSizedFormat::DEPTH16, { TextureFormat::DEPTH, TextureDataType::UINT16 } },
    { TextureSizedFormat::DEPTH24, { TextureFormat::DEPTH, TextureDataType::UINT32 } },
    { TextureSizedFormat::DEPTH32, { TextureFormat::DEPTH, TextureDataType::UINT32 } },
    { TextureSizedFormat::DEPTH32F, { TextureFormat::DEPTH, TextureDataType::FLOAT32 } },
    { TextureSizedFormat::DEPTH24_STENCIL8, { TextureFormat::DEPTH_STENCIL, TextureDataType::UINT_24_8 } },
    { TextureSizedFormat::DEPTH32F_STENCIL8, { TextureFormat::DEPTH_STENCIL, TextureDataType::FLOAT_32_UINT_24_8_REV } },
    { TextureSizedFormat::STENCIL1, { TextureFormat::STENCIL, TextureDataType::UINT8 } },
    { TextureSizedFormat::STENCIL4, { TextureFormat::STENCIL, TextureDataType::UINT8 } },
    { TextureSizedFormat::STENCIL8, { TextureFormat::STENCIL, TextureDataType::UINT8 } },
    { TextureSizedFormat::STENCIL16, { TextureFormat::STENCIL, TextureDataType::UINT16 } }
};

EnumMap<GraphicsBufferTarget> GraphicsEnums::bufferTargets = {
    GL_ARRAY_BUFFER,
    GL_ATOMIC_COUNTER_BUFFER,
    GL_COPY_READ_BUFFER,
    GL_COPY_WRITE_BUFFER,
    GL_DISPATCH_INDIRECT_BUFFER,
    GL_DRAW_INDIRECT_BUFFER,
    GL_ELEMENT_ARRAY_BUFFER,
    GL_PIXEL_PACK_BUFFER,
    GL_PIXEL_UNPACK_BUFFER,
    GL_QUERY_BUFFER,
    GL_SHADER_STORAGE_BUFFER,
    GL_TEXTURE_BUFFER,
    GL_TRANSFORM_FEEDBACK_BUFFER,
    GL_UNIFORM_BUFFER
};
EnumMap<GraphicsAccess> GraphicsEnums::accessValues = {
    GL_READ_ONLY,
    GL_WRITE_ONLY,
    GL_READ_WRITE
};
EnumMap<VertexType> GraphicsEnums::vertexTypes = {
    GL_BYTE,
    GL_SHORT,
    GL_INT,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT,
    GL_HALF_FLOAT,
    GL_FLOAT,
    GL_DOUBLE,
    GL_FIXED
};
EnumMap<VertexPackedType> GraphicsEnums::vertexPackedTypes = {
    GL_INT_2_10_10_10_REV,
    GL_UNSIGNED_INT_2_10_10_10_REV,
    GL_UNSIGNED_INT_10F_11F_11F_REV
};
EnumMap<IndexType> GraphicsEnums::indexTypes = {
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_SHORT,
    GL_UNSIGNED_INT
};
EnumMap<ShaderType> GraphicsEnums::shaderTypes = {
    GL_FRAGMENT_SHADER,
    GL_VERTEX_SHADER,
    GL_GEOMETRY_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_COMPUTE_SHADER
};
EnumMap<PrimitiveAllType> GraphicsEnums::primitiveAllTypes = {
    GL_POINTS, 
    GL_LINE_STRIP, 
    GL_LINE_LOOP, 
    GL_LINES, 
    GL_TRIANGLE_STRIP, 
    GL_TRIANGLE_FAN, 
    GL_TRIANGLES, 
    GL_PATCHES,
    GL_LINE_STRIP_ADJACENCY, 
    GL_LINES_ADJACENCY, 
    GL_TRIANGLE_STRIP_ADJACENCY, 
    GL_TRIANGLES_ADJACENCY
};
EnumMap<PrimitiveType> GraphicsEnums::primitiveTypes = {
    GL_POINTS, 
    GL_LINE_STRIP, 
    GL_LINE_LOOP, 
    GL_LINES, 
    GL_TRIANGLE_STRIP, 
    GL_TRIANGLE_FAN, 
    GL_TRIANGLES, 
    GL_PATCHES
};
EnumMap<PrimitiveAdjacencyType> GraphicsEnums::primitiveAdjacencyTypes = {
    GL_LINE_STRIP_ADJACENCY, 
    GL_LINES_ADJACENCY, 
    GL_TRIANGLE_STRIP_ADJACENCY, 
    GL_TRIANGLES_ADJACENCY
};

EnumMap<TextureFormat> GraphicsEnums::textureFormats = {
    GL_RED, 
    GL_RG, 
    GL_RGB, 
    GL_RGBA, 
    GL_RED_INTEGER, 
    GL_RG_INTEGER, 
    GL_RGB_INTEGER, 
    GL_RGBA_INTEGER,
    GL_STENCIL_INDEX, 
    GL_DEPTH_COMPONENT, 
    GL_DEPTH_STENCIL
};
EnumMap<TextureDataType> GraphicsEnums::textureDataTypes = {
    GL_UNSIGNED_BYTE, 
    GL_BYTE, 
    GL_UNSIGNED_SHORT, 
    GL_SHORT, 
    GL_UNSIGNED_INT, 
    GL_INT, 
    GL_HALF_FLOAT, 
    GL_FLOAT, 
    GL_UNSIGNED_BYTE_3_3_2,
    GL_UNSIGNED_SHORT_4_4_4_4,
    GL_UNSIGNED_SHORT_5_5_5_1,
    GL_UNSIGNED_INT_10_10_10_2,
    GL_UNSIGNED_INT_24_8,
    GL_FLOAT_32_UNSIGNED_INT_24_8_REV
};
EnumMap<TextureSizedFormat> GraphicsEnums::textureSizedFormats = {
    GL_R8,
    GL_R8_SNORM,
    GL_R16,
    GL_R16_SNORM,
    GL_RG8,
    GL_RG8_SNORM,
    GL_RG16,
    GL_RG16_SNORM,
    GL_R3_G3_B2,
    GL_RGB8,
    GL_RGB8_SNORM,
    GL_RGB16_SNORM,
    GL_RGBA4,
    GL_RGB5_A1,
    GL_RGBA8,
    GL_RGBA8_SNORM,
    GL_RGB10_A2,
    GL_RGB10_A2UI,
    GL_RGBA16,
    GL_SRGB8,
    GL_SRGB8_ALPHA8,
    GL_R16F,
    GL_RG16F,
    GL_RGB16F,
    GL_RGBA16F,
    GL_R32F,
    GL_RG32F,
    GL_RGB32F,
    GL_RGBA32F,
    GL_R8I,
    GL_R8UI,
    GL_R16I,
    GL_R16UI,
    GL_R32I,
    GL_R32UI,
    GL_RG8I,
    GL_RG8UI,
    GL_RG16I,
    GL_RG16UI,
    GL_RG32I,
    GL_RG32UI,
    GL_RGB8I,
    GL_RGB8UI,
    GL_RGB16I,
    GL_RGB16UI,
    GL_RGB32I,
    GL_RGB32UI,
    GL_RGBA8I,
    GL_RGBA8UI,
    GL_RGBA16I,
    GL_RGBA16UI,
    GL_RGBA32I,
    GL_RGBA32UI,
    GL_DEPTH_COMPONENT16,
    GL_DEPTH_COMPONENT24,
    GL_DEPTH_COMPONENT32,
    GL_DEPTH_COMPONENT32F,
    GL_DEPTH24_STENCIL8,
    GL_DEPTH32F_STENCIL8,
    GL_STENCIL_INDEX1,
    GL_STENCIL_INDEX4,
    GL_STENCIL_INDEX8,
    GL_STENCIL_INDEX16
};

EnumMap<TextureType> GraphicsEnums::textureTypes = {
    GL_TEXTURE_1D,
    GL_TEXTURE_2D,
    GL_TEXTURE_3D,
    GL_TEXTURE_1D_ARRAY,
    GL_TEXTURE_2D_ARRAY,
    GL_TEXTURE_RECTANGLE,
    GL_TEXTURE_CUBE_MAP,
    GL_TEXTURE_CUBE_MAP_ARRAY,
    GL_TEXTURE_BUFFER,
    GL_TEXTURE_2D_MULTISAMPLE, 
    GL_TEXTURE_2D_MULTISAMPLE_ARRAY
};

EnumMap<TextureUnit> GraphicsEnums::textureUnits = {
    GL_TEXTURE0 + 0,
    GL_TEXTURE0 + 1,
    GL_TEXTURE0 + 2,
    GL_TEXTURE0 + 3,
    GL_TEXTURE0 + 4,
    GL_TEXTURE0 + 5,
    GL_TEXTURE0 + 6,
    GL_TEXTURE0 + 7,
    GL_TEXTURE0 + 8,
    GL_TEXTURE0 + 9,
    GL_TEXTURE0 + 10,
    GL_TEXTURE0 + 11,
    GL_TEXTURE0 + 12,
    GL_TEXTURE0 + 13,
    GL_TEXTURE0 + 14,
    GL_TEXTURE0 + 15,
    GL_TEXTURE0 + 16,
    GL_TEXTURE0 + 17,
    GL_TEXTURE0 + 18,
    GL_TEXTURE0 + 19,
    GL_TEXTURE0 + 20,
    GL_TEXTURE0 + 21,
    GL_TEXTURE0 + 22,
    GL_TEXTURE0 + 23,
    GL_TEXTURE0 + 24,
    GL_TEXTURE0 + 25,
    GL_TEXTURE0 + 26,
    GL_TEXTURE0 + 27,
    GL_TEXTURE0 + 28,
    GL_TEXTURE0 + 29,
    GL_TEXTURE0 + 30,
    GL_TEXTURE0 + 31
};

EnumMap<TextureCubeMapSide> GraphicsEnums::textureCubeMapSides = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

EnumMap<TextureMinFilter> GraphicsEnums::textureMinFilters =
{
    GL_NEAREST,
    GL_LINEAR,
    GL_NEAREST_MIPMAP_NEAREST,
    GL_LINEAR_MIPMAP_NEAREST,
    GL_NEAREST_MIPMAP_LINEAR,
    GL_LINEAR_MIPMAP_LINEAR
};
EnumMap<TextureMagFilter> GraphicsEnums::textureMagFilters =
{
    GL_NEAREST,
    GL_LINEAR
};
EnumMap<TextureWrapAxis> GraphicsEnums::textureWrapAxis =
{
    GL_TEXTURE_WRAP_S,
    GL_TEXTURE_WRAP_T,
    GL_TEXTURE_WRAP_R
};
EnumMap<TextureWrapType> GraphicsEnums::textureWrapTypes =
{
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER,
    GL_MIRRORED_REPEAT,
    GL_REPEAT,
    GL_MIRROR_CLAMP_TO_EDGE
};
EnumMap<CustomFramebufferAttachment> GraphicsEnums::customFramebufferAttachments = {
    GL_DEPTH_ATTACHMENT,
    GL_STENCIL_ATTACHMENT,
    GL_DEPTH_STENCIL_ATTACHMENT,
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8,
    GL_COLOR_ATTACHMENT9,
    GL_COLOR_ATTACHMENT10,
    GL_COLOR_ATTACHMENT11,
    GL_COLOR_ATTACHMENT12,
    GL_COLOR_ATTACHMENT13,
    GL_COLOR_ATTACHMENT14,
    GL_COLOR_ATTACHMENT15,
    GL_COLOR_ATTACHMENT16,
    GL_COLOR_ATTACHMENT17,
    GL_COLOR_ATTACHMENT18,
    GL_COLOR_ATTACHMENT19,
    GL_COLOR_ATTACHMENT20,
    GL_COLOR_ATTACHMENT21,
    GL_COLOR_ATTACHMENT22,
    GL_COLOR_ATTACHMENT23,
    GL_COLOR_ATTACHMENT24,
    GL_COLOR_ATTACHMENT25,
    GL_COLOR_ATTACHMENT26,
    GL_COLOR_ATTACHMENT27,
    GL_COLOR_ATTACHMENT28,
    GL_COLOR_ATTACHMENT29,
    GL_COLOR_ATTACHMENT30,
    GL_COLOR_ATTACHMENT31
};
EnumMap<CustomFramebufferDrawAttachment> GraphicsEnums::customFramebufferDrawAttachments = {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8,
    GL_COLOR_ATTACHMENT9,
    GL_COLOR_ATTACHMENT10,
    GL_COLOR_ATTACHMENT11,
    GL_COLOR_ATTACHMENT12,
    GL_COLOR_ATTACHMENT13,
    GL_COLOR_ATTACHMENT14,
    GL_COLOR_ATTACHMENT15,
    GL_COLOR_ATTACHMENT16,
    GL_COLOR_ATTACHMENT17,
    GL_COLOR_ATTACHMENT18,
    GL_COLOR_ATTACHMENT19,
    GL_COLOR_ATTACHMENT20,
    GL_COLOR_ATTACHMENT21,
    GL_COLOR_ATTACHMENT22,
    GL_COLOR_ATTACHMENT23,
    GL_COLOR_ATTACHMENT24,
    GL_COLOR_ATTACHMENT25,
    GL_COLOR_ATTACHMENT26,
    GL_COLOR_ATTACHMENT27,
    GL_COLOR_ATTACHMENT28,
    GL_COLOR_ATTACHMENT29,
    GL_COLOR_ATTACHMENT30,
    GL_COLOR_ATTACHMENT31
};
EnumMap<BlendFuncType> GraphicsEnums::blendFuncTypes = {
    GL_ZERO,
    GL_ONE, 
    GL_SRC_COLOR, 
    GL_ONE_MINUS_SRC_COLOR,
    GL_DST_COLOR, 
    GL_ONE_MINUS_DST_COLOR, 
    GL_SRC_ALPHA, 
    GL_ONE_MINUS_SRC_ALPHA, 
    GL_DST_ALPHA, 
    GL_ONE_MINUS_DST_ALPHA, 
    GL_CONSTANT_COLOR, 
    GL_ONE_MINUS_CONSTANT_COLOR, 
    GL_CONSTANT_ALPHA, 
    GL_ONE_MINUS_CONSTANT_ALPHA, 
    GL_SRC_ALPHA_SATURATE, 
    GL_SRC1_COLOR, 
    GL_ONE_MINUS_SRC1_COLOR, 
    GL_SRC1_ALPHA, 
    GL_ONE_MINUS_SRC1_ALPHA
};
EnumMap<CullFaceType> GraphicsEnums::cullFaceTypes = {
    GL_FRONT,
    GL_BACK,
    GL_FRONT_AND_BACK
};
EnumMap<FrontFaceType> GraphicsEnums::frontFaceTypes = {
    GL_CW,
    GL_CCW
};
EnumMap<DepthFuncType> GraphicsEnums::depthFuncTypes = {
    GL_NEVER, 
    GL_LESS, 
    GL_EQUAL, 
    GL_LEQUAL, 
    GL_GREATER, 
    GL_NOTEQUAL, 
    GL_GEQUAL, 
    GL_ALWAYS
};
EnumMap<StencilFuncType> GraphicsEnums::stencilFuncTypes = {
    GL_NEVER, 
    GL_LESS, 
    GL_LEQUAL, 
    GL_GREATER, 
    GL_GEQUAL, 
    GL_EQUAL, 
    GL_NOTEQUAL, 
    GL_ALWAYS
};
EnumMap<StencilOpType> GraphicsEnums::stencilOpTypes = {
    GL_KEEP, 
    GL_ZERO, 
    GL_REPLACE, 
    GL_INCR, 
    GL_INCR_WRAP, 
    GL_DECR, 
    GL_DECR_WRAP, 
    GL_INVERT
};
EnumMap<PolygonModeType> GraphicsEnums::polygonModeTypes = {
    GL_POINT, 
    GL_LINE, 
    GL_FILL
};
EnumMap<GraphicsQueryType> GraphicsEnums::queryTypes = {
    GL_SAMPLES_PASSED,
    GL_ANY_SAMPLES_PASSED,
    GL_ANY_SAMPLES_PASSED_CONSERVATIVE,
    GL_PRIMITIVES_GENERATED,
    GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, 
    GL_TIME_ELAPSED
};

TextureDataFormat GraphicsEnums::GetTextureDataFormat(TextureSizedFormat format)
{
    auto resIt = s_sizedFormatToDataFormat.find(format);
    assert(resIt != s_sizedFormatToDataFormat.end());
    return resIt->second;
}

}