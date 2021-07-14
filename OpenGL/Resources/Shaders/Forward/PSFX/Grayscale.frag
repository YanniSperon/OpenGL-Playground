#version 460

out vec4 out_Color;

in vec2 s_TexCoord;

layout(binding = 0) uniform sampler2D u_ScreenTexture;

void main()
{ 
	vec4 texSample = texture(u_ScreenTexture, s_TexCoord);
	float average = 0.2126 * texSample.r + 0.7152 * texSample.g + 0.0722 * texSample.b;
	out_Color = vec4(average, average, average, texSample.a);
}