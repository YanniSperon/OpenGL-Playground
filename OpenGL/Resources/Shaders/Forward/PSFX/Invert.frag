#version 460

out vec4 out_Color;

in vec2 s_TexCoord;

layout(binding = 0) uniform sampler2D u_ScreenTexture;

void main()
{ 
	vec4 texSample = texture(u_ScreenTexture, s_TexCoord);
	out_Color = vec4(1.0 - texSample.rgb, texSample.a);
}