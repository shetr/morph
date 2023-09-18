#version 450 core

#type vert
			
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texCoord;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec3 v_tangent;

uniform mat4 u_PVM;

void main()
{
    gl_Position = u_PVM * vec4(v_position, 1);	
}

#type frag

layout(location = 0) out vec4 color;

uniform vec4 u_color;

void main()
{
    color = u_color;
}