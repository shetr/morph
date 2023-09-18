#version 450 core

#type vert
			
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texCoord;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec3 v_tangent;

out vec3 f_position;

uniform mat4 u_M;
uniform mat4 u_PVM;

void main()
{
    f_position = vec3(u_M*vec4(v_position, 1));
    gl_Position = u_PVM * vec4(v_position, 1);	
}

#type frag

#include <engine:shaders/libs/PBR.glsl>

layout(location = 0) out vec4 color;

in vec3 f_position;

uniform mat4 u_M;

uniform uint u_lights_count;
#ifndef MAX_LIGHT_COUNT
    #define MAX_LIGHT_COUNT 1
#endif
uniform Light u_lights[MAX_LIGHT_COUNT];

void main()
{
    vec3 dirToLight = vec3(0);
    for(uint l = 0; l < u_lights_count; l++) {   
        dirToLight += u_lights[l].position.xyz - f_position;
    }
    dirToLight = dirToLight / u_lights_count;
    dirToLight = normalize(dirToLight);
   
    color = vec4((dirToLight + vec3(1)) / 2, 1.0);
}