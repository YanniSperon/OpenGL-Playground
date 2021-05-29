#pragma once

#include <glm/glm.hpp>

class Light {
private:
	glm::vec3 m_Translation;
	glm::vec3 m_Color;

	bool m_IsEnabled;

public:
	Light();
	~Light();

	void SetTranslation(const glm::vec3& translation);
	void SetColor(const glm::vec3& color);
	void SetIsEnabled(bool isEnabled);

	void AddTranslation(const glm::vec3& translation);
	void AddColor(const glm::vec3& color);

	const glm::vec3& GetTranslation() const;
	const glm::vec3& GetColor() const;
	const bool GetIsEnabled() const;

	glm::vec3& GetTranslationRef();
	glm::vec3& GetColorRef();
	bool& GetIsEnabledRef();

	static Light* DeepCopy(Light* light);
};
