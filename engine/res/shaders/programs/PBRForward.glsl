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

#include <engine:shaders/libs/PBR.glsl>

layout(location = 0) out vec4 color;

in vec3 f_position;
in vec2 f_texCoord;
in vec3 f_normal;

uniform mat4 u_M;

uniform vec3 u_envGlobalAmbient;
uniform vec3 u_cameraPosition;
uniform Material u_material;
uniform MaterialTex u_materialTex;
uniform uint u_lights_count;
#ifndef MAX_LIGHT_COUNT
    #define MAX_LIGHT_COUNT 1
#endif
uniform Light u_lights[MAX_LIGHT_COUNT];

void main()
{
    vec3 normal = normalize((u_M * vec4(f_normal, 0)).xyz);

    Material material;
    material.albedo = u_material.albedo * texture(u_materialTex.albedo, f_texCoord).xyz;
    material.metallic = u_material.metallic * texture(u_materialTex.metallic, f_texCoord).x;
    material.roughness = u_material.roughness * texture(u_materialTex.roughness, f_texCoord).x;
    material.ao = u_material.ao * texture(u_materialTex.ao, f_texCoord).x;

    vec3 Lo = vec3(0.0);
    for(uint l = 0; l < u_lights_count; l++) {             
        Lo += ComputeLight(f_position,  normal, u_cameraPosition, material, u_lights[l]); 
    }   
  
    vec3 ambient = u_envGlobalAmbient * material.albedo * material.ao;
    vec3 colorPBR = ambient + Lo;
	
    colorPBR = colorPBR / (colorPBR + vec3(1.0));
    colorPBR = pow(colorPBR, vec3(1.0/2.2));  
   
    color = vec4(colorPBR, 1.0);
}