#version 460

out vec4 out_Color;
in vec2 s_TexCoord;

layout(binding = 0) uniform sampler2D u_Albedo;

void main()
{
	out_Color = texture(u_Albedo, s_TexCoord);
}