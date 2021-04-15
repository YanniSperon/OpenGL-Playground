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
	std::shared_ptr<Shader> m_CubeMapShader;
	std::shared_ptr<Shader> m_IrradianceShader;
	std::shared_ptr<Shader> m_PrefilterShader;
	std::shared_ptr<Shader> m_BRDFShader;

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_FBO;
	GLuint m_RBO;

	GLuint m_EnvironmentMap;
	GLuint m_IrradianceMap;
	GLuint m_PrefilterMap;

	GLuint m_BRDFLookUpTexture;

	float m_Gamma;

	bool m_FakeUser;
public:
	Skybox(const std::string& path);
	~Skybox();

	void Draw(const glm::mat4& projection, const glm::mat4& view);

	float GetGamma();
	void SetGamma(float gamma);

	void BindIrradianceMap(int slot);
	void BindPrefilterMap(int slot);
	void BindBRDFLUT(int slot);

	static void Unbind();

	void SetHasFakeUser(bool fakeUser);
	bool GetHasFakeUser();

	void RenderBRDF();
private:
	void PrepareFramebuffer();
	void PrepareEnvironmentMap();
	void PrepareIrradianceMap();
	void PreparePrefilterMap();
	void PrepareBRDFLUT();
};