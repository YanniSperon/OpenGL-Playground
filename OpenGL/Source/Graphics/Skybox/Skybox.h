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
	std::shared_ptr<Shader> m_IrradianceShader;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_FBO;
	GLuint m_RBO;

	GLuint m_CubeMap;
	GLuint m_IrradianceMap;

	float m_Gamma;
public:
	Skybox(const std::string& path, float gamma);
	~Skybox();

	void Draw(const glm::mat4& projection, const glm::mat4& view);

	void BindIrradianceMap(int slot);

	float GetGamma();
	void SetGamma(float gamma);
private:
	void GenerateFramebuffer();
	void GenerateMaps();
	void RenderMaps();
};