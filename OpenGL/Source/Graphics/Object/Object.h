#pragma once

#include "Mesh3D.h"
#include "Material.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <memory>
#include <string>

class Object {
private:
	std::shared_ptr<Mesh3D> m_Mesh;
	std::shared_ptr<Material> m_Material;

	glm::vec3 m_Rotation;
	glm::vec3 m_Translation;
	glm::vec3 m_Scale;

	bool m_IsDynamic;

	bool m_IsTranslucent;

	bool m_IsEnabled;

	glm::mat4 m_FinalTransformation;

public:
	Object(const std::string& meshPath, std::string shaderPath, int shaderType, std::string texturePath, int textureTypes, bool isDynamic);
	~Object();

	const glm::vec3& GetTranslation();
	const glm::vec3& GetRotation();
	const glm::vec3& GetScale();
	glm::vec3& GetTranslationRef();
	glm::vec3& GetRotationRef();
	glm::vec3& GetScaleRef();
	bool GetIsTranslucent();
	bool GetIsEnabled();

	Mesh3D& GetMesh();
	Material& GetMaterial();

	void Bind();

	void SetTranslation(const glm::vec3& translation);
	void SetRotation(const glm::vec3& rotation);
	void SetScale(const glm::vec3& scale);
	void SetIsTranslucent(bool isTranslucent);
	void SetIsEnabled(bool isEnabled);

	void AddTranslation(const glm::vec3& translation);
	void AddRotation(const glm::vec3& rotation);
	void AddScale(const glm::vec3& scale);

	glm::mat4 GetTransformationMatrix();
	glm::mat4 GetFinalTransformationMatrix();
	void LookAtPosition(const glm::vec3& position);
	void LookInDirection(const glm::vec3& forwardDirection);
	void LookInDirectionX(const glm::vec3& forwardDirection);

	void GenerateFinalTransformation(const glm::mat4& offset);
	void SetFinalTransformation(const glm::mat4& transformation);
	void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::vec3& translation);
	void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& modelOffset);

	static Object* DeepCopy(Object* object);
};
