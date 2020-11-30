#define vec3 float3

struct DirLight
{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

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
};

cbuffer DIR_LIGHT
{
    DirLight dirLight;
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

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float2 textu);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float2 textu);

float4 main(vec3 pos : POSITION, vec3 normal : NORMAL, float2 textu : TEXCOORD) : SV_TARGET
{
    // properties
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - pos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(dirLight, norm, viewDir, textu);
    // phase 2: Point lights
    for (int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, pos, viewDir, textu);
    // phase 3: Spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);    
    
    return float4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, float2 textu)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir),
    0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient = light.ambient * (vec3) tex[0].Sample(splr[0], textu);
    vec3 diffuse = light.diffuse * diff * (vec3) tex[0].Sample(splr[0], textu);
    vec3 specular = light.specular * spec * (vec3) tex[1].Sample(splr[1], textu);
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float2 textu)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linearT * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * (vec3) tex[0].Sample(splr[0], textu);
    vec3 diffuse = light.diffuse * diff * (vec3) tex[0].Sample(splr[0], textu);
    vec3 specular = light.specular * spec * (vec3) tex[1].Sample(splr[1], textu);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}