#pragma once

#include "Skybox.h"

#include <glm/glm.hpp>
#include <string>

class Camera {
private:
	glm::vec3 m_Rotation;
	glm::vec3 m_Translation;

	float m_NearPlane;
	float m_FarPlane;

	float m_MovementSpeed;

	bool m_IsFocused;

	Skybox* m_Skybox;
public:
	Camera();
	~Camera();

	void SetNearPlane(float nearPlane);
	void SetFarPlane(float farPlane);
	void SetMovementSpeed(float movementSpeed);
	void SetTranslation(const glm::vec3& translation);
	void SetUpDirection(const glm::vec3& upDirection);
	void SetIsFocused(bool isFocused);
	void SetSkybox(const std::string& path, float gamma);

	const float GetNearPlane();
	const float GetFarPlane();
	const float GetMovementSpeed();
	const glm::vec3& GetTranslation();
	const glm::vec3& GetRotation();
	glm::vec3& GetTranslationRef();
	glm::vec3& GetRotationRef();
	float& GetNearPlaneRef();
	float& GetFarPlaneRef();
	float& GetMovementSpeedRef();
	glm::vec3 GetUpDirection();
	glm::vec3 GetForwardDirection();
	glm::mat4 GetProjectionMatrix(int width, int height);
	glm::mat4 GetViewMatrix();
	const bool GetIsFocused();
	Skybox* GetSkybox();

	void LookAtPosition(const glm::vec3& position);
	void LookInDirection(const glm::vec3& forwardDirection);
	void LookAtMouse(float sensitivity, float xPos, float yPos, float oldXPos, float oldYPos);

	void MoveForward(float deltaTime);
	void MoveBackward(float deltaTime);
	void StrafeLeft(float deltaTime);
	void StrafeRight(float deltaTime);
	void MoveUp(float deltaTime);
	void MoveDown(float deltaTime);

	void SetRotation(const glm::vec3& rotation);

	void AddTranslation(const glm::vec3& translation);
	void AddRotation(const glm::vec3& rotation);

	static Camera* DeepCopy(Camera* camera);
};
