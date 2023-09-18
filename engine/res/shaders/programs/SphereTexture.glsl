#version 450 core

#type vert
			
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texCoord;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec3 v_tangent;

out vec3 f_normal;
out vec3 f_position;

uniform mat4 u_M;
uniform mat4 u_PVM;

void main()
{
    f_normal = v_normal;
    f_position = (u_M * vec4(v_position, 1)).xyz;
    gl_Position = u_PVM * vec4(v_position, 1);	
}

#type frag

#define PI 3.14159265359

layout(location = 0) out vec4 color;

in vec3 f_normal;
in vec3 f_position;

uniform sampler2D u_textureSampler;

void main()
{
    vec3 centerNormal = normalize(f_position);
    vec3 localNormal = f_normal;
    vec3 normal = normalize(centerNormal);

    vec2 xz = vec2(normal.x, normal.z);
    vec2 ty = vec2(length(xz), normal.y);
    vec2 xzDir = normalize(xz);
    vec2 tyDir = normalize(ty);

    float phi = xzDir.y >= 0 ? acos(-xzDir.x) : PI + acos(xzDir.x);
    float theta = acos(-tyDir.y);

    vec2 texCoord = vec2(phi, theta) / vec2(2*PI, PI);

    color = texture2D(u_textureSampler, texCoord);
}