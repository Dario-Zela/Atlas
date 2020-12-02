#define vec3 float3

struct PointLight
{
    vec3 position;
    float padding;
    
    float constant;
    float linearT;
    float quadratic;
    float padding2;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

cbuffer SV_IN
{
    vec3 viewPos;
    vec3 viewDir;
    float padding;
    float cutOff;
    float outerCutOff;
};

#define NR_POINT_LIGHTS 4  
cbuffer POINT_LIGHTS
{
    PointLight
    pointLights[NR_POINT_LIGHTS];
}

Texture2D tex[2];

SamplerState splr[2];

cbuffer SV_IN3
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float2 textu);
vec3 CalcSpotLight(vec3 normal, vec3 fragPos, float2 textu);

float4 main(vec3 pos : POSITION, float2 textu : TEXCOORD) : SV_TARGET
{
    // properties
    vec3 norm = (vec3) tex[1].Sample(splr[1], textu);
    vec3 viewDir = normalize(viewPos - pos);
    vec3 result = vec3(0,0,0);
    
    // phase 1: Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, pos, viewDir, textu);
    // phase 2: Spot light
    result += CalcSpotLight(norm, pos, textu);    
    
    return float4(result, 1.0);
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float2 textu)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = 0.4f; //max(dot(normal, lightDir), -dot(normal, lightDir));
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linearT * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * (vec3) tex[0].Sample(splr[0], textu);
    vec3 diffuse = light.diffuse * diff * (vec3) tex[0].Sample(splr[0], textu);
    vec3 ReSpecular = light.specular * spec * 0;
    ambient *= attenuation;
    diffuse *= attenuation;
    ReSpecular *= attenuation;
    return (ambient * 0 + diffuse + ReSpecular);

}

vec3 CalcSpotLight(vec3 normal, vec3 fragPos, float2 textu)
{
    vec3 lightDir = normalize(viewPos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(viewPos - fragPos);

    // combine results
    vec3 resAmbient = (vec3) tex[0].Sample(splr[0], textu);
    vec3 resDiffuse = diff * (vec3) tex[0].Sample(splr[0], textu);
    vec3 resSpecular = spec * vec3(1, 1, 1) * 0;
    
    float theta = dot(lightDir, normalize(-viewDir));
    float epsilon = cutOff - outerCutOff;
    float intensity = clamp((theta - outerCutOff) / epsilon, 0.0, 1.0);
    
    resDiffuse *= intensity;
    resSpecular *= intensity;
    
    return (resAmbient + resDiffuse + resSpecular) * 0;
}