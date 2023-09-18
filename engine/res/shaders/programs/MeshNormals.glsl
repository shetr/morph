#version 450 core

#type vert
			
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texCoord;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec3 v_tangent;

out vec3 f_normal;

uniform mat4 u_PVM;

void main()
{
    f_normal = v_normal;
    gl_Position = u_PVM * vec4(v_position, 1);	
}

#type frag

layout(location = 0) out vec4 color;

in vec3 f_normal;

uniform mat4 u_M;

void main()
{
    vec3 normal = normalize((u_M * vec4(f_normal, 0)).xyz);
    color = vec4((normal + vec3(1)) / 2, 1);
}