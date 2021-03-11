#include "Camera.h"
#include "Global.h"
#include "Console.h"

#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/gtx/euler_angles.hpp"
#include "Vendor/glm/gtx/rotate_vector.hpp"

Camera::Camera()
	: m_NearPlane(0.1f), m_FarPlane(250.0f), m_MovementSpeed(5.0f), m_Translation(0.0f), m_IsFocused(false), m_Rotation(-270.0f, 0.0f, 0.0f), m_Skybox(nullptr)
{

}

Camera::~Camera()
{

}

void Camera::SetNearPlane(float nearPlane)
{
	m_NearPlane = nearPlane;
}

void Camera::SetFarPlane(float farPlane)
{
	m_FarPlane = farPlane;
}

void Camera::SetMovementSpeed(float movementSpeed)
{
	m_MovementSpeed = movementSpeed;
}

void Camera::SetTranslation(const glm::vec3& translation)
{
	m_Translation = translation;
}

void Camera::SetUpDirection(const glm::vec3& upDirection)
{
	m_Rotation.z = glm::degrees(glm::acos(glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), upDirection)));
}

void Camera::SetIsFocused(bool isFocused)
{
	m_IsFocused = isFocused;
}

void Camera::SetSkybox(const std::string& path, float gamma)
{
	if (m_Skybox) {
		delete m_Skybox;
	}
	m_Skybox = new Skybox(path, gamma);
}

const float Camera::GetNearPlane()
{
	return m_NearPlane;
}

const float Camera::GetFarPlane()
{
	return m_FarPlane;
}

const float Camera::GetMovementSpeed()
{
	return m_MovementSpeed;
}

const glm::vec3& Camera::GetTranslation()
{
	return m_Translation;
}

const glm::vec3& Camera::GetRotation()
{
	return m_Rotation;
}

glm::vec3& Camera::GetTranslationRef()
{
	return m_Translation;
}

glm::vec3& Camera::GetRotationRef()
{
	return m_Rotation;
}

float& Camera::GetNearPlaneRef()
{
	return m_NearPlane;
}

float& Camera::GetFarPlaneRef()
{
	return m_FarPlane;
}

float& Camera::GetMovementSpeedRef()
{
	return m_MovementSpeed;
}

glm::vec3 Camera::GetUpDirection()
{
	return glm::rotateZ(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(-m_Rotation.z));
}

glm::vec3 Camera::GetForwardDirection()
{
	return glm::normalize(glm::vec3(glm::cos(glm::radians(m_Rotation.x)) * glm::cos(glm::radians(m_Rotation.y)), glm::sin(glm::radians(m_Rotation.y)), glm::sin(glm::radians(m_Rotation.x)) * glm::cos(glm::radians(m_Rotation.y))));
}

glm::mat4 Camera::GetProjectionMatrix(int width, int height)
{
	return glm::perspective(glm::radians(90.0f), (float)width / (float)height, m_NearPlane, m_FarPlane);
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_Translation, m_Translation + GetForwardDirection(), GetUpDirection());
}

const bool Camera::GetIsFocused()
{
	return m_IsFocused;
}

Skybox* Camera::GetSkybox()
{
	return m_Skybox;
}

void Camera::LookAtPosition(const glm::vec3& position)
{
	glm::vec3 newDirection = glm::normalize(m_Translation - position);

	m_Rotation.x = glm::degrees(atan2(newDirection.z, newDirection.x)) + 180.0f;
	m_Rotation.y = glm::degrees(-glm::asin(newDirection.y));
}

void Camera::LookInDirection(const glm::vec3& forwardDirection)
{
	m_Rotation.x = glm::degrees(atan2(forwardDirection.z, forwardDirection.x)) + 180.0f;
	m_Rotation.y = glm::degrees(-glm::asin(forwardDirection.y));
}

void Camera::LookAtMouse(float sensitivity, float xPos, float yPos, float oldXPos, float oldYPos)
{
	float xOffset = xPos - oldXPos;
	float yOffset = oldYPos - yPos;
	m_Rotation.x += xOffset * sensitivity;
	m_Rotation.y += yOffset * sensitivity;

	if (m_Rotation.y > 89.0f) {
		m_Rotation.y = 89.0f;
	}
	if (m_Rotation.y < -89.0f) {
		m_Rotation.y = -89.0f;
	}
}

void Camera::MoveForward(float deltaTime)
{
	glm::vec3 forwardDirection = GetForwardDirection();
	glm::vec2 normalizedViewDirection = glm::normalize(glm::vec2(forwardDirection.x, forwardDirection.z));
	m_Translation.x += m_MovementSpeed * normalizedViewDirection.x * deltaTime;
	m_Translation.z += m_MovementSpeed * normalizedViewDirection.y * deltaTime;
}

void Camera::MoveBackward(float deltaTime)
{
	glm::vec3 forwardDirection = GetForwardDirection();
	glm::vec2 normalizedViewDirection = glm::normalize(glm::vec2(forwardDirection.x, forwardDirection.z));
	m_Translation.x += -m_MovementSpeed * normalizedViewDirection.x * deltaTime;
	m_Translation.z += -m_MovementSpeed * normalizedViewDirection.y * deltaTime;
}

void Camera::StrafeLeft(float deltaTime)
{
	glm::vec3 forwardDirection = GetForwardDirection();
	glm::vec3 strafeDirection = glm::cross(forwardDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec2 normalizedStrafeDirection = glm::normalize(glm::vec2(strafeDirection.x, strafeDirection.z));
	m_Translation.x += -m_MovementSpeed * normalizedStrafeDirection.x * deltaTime;
	m_Translation.z += -m_MovementSpeed * normalizedStrafeDirection.y * deltaTime;
}

void Camera::StrafeRight(float deltaTime)
{
	glm::vec3 forwardDirection = GetForwardDirection();
	glm::vec3 strafeDirection = glm::cross(forwardDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec2 normalizedStrafeDirection = glm::normalize(glm::vec2(strafeDirection.x, strafeDirection.z));
	m_Translation.x += m_MovementSpeed * normalizedStrafeDirection.x * deltaTime;
	m_Translation.z += m_MovementSpeed * normalizedStrafeDirection.y * deltaTime;
}

void Camera::MoveUp(float deltaTime)
{
	m_Translation += m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime;
}

void Camera::MoveDown(float deltaTime)
{
	m_Translation += -m_MovementSpeed * glm::vec3(0.0f, 1.0f, 0.0f) * deltaTime;
}

void Camera::SetRotation(const glm::vec3& rotation)
{
	m_Rotation = rotation;
}

void Camera::AddTranslation(const glm::vec3& translation)
{
	m_Translation += translation;
}

void Camera::AddRotation(const glm::vec3& rotation)
{
	m_Rotation += rotation;
}

Camera* Camera::DeepCopy(Camera* camera)
{
	Camera* copy = new Camera();
	return copy;
}
