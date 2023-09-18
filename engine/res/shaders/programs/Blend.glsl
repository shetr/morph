#version 450 core

#type vert
			
layout(location = 0) in vec2 v_position;
layout(location = 1) in vec2 v_texCoord;

out vec2 f_texCoord;

void main()
{
    f_texCoord = v_texCoord;
    gl_Position = vec4(v_position, 0, 1.0);
}

#type frag

layout(location = 0) out vec4 color;

in vec2 f_texCoord;

uniform sampler2D u_texture1Sampler;
uniform sampler2D u_texture2Sampler;
uniform float u_blendFactor;

void main()
{
    vec4 texture1 = texture2D(u_texture1Sampler, f_texCoord);
    vec4 texture2 = texture2D(u_texture2Sampler, f_texCoord);
    color = (1 - u_blendFactor) * texture1 + u_blendFactor * texture2;
}