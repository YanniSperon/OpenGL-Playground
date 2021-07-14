#pragma once

#include <glew.h>
#include <memory>

#include "Shader.h"
#include "Pipeline.h"

class ForwardPipeline : public Pipeline {
private:
	std::shared_ptr<Shader> m_PSFXShader;

	GLuint m_FBO;
	GLuint m_RBO;

	GLuint m_Texture;

	GLuint m_QuadVBO;
	GLuint m_QuadVAO;
public:
	ForwardPipeline(const std::string& psfxShaderPath, int psfxShaderType, unsigned int width, unsigned int height);
	virtual ~ForwardPipeline() override;

	void SetPSFXShader(const std::string& path, int type);

	virtual void Prepare(unsigned int width, unsigned int height) override;
	virtual void Flush() override;
};
