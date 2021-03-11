#pragma once

#include "Shader.h"
#include "Texture.h"
#include "Mesh3D.h"

#include "glm.hpp"

#include <glew.h>
#include <string>
#include <memory>

class Skybox {
private:
	std::shared_ptr<Texture> m_HDRTexture;
	std::shared_ptr<Mesh3D> m_CubeMesh;
	std::shared_ptr<Shader> m_RenderShader;
	std::shared_ptr<Shader> m_CubemapShader;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_FBO;
	GLuint m_RBO;
	GLuint m_CubeMap;
public:
	Skybox(const std::string& path);
	~Skybox();

	void PrepareEnvironmentMap();

	void Draw(const glm::mat4& projection, const glm::mat4& view);
};