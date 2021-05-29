#include "Light.h"

Light::Light()
	: m_Translation(0.0f), m_Color(300.0f, 300.0f, 300.0f), m_IsEnabled(true)
{
}

Light::~Light()
{

}

void Light::SetTranslation(const glm::vec3& translation)
{
	m_Translation = translation;
}

void Light::SetColor(const glm::vec3& color)
{
	m_Color = color;
}

void Light::SetIsEnabled(bool isEnabled)
{
	m_IsEnabled = isEnabled;
}

void Light::AddTranslation(const glm::vec3& translation)
{
	m_Translation += translation;
}

void Light::AddColor(const glm::vec3& color)
{
	m_Color += color;
}

const glm::vec3& Light::GetTranslation() const
{
	return m_Translation;
}

const glm::vec3& Light::GetColor() const
{
	return m_Color;
}

const bool Light::GetIsEnabled() const
{
	return m_IsEnabled;
}

glm::vec3& Light::GetTranslationRef()
{
	return m_Translation;
}

glm::vec3& Light::GetColorRef()
{
	return m_Color;
}

bool& Light::GetIsEnabledRef()
{
	return m_IsEnabled;
}

Light* Light::DeepCopy(Light* light)
{
	Light* copy = new Light();
	copy->m_Translation = light->m_Translation;
	copy->m_Color = light->m_Color;
	copy->m_IsEnabled = light->m_IsEnabled;
	return copy;
}
