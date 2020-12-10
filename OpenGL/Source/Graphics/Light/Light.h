#pragma once

#include "Shader.h"

#include <memory>
#include <glm/glm.hpp>
#include <glew.h>

class Light {
private:
	glm::vec3 m_Translation;
	glm::vec3 m_Rotation;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

public:
	Light();
	~Light();

	void SetTranslation(const glm::vec3& translation);
	void SetRotation(const glm::vec3& rotation);
	void SetAmbient(const glm::vec3& ambient);
	void SetDiffuse(const glm::vec3& diffuse);
	void SetSpecular(const glm::vec3& specular);

	const glm::vec3& GetTranslation();
	const glm::vec3& GetRotation();

	const glm::vec3& GetAmbient();
	const glm::vec3& GetDiffuse();
	const glm::vec3& GetSpecular();

	glm::vec3& GetTranslationRef();
	glm::vec3& GetRotationRef();

	static Light* DeepCopy(Light* light);
};
