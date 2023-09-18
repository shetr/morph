#version 450 core
#extension GL_EXT_gpu_shader4 : require

#type vert
			
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_uv;

out vec2 f_uv;

void main()
{
    f_uv = a_uv;
    gl_Position = vec4(a_position, 0, 1);	
}

#type frag

layout(location = 0) out vec4 color;

in vec2 f_uv;

uniform uint u_iterationsLimit;
uniform usampler2D u_iterationsSampler;
uniform vec3 u_insideColor;
uniform vec3 u_minColor;
uniform vec3 u_maxColor;
uniform int u_antialiasing;
uniform uint u_SSAA_samplesWidth;

float kernel[9] = float[](
    1.0 / 32, 3.0 / 32, 1.0 / 32,
    3.0 / 32, 16.0 / 32, 3.0 / 32,
    1.0 / 32, 3.0 / 32, 1.0 / 32  
);
vec3 GetColor(vec2 uv)
{
    uint iterations = texture2D(u_iterationsSampler, uv).x;
    if(iterations > u_iterationsLimit) {
        return u_insideColor;
    } else {
        float iterRatio = float(iterations) / float(u_iterationsLimit);
        
        return mix(u_minColor, u_maxColor, iterRatio);
    }
}

vec3 GetColorSampled(vec2 uv)
{
    vec3 color = vec3(0);
    vec2 offset = vec2(1) / vec2(textureSize(u_iterationsSampler, 0));
    vec2 halfWidth = offset * vec2(float(u_SSAA_samplesWidth) * 0.5);
    vec2 uvStart = uv - halfWidth;

    for(uint yi = 0; yi < u_SSAA_samplesWidth; yi++) {
        for(uint xi = 0; xi < u_SSAA_samplesWidth; xi++) {
            vec2 uvLocal = uvStart + vec2(xi, yi) * offset;
            color += GetColor(uvLocal);
        }
    }
    return color / vec3(u_SSAA_samplesWidth * u_SSAA_samplesWidth);    
}

vec3 GetColorBluredAA()
{
    vec2 offset = vec2(1) / vec2(textureSize(u_iterationsSampler, 0));  

    vec2 offsets[9] = vec2[](
        vec2(-offset.x,  offset.y), // top-left
        vec2( 0.0f,    offset.y), // top-center
        vec2( offset.x,  offset.y), // top-right
        vec2(-offset.x,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset.x,  0.0f),   // center-right
        vec2(-offset.x, -offset.y), // bottom-left
        vec2( 0.0f,   -offset.y), // bottom-center
        vec2( offset.x, -offset.y)  // bottom-right    
    );
    
    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = GetColor(f_uv + offsets[i]);
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    
    return col;
}

void main()
{
    if(u_antialiasing == 0) {
        color = vec4(GetColor(f_uv), 1);
    } else if(u_antialiasing == 1) {
        color = vec4(GetColorBluredAA(), 1);
    } else {
        color = vec4(GetColorSampled(f_uv), 1);
    }
}