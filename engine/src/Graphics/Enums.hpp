
#ifndef MORPH_GRAPHICS_ENUMS_HPP
#define MORPH_GRAPHICS_ENUMS_HPP

#include <Core/Core.hpp>

namespace Morph {

struct Rectangle
{
    uvec2 from;
    uvec2 size;
};

enum class MakeCurrentErrorType
{
    CONTEXT_ALREADY_CURRENT,
    THREAD_ALREADY_CURRENT,
    OPENGL_LOAD_FAIL
};

enum class GraphicsBufferTarget {
    ARRAY,
    ATOMIC_COUNTER,
    COPY_READ,
    COPY_WRITE,
    DISPATCH_INDIRECT,
    DRAW_INDIRECT,
    ELEMENT_ARRAY,
    PIXEL_PACK,
    PIXEL_UNPACK,
    QUERY,
    SHADER_STORAGE,
    TEXTURE,
    TRANSFORM_FEEDBACK,
    UNIFORM
};

enum class GraphicsAccess {
    READ,
    WRITE,
    READ_WRITE
};

enum class VertexType
{
    INT8,
    INT16,
    INT32,
    UINT8,
    UINT16,
    UINT32,
    FLOAT16,
    FLOAT32,
    FLOAT64,
    FIXED
};
enum class VertexPackedType
{
    INT_2_10_10_10_REV,
    UINT_2_10_10_10_REV,
    UINT_10F_11F_11F_REV
};
enum class VertexElementCount : uint
{
    _1 = 1,
    _2 = 2,
    _3 = 3,
    _4 = 4
};
enum class IndexType
{
    UINT8,
    UINT16,
    UINT32
};

enum class ShaderType
{
    FRAGMENT,
    VERTEX,
    GEOMETRY,
    TESS_EVALUATION,
    TESS_CONTROL,
    COMPUTE
};

enum class RenderShaderType
{
    FRAGMENT,
    VERTEX,
    GEOMETRY,
    TESS_EVALUATION,
    TESS_CONTROL
};

enum class PrimitiveAllType
{
    POINTS, 
    LINE_STRIP, 
    LINE_LOOP, 
    LINES, 
    TRIANGLE_STRIP, 
    TRIANGLE_FAN, 
    TRIANGLES, 
    PATCHES,
    LINE_STRIP_ADJACENCY, 
    LINES_ADJACENCY, 
    TRIANGLE_STRIP_ADJACENCY, 
    TRIANGLES_ADJACENCY
};

enum class PrimitiveType
{
    POINTS, 
    LINE_STRIP, 
    LINE_LOOP, 
    LINES, 
    TRIANGLE_STRIP, 
    TRIANGLE_FAN, 
    TRIANGLES, 
    PATCHES
};

enum class PrimitiveAdjacencyType
{
    LINE_STRIP, 
    LINES, 
    TRIANGLE_STRIP, 
    TRIANGLES
};

enum class TextureFormat
{
    R, 
    RG, 
    RGB, 
    RGBA, 
    R_INT, 
    RG_INT, 
    RGB_INT, 
    RGBA_INT,
    STENCIL, 
    DEPTH, 
    DEPTH_STENCIL
};

enum class TextureDataType
{
    UINT8, 
    INT8, 
    UINT16, 
    INT16, 
    UINT32, 
    INT32, 
    FLOAT16, 
    FLOAT32, 
    UINT_3_3_2,
    UINT_4_4_4_4,
    UINT_5_5_5_1,
    UINT_10_10_10_2,
    UINT_24_8,
    FLOAT_32_UINT_24_8_REV
};

enum class TextureSizedFormat
{
    R8,
    R8_S,
    R16,
    R16_S,
    RG8,
    RG8_S,
    RG16,
    RG16_S,
    R3_G3_B2,
    RGB8,
    RGB8_S,
    RGB16_S,
    RGBA4,
    RGB5_A1,
    RGBA8,
    RGBA8_S,
    RGB10_A2,
    RGB10_A2UI,
    RGBA16,
    SRGB8,
    SRGB8_A8,
    R16F,
    RG16F,
    RGB16F,
    RGBA16F,
    R32F,
    RG32F,
    RGB32F,
    RGBA32F,
    R8I,
    R8UI,
    R16I,
    R16UI,
    R32I,
    R32UI,
    RG8I,
    RG8UI,
    RG16I,
    RG16UI,
    RG32I,
    RG32UI,
    RGB8I,
    RGB8UI,
    RGB16I,
    RGB16UI,
    RGB32I,
    RGB32UI,
    RGBA8I,
    RGBA8UI,
    RGBA16I,
    RGBA16UI,
    RGBA32I,
    RGBA32UI,
    DEPTH16,
    DEPTH24,
    DEPTH32,
    DEPTH32F,
    DEPTH24_STENCIL8,
    DEPTH32F_STENCIL8,
    STENCIL1,
    STENCIL4,
    STENCIL8,
    STENCIL16
};

struct TextureDataFormat
{
    TextureFormat format;
    TextureDataType type;
};

enum class TextureType 
{
    _1D,
    _2D,
    _3D,
    _1D_ARRAY,
    _2D_ARRAY,
    RECTANGLE,
    CUBE_MAP,
    CUBE_MAP_ARRAY,
    BUFFER,
    _2D_MULTISAMPLE, 
    _2D_MULTISAMPLE_ARRAY
};

enum class TextureUnit
{
    _0 = 0,
    _1,
    _2,
    _3,
    _4,
    _5,
    _6,
    _7,
    _8,
    _9,
    _10,
    _11,
    _12,
    _13,
    _14,
    _15,
    _16,
    _17,
    _18,
    _19,
    _20,
    _21,
    _22,
    _23,
    _24,
    _25,
    _26,
    _27,
    _28,
    _29,
    _30,
    _31
};


enum class TextureCubeMapSide
{
    POS_X,
    NEG_X,
    POS_Y,
    NEG_Y,
    POS_Z,
    NEG_Z
};

enum class TextureMinFilter
{
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR
};

enum class TextureMagFilter
{
    NEAREST,
    LINEAR
};

enum class TextureWrapAxis
{
    X, Y, Z
};

enum class TextureWrapType
{
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
    MIRRORED_REPEAT,
    REPEAT,
    MIRROR_CLAMP_TO_EDGE
};

enum class CustomFramebufferAttachment
{
    DEPTH,
    STENCIL,
    DEPTH_STENCIL,
    COLOR_0,
    COLOR_1,
    COLOR_2,
    COLOR_3,
    COLOR_4,
    COLOR_5,
    COLOR_6,
    COLOR_7,
    COLOR_8,
    COLOR_9,
    COLOR_10,
    COLOR_11,
    COLOR_12,
    COLOR_13,
    COLOR_14,
    COLOR_15,
    COLOR_16,
    COLOR_17,
    COLOR_18,
    COLOR_19,
    COLOR_20,
    COLOR_21,
    COLOR_22,
    COLOR_23,
    COLOR_24,
    COLOR_25,
    COLOR_26,
    COLOR_27,
    COLOR_28,
    COLOR_29,
    COLOR_30,
    COLOR_31
};

enum class CustomFramebufferDrawAttachment
{
    COLOR_0,
    COLOR_1,
    COLOR_2,
    COLOR_3,
    COLOR_4,
    COLOR_5,
    COLOR_6,
    COLOR_7,
    COLOR_8,
    COLOR_9,
    COLOR_10,
    COLOR_11,
    COLOR_12,
    COLOR_13,
    COLOR_14,
    COLOR_15,
    COLOR_16,
    COLOR_17,
    COLOR_18,
    COLOR_19,
    COLOR_20,
    COLOR_21,
    COLOR_22,
    COLOR_23,
    COLOR_24,
    COLOR_25,
    COLOR_26,
    COLOR_27,
    COLOR_28,
    COLOR_29,
    COLOR_30,
    COLOR_31
};

enum class MemoryBarrierType : uint
{
    VERTEX_ATTRIB_ARRAY = 0x00000001,
    ELEMENT_ARRAY = 0x00000002,
    UNIFORM = 0x00000004,
    TEXTURE_FETCH = 0x00000008,
    SHADER_IMAGE_ACCESS = 0x00000020,
    COMMAND = 0x00000040,
    PIXEL_BUFFER = 0x00000080,
    TEXTURE_UPDATE = 0x00000100,
    BUFFER_UPDATE = 0x00000200,
    FRAMEBUFFER = 0x00000400,
    TRANSFORM_FEEDBACK = 0x00000800,
    ATOMIC_COUNTER = 0x00001000,
    SHADER_STORAGE = 0x00002000,
    CLIENT_MAPPED_BUFFER = 0x00004000,
    QUERY_BUFFER = 0x00008000,
    ALL = 0xFFFFFFFF
};

enum class FramebufferBufferType : uint
{
    DEPTH = 0x00000100,
    STENCIL = 0x00000400,
    COLOR = 0x00004000
};

enum class BlendFuncType
{
    ZERO,
    ONE, 
    SRC_COLOR, 
    ONE_MINUS_SRC_COLOR,
    DST_COLOR, 
    ONE_MINUS_DST_COLOR, 
    SRC_ALPHA, 
    ONE_MINUS_SRC_ALPHA, 
    DST_ALPHA, 
    ONE_MINUS_DST_ALPHA, 
    CONSTANT_COLOR, 
    ONE_MINUS_CONSTANT_COLOR, 
    CONSTANT_ALPHA, 
    ONE_MINUS_CONSTANT_ALPHA, 
    SRC_ALPHA_SATURATE, 
    SRC1_COLOR, 
    ONE_MINUS_SRC1_COLOR, 
    SRC1_ALPHA, 
    ONE_MINUS_SRC1_ALPHA
};

enum class CullFaceType
{
    FRONT,
    BACK,
    FRONT_AND_BACK
};

enum class FrontFaceType
{
    CW,
    CCW
};

enum class DepthFuncType
{
    NEVER, 
    LESS, 
    EQUAL, 
    LEQUAL, 
    GREATER, 
    NOTEQUAL, 
    GEQUAL, 
    ALWAYS
};

enum class StencilFuncType
{
    NEVER, 
    LESS, 
    LEQUAL, 
    GREATER, 
    GEQUAL, 
    EQUAL, 
    NOTEQUAL, 
    ALWAYS
};

enum class StencilOpType
{
    KEEP, 
    ZERO, 
    REPLACE, 
    INCR, 
    INCR_WRAP, 
    DECR, 
    DECR_WRAP, 
    INVERT
};

enum class PolygonModeType
{
    POINT, 
    LINE, 
    FILL
};

enum class GraphicsQueryType
{
    SAMPLES_PASSED,
    ANY_SAMPLES_PASSED,
    ANY_SAMPLES_PASSED_CONSERVATIVE,
    PRIMITIVES_GENERATED,
    TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, 
    TIME_ELAPSED
};

class GraphicsEnums
{
private:
    static unord_map<TextureSizedFormat, TextureDataFormat> s_sizedFormatToDataFormat;
public:
    static EnumMap<GraphicsBufferTarget> bufferTargets;
    static EnumMap<GraphicsAccess> accessValues;
    static EnumMap<VertexType> vertexTypes;
    static EnumMap<VertexPackedType> vertexPackedTypes;
    static EnumMap<IndexType> indexTypes;
    static EnumMap<ShaderType> shaderTypes;
    static EnumMap<PrimitiveAllType> primitiveAllTypes;
    static EnumMap<PrimitiveType> primitiveTypes;
    static EnumMap<PrimitiveAdjacencyType> primitiveAdjacencyTypes;
    static EnumMap<TextureFormat> textureFormats;
    static EnumMap<TextureDataType> textureDataTypes;
    static EnumMap<TextureSizedFormat> textureSizedFormats;
    static EnumMap<TextureType> textureTypes;
    static EnumMap<TextureUnit> textureUnits;
    static EnumMap<TextureCubeMapSide> textureCubeMapSides;
    static EnumMap<TextureMinFilter> textureMinFilters;
    static EnumMap<TextureMagFilter> textureMagFilters;
    static EnumMap<TextureWrapAxis> textureWrapAxis;
    static EnumMap<TextureWrapType> textureWrapTypes;
    static EnumMap<CustomFramebufferAttachment> customFramebufferAttachments;
    static EnumMap<CustomFramebufferDrawAttachment> customFramebufferDrawAttachments;
    static EnumMap<BlendFuncType> blendFuncTypes;
    static EnumMap<CullFaceType> cullFaceTypes;
    static EnumMap<FrontFaceType> frontFaceTypes;
    static EnumMap<DepthFuncType> depthFuncTypes;
    static EnumMap<StencilFuncType> stencilFuncTypes;
    static EnumMap<StencilOpType> stencilOpTypes;
    static EnumMap<PolygonModeType> polygonModeTypes;
    static EnumMap<GraphicsQueryType> queryTypes;

    inline static GraphicsAccess ConvertAccess(bool read, bool write) {
        return read && write ? GraphicsAccess::READ_WRITE : (read ? GraphicsAccess::READ : GraphicsAccess::WRITE);
    }

    template<typename INDEX_TYPE>
    static IndexType GetIndexType() {
        if(type_index(typeid(INDEX_TYPE)) == type_index(typeid(u32))) {
            return IndexType::UINT32;
        }
        if(type_index(typeid(INDEX_TYPE)) == type_index(typeid(u16))) {
            return IndexType::UINT16;
        }
        if(type_index(typeid(INDEX_TYPE)) == type_index(typeid(u8))) {
            return IndexType::UINT8;
        }   
        panic("Index buffer type not supported");
        return IndexType::UINT32;
    }

    static TextureDataFormat GetTextureDataFormat(TextureSizedFormat format);
};

}

#endif // MORPH_GRAPHICS_ENUMS_HPP