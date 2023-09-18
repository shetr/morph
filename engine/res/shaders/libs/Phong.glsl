
#define PHONG_LIGHT_NONE 0
#define PHONG_LIGHT_DIR 1
#define PHONG_LIGHT_POINT 2
#define PHONG_LIGHT_SPOT 3

struct PhongLight
{
    int lightType;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;
    vec3 spotDir;
    float spotExponent;
    float spotCutoff;
    float attenuationConst;
    float attenuationLin;
    float attenuationQuad;
};

struct PhongMaterial
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 emission;
    float shinines;
};

struct PhongMaterialTex
{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    sampler2D normal;
};

vec4 Phong_ComputeLight(vec3 position, vec3 normal, vec3 cameraPosition, PhongMaterial material, PhongLight light)
{
    if(light.lightType <= PHONG_LIGHT_NONE || light.lightType > PHONG_LIGHT_SPOT) {
        return vec4(0);
    }

    vec3 dirToCam = normalize(cameraPosition - position);
    vec3 dirToLight;
    float spotLightEffect = 1;
    float attenuationFactor = 1;
    if(light.lightType == PHONG_LIGHT_DIR) { // DIR light
        dirToLight = -light.spotDir;
        attenuationFactor = 1 / light.attenuationConst;
    } else { // POINT or SPOT light
        dirToLight = light.position.xyz - position;
        float dist = length(dirToLight);
        dirToLight = normalize(dirToLight);
        attenuationFactor = 1 / (light.attenuationConst + light.attenuationLin * dist + light.attenuationQuad * dist * dist);
        if(light.lightType == PHONG_LIGHT_SPOT) { // SPOT light
            float cosAlpha = dot(dirToLight, -light.spotDir);
            if(cosAlpha < cos(light.spotCutoff)) {
                spotLightEffect = 0;
            } else {
                spotLightEffect = pow(max(cosAlpha,0), light.spotExponent);
            }
        }
    }
    // equivalent to: vec3 reflected = -dirToLight + 2 * dot(dirToLight, normal) * normal;
    vec3 reflected = reflect(-dirToLight, normal);
    float specularExp = pow(max(dot(dirToCam, reflected),0), material.shinines);
    if(dot(reflected, normal) < 0) {
        specularExp = 0;
    }

    vec4 ambient = light.ambient * material.ambient;
    vec4 diffuse = max(dot(dirToLight, normal),0) * light.diffuse * material.diffuse;
    vec4 specular = specularExp * light.specular * material.specular;

    return spotLightEffect * attenuationFactor * (ambient + diffuse + specular);
}