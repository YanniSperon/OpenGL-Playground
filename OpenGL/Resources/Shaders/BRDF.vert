#version 460

layout (location = 0) in vec3 in_VertexPosition;
layout (location = 1) in vec2 in_VertexTexCoord;

out vec2 s_TexCoord;

void main()
{
    s_TexCoord = in_VertexTexCoord;
	gl_Position = vec4(in_VertexPosition, 1.0);
}