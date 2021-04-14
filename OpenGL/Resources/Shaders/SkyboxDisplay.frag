#version 460

out vec4 o_FragColor;
in vec3 s_FragPos;

layout(binding = 0) uniform samplerCube m_EnvironmentMap;

uniform float u_Gamma = 2.2;

void main()
{
    vec3 envColor = textureLod(m_EnvironmentMap, s_FragPos, 0.0).rgb;

    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/u_Gamma));

    o_FragColor = vec4(envColor, 1.0);
}
