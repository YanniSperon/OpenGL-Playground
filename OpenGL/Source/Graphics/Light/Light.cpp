#include "Light.h"
#include "ShaderManager.h"

#include "glm/gtc/matrix_transform.hpp"


Light::Light()
	: m_Rotation(0.0f), m_Translation(0.0f), m_Ambient(0.2f), m_Diffuse(0.5f), m_Specular(1.0f)
{
}

Light::~Light()
{

}

void Light::SetTranslation(const glm::vec3& translation)
{
	m_Translation = translation;
}

void Light::SetRotation(const glm::vec3& rotation)
{
	m_Rotation = rotation;
}

void Light::SetAmbient(const glm::vec3& ambient)
{
	m_Ambient = ambient;
}

void Light::SetDiffuse(const glm::vec3& diffuse)
{
	m_Diffuse = diffuse;
}

void Light::SetSpecular(const glm::vec3& specular)
{
	m_Specular = specular;
}

const glm::vec3& Light::GetTranslation()
{
	return m_Translation;
}

const glm::vec3& Light::GetRotation()
{
	return m_Rotation;
}

const glm::vec3& Light::GetAmbient()
{
	return m_Ambient;
}

const glm::vec3& Light::GetDiffuse()
{
	return m_Diffuse;
}

const glm::vec3& Light::GetSpecular()
{
	return m_Specular;
}

glm::vec3& Light::GetTranslationRef()
{
	return m_Translation;
}

glm::vec3& Light::GetRotationRef()
{
	return m_Rotation;
}

Light* Light::DeepCopy(Light* light)
{
	Light* copy = new Light();
	return copy;
}
