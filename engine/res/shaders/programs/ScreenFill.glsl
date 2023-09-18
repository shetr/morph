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

uniform sampler2D u_textureSampler;

void main()
{
    color = vec4(texture2D(u_textureSampler, f_texCoord).xyz, 1);
}