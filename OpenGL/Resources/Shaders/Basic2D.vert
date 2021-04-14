#version 460

layout (location = 0) in vec2 in_VertexPosition;
layout (location = 1) in vec2 in_VertexTexCoord;

out vec2 s_TexCoord;

void main()
{
	gl_Position = vec4(in_VertexPosition, 0.0, 1.0);
	s_TexCoord = in_VertexTexCoord;
}