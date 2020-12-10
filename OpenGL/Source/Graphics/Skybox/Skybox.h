#pragma once

#include "Shader.h"

#include "glm.hpp"

#include <glew.h>
#include <string>
#include <memory>

class Skybox {
private:
	GLuint m_HDRTexture;
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_FBO;
	GLuint m_RBO;
	GLuint m_CubeMap;

	glm::mat4 m_Projection;
	glm::mat4* m_Views;
	
	std::shared_ptr<Shader> m_IBLShader;
	std::shared_ptr<Shader> m_RenderShader;
public:
	Skybox(const std::string& path);
	~Skybox();

	void PrepareEnvironmentMap();

	void Draw(const glm::mat4& projection, const glm::mat4& view);
};