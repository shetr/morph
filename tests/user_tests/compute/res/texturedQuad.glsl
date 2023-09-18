#version 450 core

#type vert
			
layout(location = 0) in vec2 a_position;
layout(location = 1) in vec2 a_uv;

out vec2 f_uv;

void main()
{
    f_uv = a_uv;
    gl_Position = vec4(a_position, 0, 1.0);	
}

#type frag
			
layout(location = 0) out vec4 color;

in vec2 f_uv;

uniform vec3 u_color = vec3(0, 1, 0);

uniform sampler2D u_textureSampler;

void main()
{
    //color = vec4(u_color, 1.0);
    color = vec4(texture2D(u_textureSampler, f_uv).xyz, 1);
}