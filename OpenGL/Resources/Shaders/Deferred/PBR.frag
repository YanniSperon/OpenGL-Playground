#version 460

out vec4 out_Color;

in vec2 s_TexCoord;
in vec3 s_Normal;
in vec3 s_FragPos;

// Material
layout(binding = 0) uniform sampler2D u_Albedo;
layout(binding = 1) uniform sampler2D u_Normal;
layout(binding = 2) uniform sampler2D u_Metallic;
layout(binding = 3) uniform sampler2D u_Roughness;
layout(binding = 4) uniform sampler2D u_AO;

// IBL
layout(binding = 5) uniform samplerCube u_IrradianceMap;
layout(binding = 6) uniform samplerCube u_PrefilterMap;
layout(binding = 7) uniform sampler2D u_BRDFLUT;

uniform vec3 u_CamPos;

#define GAMMA 2.2
#define MAX_LIGHTS 100
#define ATTENUATION_MINIMUM 0.001f

// Lights
uniform vec3 u_LightPositions[MAX_LIGHTS];
uniform vec3 u_LightColors[MAX_LIGHTS];

uniform int u_NumLights;

const float c_PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(u_Normal, s_TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(s_FragPos); // immediate rate of change of the fragment position in the x direction
    vec3 Q2  = dFdy(s_FragPos);
    vec2 st1 = dFdx(s_TexCoord);
    vec2 st2 = dFdy(s_TexCoord);

    vec3 N = normalize(s_Normal);
    vec3 T = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = c_PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;
    // k becomes a^2/2.0 when IBL

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - min(cosTheta, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
    vec4 albedoRaw = texture(u_Albedo, s_TexCoord);
    vec3 albedo = pow(albedoRaw.rgb, vec3(GAMMA));
    float metallic = texture(u_Metallic, s_TexCoord).r;
    float roughness = texture(u_Roughness, s_TexCoord).r;
    float ao = texture(u_AO, s_TexCoord).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(u_CamPos - s_FragPos);
    vec3 R = reflect(-V, N);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < u_NumLights; ++i) 
    {
        float dist = length(u_LightPositions[i] - s_FragPos);
        float attenuation = 1.0 / (dist * dist);
        vec3 radiance = u_LightColors[i] * attenuation;
        float largestRadianceComp = max(max(radiance.r, radiance.g), radiance.b);

        if (largestRadianceComp < ATTENUATION_MINIMUM) {
            continue;
        }
        // calculate per-light radiance
        vec3 L = normalize(u_LightPositions[i] - s_FragPos);
        vec3 H = normalize(V + L);

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G   = GeometrySmith(N, V, L, roughness);
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
        
        vec3 nominator = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
        vec3 specular = nominator / denominator;
        
         // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;
            
        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);

        // add to outgoing radiance Lo
        Lo += (kD * albedo / c_PI + specular) * radiance * NdotL; // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }

    // ambient lighting (we now use IBL as the ambient term)
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(u_IrradianceMap, N).rgb;
    vec3 diffuse      = irradiance * albedo;
    
    // sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilteredColor = textureLod(u_PrefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(u_BRDFLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
    
    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/GAMMA));

    out_Color = vec4(color, albedoRaw.a);
}