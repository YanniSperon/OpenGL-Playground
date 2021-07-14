#version 460

out vec4 out_Color;

in vec2 s_TexCoord;

layout(binding = 0) uniform sampler2D u_ScreenTexture;

const float offset = 1.0 / 1920.0;

void main()
{
    vec2 offsets[25] = vec2[](
        vec2(-offset*2.0f, offset*2.0f),  vec2(-offset, offset*2.0f),  vec2( 0.0f, offset*2.0f),  vec2(offset, offset*2.0f),  vec2(offset*2.0f, offset*2.0f), // top
        vec2(-offset*2.0f, offset),       vec2(-offset, offset),       vec2( 0.0f, offset),       vec2(offset, offset),       vec2(offset*2.0f, offset),      // center-top
        vec2(-offset*2.0f, 0.0f),         vec2(-offset, 0.0f),         vec2( 0.0f, 0.0f),         vec2(offset, 0.0f),         vec2(offset*2.0f, 0.0f),        // center
        vec2(-offset*2.0f, -offset),      vec2(-offset, -offset),      vec2( 0.0f, -offset),      vec2(offset, -offset),      vec2(offset*2.0f, -offset),     // center-bottom
        vec2(-offset*2.0f, -offset*2.0f), vec2(-offset, -offset*2.0f), vec2( 0.0f, -offset*2.0f), vec2(offset, -offset*2.0f), vec2(offset*2.0f, -offset*2.0f) // bottom
    );
    
    float kernel[25] = float[](
        1.0/256.0,  4.0/256.0,  6.0/256.0,  4.0/256.0, 1.0/256.0,
        4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0,
        6.0/256.0, 24.0/256.0, 36.0/256.0, 24.0/256.0, 6.0/256.0,
        4.0/256.0, 16.0/256.0, 24.0/256.0, 16.0/256.0, 4.0/256.0,
        1.0/256.0,  4.0/256.0,  6.0/256.0,  4.0/256.0, 1.0/256.0
    );
    
    vec3 sampleTex[25];
    for(int i = 0; i < 25; i++)
    {
        sampleTex[i] = vec3(texture(u_ScreenTexture, s_TexCoord.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 25; i++) {
        col += sampleTex[i] * kernel[i];
    }
    
    out_Color = vec4(col, 1.0);
}