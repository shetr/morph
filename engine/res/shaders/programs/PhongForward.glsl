#version 450 core

#type vert
			
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec2 v_texCoord;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec3 v_tangent;

out vec3 f_position;
out vec2 f_texCoord;
out vec3 f_normal;

uniform mat4 u_M;
uniform mat4 u_V;
uniform mat4 u_P;
uniform mat4 u_PVM;

void main()
{
    f_position = vec3(u_M*vec4(v_position, 1));
    f_texCoord = v_texCoord;
    f_normal = v_normal;
    gl_Position = u_PVM * vec4(v_position, 1);	
}

#type frag

#include <engine:shaders/libs/Phong.glsl>

layout(location = 0) out vec4 color;

in vec3 f_position;
in vec2 f_texCoord;
in vec3 f_normal;

uniform mat4 u_M;

uniform vec4 u_envGlobalAmbient;
uniform vec3 u_cameraPosition;
uniform PhongMaterial u_material;
uniform PhongMaterialTex u_materialTex;
uniform uint u_lights_count;
#ifndef PHONG_MAX_LIGHT_COUNT
    #define PHONG_MAX_LIGHT_COUNT 1
#endif
uniform PhongLight u_lights[PHONG_MAX_LIGHT_COUNT];

void main()
{
    //vec3 normal = normalize(u_N * f_normal);
    vec3 normal = normalize((u_M * vec4(f_normal, 0)).xyz);

    PhongMaterial material;
    material.ambient = u_material.ambient * texture(u_materialTex.ambient, f_texCoord);
    material.diffuse = u_material.diffuse * texture(u_materialTex.diffuse, f_texCoord);
    material.specular = u_material.specular * texture(u_materialTex.specular, f_texCoord);
    material.emission = u_material.emission * texture(u_materialTex.emission, f_texCoord);
    material.shinines = u_material.shinines;
    vec4 globalAmbient = u_envGlobalAmbient * material.ambient;
    vec4 phongColor =  material.emission + globalAmbient;
    for(uint l = 0; l < u_lights_count; l++) {
        phongColor += Phong_ComputeLight(f_position,  normal, u_cameraPosition, material, u_lights[l]);
    }

    color = phongColor;
}