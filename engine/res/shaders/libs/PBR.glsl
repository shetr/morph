
#define LIGHT_NONE 0
#define LIGHT_DIR 1
#define LIGHT_POINT 2
#define LIGHT_SPOT 3

#define PBR_PI 3.14159265359

struct Light
{
    int lightType;
    vec3 color;
    vec3 position;
    vec3 spotDir;
    float spotExponent;
    float spotCutoff;
    float attenuationConst;
    float attenuationLin;
    float attenuationQuad;
};

struct Material
{
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

struct MaterialTex
{
    sampler2D albedo;
    sampler2D metallic;
    sampler2D roughness;
    sampler2D ao;
    sampler2D normal;
};

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 FresnelSchlick(float cosTheta, vec3 F0);

vec3 ComputeLight(vec3 position, vec3 normal, vec3 cameraPosition, Material material, Light light)
{
    if(light.lightType <= LIGHT_NONE || light.lightType > LIGHT_SPOT) {
        return vec3(0);
    }

    vec3 dirToCam = normalize(cameraPosition - position);
    vec3 dirToLight;
    float spotLightEffect = 1;
    float attenuation = 1;
    if(light.lightType == LIGHT_DIR) { // DIR light
        dirToLight = normalize(-light.spotDir);
        attenuation = 1 / light.attenuationConst;
    } else { // POINT or SPOT light
        dirToLight = light.position.xyz - position;
        float dist = length(dirToLight);
        dirToLight = normalize(dirToLight);
        attenuation = 1 / (light.attenuationConst + light.attenuationLin * dist + light.attenuationQuad * dist * dist);
        if(light.lightType == LIGHT_SPOT) { // SPOT light
            float cosAlpha = dot(dirToLight, -light.spotDir);
            if(cosAlpha < cos(light.spotCutoff)) {
                spotLightEffect = 0;
            } else {
                spotLightEffect = pow(max(cosAlpha,0), light.spotExponent);
            }
        }
    }

    vec3 F0 = mix(vec3(0.04), material.albedo, material.metallic); // surface reflectance
    vec3 V = dirToCam;
    vec3 L = dirToLight;
    vec3 N = normal;
    vec3 H = normalize(V + L);

    vec3 radiance = light.color * attenuation * spotLightEffect;
    
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, material.roughness);        
    float G   = GeometrySmith(N, V, L, material.roughness);      
    vec3 F    = FresnelSchlick(max(dot(H, V), 0.0), F0);       
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - material.metallic;	  
    
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
    vec3 specular     = numerator / max(denominator, 0.001);  
        
    // add to outgoing radiance Lo
    float NdotL = max(dot(N, L), 0.0);                
    return (kD * material.albedo / PBR_PI + specular) * radiance * NdotL;
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PBR_PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  