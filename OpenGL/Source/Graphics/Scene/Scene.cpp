#include "Scene.h"
#include <algorithm>

Scene::Scene()
	: m_OpaqueObjects(), m_Lights(), m_Cameras(), m_ActiveCamera(0), m_ShouldUpdate(true), m_OpaquePipeline("Resources/Shaders/Deferred/PSFX/Basic", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER, 1920, 1080), m_TransparentPipeline("Resources/Shaders/Forward/PSFX/Basic", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER, 1920, 1080)
{
	m_Cameras.push_back(new Camera());
}

Scene::~Scene()
{
	for (std::size_t i = 0; i < m_OpaqueObjects.size(); ++i) {
		delete m_OpaqueObjects[i];
	}
	for (std::size_t i = 0; i < m_TransparentObjects.size(); ++i) {
		delete m_TransparentObjects[i];
	}
	for (std::size_t i = 0; i < m_Lights.size(); ++i) {
		delete m_Lights[i];
	}
	for (std::size_t i = 0; i < m_Cameras.size(); ++i) {
		delete m_Cameras[i];
	}
}

DeferredPipeline& Scene::GetOpaquePipeline()
{
	return m_OpaquePipeline;
}

ForwardPipeline& Scene::GetTransparentPipeline()
{
	return m_TransparentPipeline;
}

void Scene::Update(float deltaTime)
{
	for (std::size_t i = 0; i < m_OpaqueObjects.size(); ++i) {
		m_OpaqueObjects[i]->GenerateFinalTransformation(glm::mat4(1.0f));
	}

	for (std::size_t i = 0; i < m_TransparentObjects.size(); ++i) {
		m_TransparentObjects[i]->GenerateFinalTransformation(glm::mat4(1.0f));
	}
}

Camera* Scene::GetActiveCamera()
{
	return m_Cameras[m_ActiveCamera];
}

const std::size_t Scene::GetActiveCameraIndex() const
{
	return m_ActiveCamera;
}

void Scene::SetActiveCamera(std::size_t index)
{
	if (index < m_Cameras.size()) {
		m_ActiveCamera = index;
	}
}

void Scene::IncrementActiveCamera()
{
	m_ActiveCamera++;
	if (m_ActiveCamera >= m_Cameras.size()) {
		m_ActiveCamera = 0;
	}
}

void Scene::DecrementActiveCamera()
{
	if (m_ActiveCamera == 0) {
		m_ActiveCamera = m_Cameras.size() - 1;
	}
	else {
		m_ActiveCamera--;
	}
}

void Scene::AddOpaqueObject(Object* object)
{
	m_OpaqueObjects.push_back(object);
}

void Scene::AddOpaqueObject(Object* object, std::size_t index)
{
	m_OpaqueObjects.insert(m_OpaqueObjects.begin() + index, object);
}

void Scene::AddTransparentObject(Object* object)
{
	m_TransparentObjects.push_back(object);
}

void Scene::AddTransparentObject(Object* object, std::size_t index)
{
	m_TransparentObjects.insert(m_TransparentObjects.begin() + index, object);
}

void Scene::AddLight(Light* light)
{
	m_Lights.push_back(light);
}

void Scene::AddLight(Light* light, std::size_t index)
{
	m_Lights.insert(m_Lights.begin() + index, light);
}

void Scene::AddCamera(Camera* camera)
{
	m_Cameras.push_back(camera);
}

void Scene::AddCamera(Camera* camera, std::size_t index)
{
	m_Cameras.insert(m_Cameras.begin() + index, camera);
	if (m_ActiveCamera >= index) {
		IncrementActiveCamera();
	}
}

Object* Scene::RemoveObject(Object* object)
{
	for (std::size_t i = 0; i < m_OpaqueObjects.size(); ++i) {
		if (object == m_OpaqueObjects[i]) {
			m_OpaqueObjects.erase(m_OpaqueObjects.begin() + i);
			return object;
		}
	}

	for (std::size_t i = 0; i < m_TransparentObjects.size(); ++i) {
		if (object == m_TransparentObjects[i]) {
			m_TransparentObjects.erase(m_TransparentObjects.begin() + i);
			return object;
		}
	}

	return object;
}

Object* Scene::RemoveOpaqueObject(Object* object)
{
	for (std::size_t i = 0; i < m_OpaqueObjects.size(); ++i) {
		if (object == m_OpaqueObjects[i]) {
			m_OpaqueObjects.erase(m_OpaqueObjects.begin() + i);
			return object;
		}
	}
	return object;
}

Object* Scene::RemoveTransparentObject(Object* object)
{
	for (std::size_t i = 0; i < m_TransparentObjects.size(); ++i) {
		if (object == m_TransparentObjects[i]) {
			m_TransparentObjects.erase(m_TransparentObjects.begin() + i);
			return object;
		}
	}
	return object;
}

Light* Scene::RemoveLight(Light* light)
{
	for (std::size_t i = 0; i < m_Lights.size(); ++i) {
		if (light == m_Lights[i]) {
			m_Lights.erase(m_Lights.begin() + i);
			return light;
		}
	}
	return light;
}

Camera* Scene::RemoveCamera(Camera* camera)
{
	if (m_Cameras.size() != 1) {
		for (std::size_t i = 0; i < m_Cameras.size(); ++i) {
			if (camera == m_Cameras[i]) {
				m_Cameras.erase(m_Cameras.begin() + i);
				if (m_ActiveCamera >= i) {
					DecrementActiveCamera();
				}
				return camera;
			}
		}
	}
	return camera;
}

Object* Scene::RemoveOpaqueObject(std::size_t index)
{
	Object* temp = m_OpaqueObjects[index];
	m_OpaqueObjects.erase(m_OpaqueObjects.begin() + index);
	return temp;
}

Object* Scene::RemoveTransparentObject(std::size_t index)
{
	Object* temp = m_TransparentObjects[index];
	m_TransparentObjects.erase(m_TransparentObjects.begin() + index);
	return temp;
}

Light* Scene::RemoveLight(std::size_t index)
{
	Light* temp = m_Lights[index];
	m_Lights.erase(m_Lights.begin() + index);
	return temp;
}

Camera* Scene::RemoveCamera(std::size_t index)
{
	if (m_Cameras.size() != 1) {
		Camera* temp = m_Cameras[index];
		m_Cameras.erase(m_Cameras.begin() + index);
		if (m_ActiveCamera >= index) {
			DecrementActiveCamera();
		}
		return temp;
	}
	else {
		return nullptr;
	}
}

Object* Scene::RemoveOpaqueObjectAt(std::size_t index)
{
	if (index < m_OpaqueObjects.size()) {
		Object* temp = m_OpaqueObjects[index];
		m_OpaqueObjects.erase(m_OpaqueObjects.begin() + index);
		return temp;
	}
	else {
		return nullptr;
	}
}

Object* Scene::RemoveTransparentObjectAt(std::size_t index)
{
	if (index < m_TransparentObjects.size()) {
		Object* temp = m_TransparentObjects[index];
		m_TransparentObjects.erase(m_TransparentObjects.begin() + index);
		return temp;
	}
	else {
		return nullptr;
	}
}

Light* Scene::RemoveLightAt(std::size_t index)
{
	if (index < m_Lights.size()) {
		Light* temp = m_Lights[index];
		m_Lights.erase(m_Lights.begin() + index);
		return temp;
	}
	else {
		return nullptr;
	}
}

Camera* Scene::RemoveCameraAt(std::size_t index)
{
	if (index < m_Cameras.size() && m_Cameras.size() != 1) {
		Camera* temp = m_Cameras[index];
		m_Cameras.erase(m_Cameras.begin() + index);
		if (m_ActiveCamera >= index) {
			DecrementActiveCamera();
		}
		return temp;
	}
	else {
		return nullptr;
	}
}

void Scene::DeleteOpaqueObject(std::size_t index)
{
	Object* temp = m_OpaqueObjects[index];
	m_OpaqueObjects.erase(m_OpaqueObjects.begin() + index);
	delete temp;
}

void Scene::DeleteTransparentObject(std::size_t index)
{
	Object* temp = m_TransparentObjects[index];
	m_TransparentObjects.erase(m_TransparentObjects.begin() + index);
	delete temp;
}

void Scene::DeleteLight(std::size_t index)
{
	Light* temp = m_Lights[index];
	m_Lights.erase(m_Lights.begin() + index);
	delete temp;
}

void Scene::DeleteCamera(std::size_t index)
{
	if (m_Cameras.size() != 1) {
		Camera* temp = m_Cameras[index];
		m_Cameras.erase(m_Cameras.begin() + index);
		if (m_ActiveCamera >= index) {
			DecrementActiveCamera();
		}
		delete temp;
	}
}

void Scene::DeleteOpaqueObjectAt(std::size_t index)
{
	if (index < m_OpaqueObjects.size()) {
		Object* temp = m_OpaqueObjects[index];
		m_OpaqueObjects.erase(m_OpaqueObjects.begin() + index);
		delete temp;
	}
}

void Scene::DeleteTransparentObjectAt(std::size_t index)
{
	if (index < m_TransparentObjects.size()) {
		Object* temp = m_TransparentObjects[index];
		m_TransparentObjects.erase(m_TransparentObjects.begin() + index);
		delete temp;
	}
}

void Scene::DeleteLightAt(std::size_t index)
{
	if (index < m_Lights.size()) {
		Light* temp = m_Lights[index];
		m_Lights.erase(m_Lights.begin() + index);
		delete temp;
	}
}

void Scene::DeleteCameraAt(std::size_t index)
{
	if (index < m_Cameras.size() && m_Cameras.size() != 1) {
		Camera* temp = m_Cameras[index];
		m_Cameras.erase(m_Cameras.begin() + index);
		if (m_ActiveCamera >= index) {
			DecrementActiveCamera();
		}
		delete temp;
	}
}

Object* Scene::GetOpaqueObject(std::size_t index)
{
	return m_OpaqueObjects[index];
}

Object* Scene::GetOpaqueObjectAt(std::size_t index)
{
	if (index < m_OpaqueObjects.size()) {
		return m_OpaqueObjects[index];
	}
	else {
		return nullptr;
	}
}

const std::size_t Scene::GetNumOpaqueObjects() const
{
	return m_OpaqueObjects.size();
}

std::vector<Object*>& Scene::GetOpaqueObjects()
{
	return m_OpaqueObjects;
}

void Scene::MarkObjectTransparent(std::size_t index)
{
	Object* temp = m_OpaqueObjects[index];
	m_OpaqueObjects.erase(m_OpaqueObjects.begin() + index);
	m_TransparentObjects.push_back(temp);
}

void Scene::MarkObjectTransparentAt(std::size_t index)
{
	if (index < m_OpaqueObjects.size()) {
		Object* temp = m_OpaqueObjects[index];
		m_OpaqueObjects.erase(m_OpaqueObjects.begin() + index);
		m_TransparentObjects.push_back(temp);
	}
}

void Scene::MarkObjectTransparent(Object* obj)
{
	for (std::size_t i = 0; i < m_OpaqueObjects.size(); ++i) {
		if (obj == m_OpaqueObjects[i]) {
			Object* temp = m_OpaqueObjects[i];
			m_OpaqueObjects.erase(m_OpaqueObjects.begin() + i);
			m_TransparentObjects.push_back(temp);
			break;
		}
	}
}

void Scene::MarkObjectOpaque(std::size_t index)
{
	Object* temp = m_TransparentObjects[index];
	m_TransparentObjects.erase(m_TransparentObjects.begin() + index);
	m_OpaqueObjects.push_back(temp);
}

void Scene::MarkObjectOpaqueAt(std::size_t index)
{
	if (index < m_TransparentObjects.size()) {
		Object* temp = m_TransparentObjects[index];
		m_TransparentObjects.erase(m_TransparentObjects.begin() + index);
		m_OpaqueObjects.push_back(temp);
	}
}

void Scene::MarkObjectOpaque(Object* obj)
{
	for (std::size_t i = 0; i < m_TransparentObjects.size(); ++i) {
		if (obj == m_TransparentObjects[i]) {
			Object* temp = m_TransparentObjects[i];
			m_TransparentObjects.erase(m_TransparentObjects.begin() + i);
			m_OpaqueObjects.push_back(temp);
			break;
		}
	}
}

Object* Scene::GetTransparentObject(std::size_t index)
{
	return m_TransparentObjects[index];
}

Object* Scene::GetTransparentObjectAt(std::size_t index)
{
	if (index < m_TransparentObjects.size()) {
		return m_TransparentObjects[index];
	}
	else {
		return nullptr;
	}
}

const std::size_t Scene::GetNumTransparentObjects() const
{
	return m_TransparentObjects.size();
}

std::vector<Object*>& Scene::GetTransparentObjects()
{
	return m_TransparentObjects;
}

Camera* Scene::GetCamera(std::size_t index)
{
	return m_Cameras[index];
}

Camera* Scene::GetCameraAt(std::size_t index)
{
	if (index < m_Cameras.size()) {
		return m_Cameras[index];
	}
	else {
		return nullptr;
	}
}

const std::size_t Scene::GetNumCameras() const
{
	return m_Cameras.size();
}

std::vector<Camera*>& Scene::GetCameras()
{
	return m_Cameras;
}

Light* Scene::GetLight(std::size_t index)
{
	return m_Lights[index];
}

Light* Scene::GetLightAt(std::size_t index)
{
	if (index < m_Lights.size()) {
		return m_Lights[index];
	}
	else {
		return nullptr;
	}
}

const std::size_t Scene::GetNumLights() const
{
	return m_Lights.size();
}

std::vector<Light*>& Scene::GetLights()
{
	return m_Lights;
}

void Scene::Render(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Camera& camera = *m_Cameras[m_ActiveCamera];
	glm::mat4 projection = camera.GetProjectionMatrix(width, height);
	glm::mat4 view = camera.GetViewMatrix();
	glm::vec3 cameraPosition = camera.GetTranslation();

	m_TransparentPipeline.Prepare(width, height);

	Skybox* skybox = camera.GetSkybox();
	GLuint lastID = 0;

	for (std::size_t i = 0; i < m_OpaqueObjects.size(); ++i) {
		Object* renderable = m_OpaqueObjects[i];
		if (renderable->GetIsEnabled()) {
			renderable->Bind();
			skybox->BindIrradianceMap(5);
			skybox->BindPrefilterMap(6);
			skybox->BindBRDFLUT(7);

			Shader& shader = renderable->GetMaterial().GetShader();

			if (shader.GetID() != lastID) {
				std::string lightPositionsPrefix = "u_LightPositions[";
				std::string lightColorsPrefix = "u_LightColors[";
				char lightArraysSuffix = ']';
				for (std::size_t i = 0; i < m_Lights.size(); ++i) {
					std::string num = std::to_string(i);
					shader.SetVec3(lightPositionsPrefix + num + lightArraysSuffix, m_Lights[i]->GetTranslation());
					shader.SetVec3(lightColorsPrefix + num + lightArraysSuffix, m_Lights[i]->GetColor());
				}
				shader.SetInt("u_NumLights", m_Lights.size());

				shader.SetVec3("u_CamPos", cameraPosition);
				shader.SetMat4("u_V", view);
				shader.SetMat4("u_P", projection);

				lastID = shader.GetID();
			}

			shader.SetMat4("u_M", renderable->GetFinalTransformationMatrix());

			renderable->GetMesh().Draw();
		}
	}

	if (skybox) {
		skybox->Draw(projection, view);
	}

	std::sort(m_TransparentObjects.begin(), m_TransparentObjects.end(), [&cameraPosition](Object* obj1, Object* obj2) {
		return glm::length2(cameraPosition - obj1->GetTranslation()) < glm::length2(cameraPosition - obj2->GetTranslation());
		});

	for (std::size_t i = 0; i < m_OpaqueObjects.size(); ++i) {
		Object* renderable = m_OpaqueObjects[i];
		if (renderable->GetIsEnabled()) {
			renderable->Bind();
			skybox->BindIrradianceMap(5);
			skybox->BindPrefilterMap(6);
			skybox->BindBRDFLUT(7);

			Shader& shader = renderable->GetMaterial().GetShader();

			if (shader.GetID() != lastID) {
				std::string lightPositionsPrefix = "u_LightPositions[";
				std::string lightColorsPrefix = "u_LightColors[";
				char lightArraysSuffix = ']';
				for (std::size_t i = 0; i < m_Lights.size(); ++i) {
					std::string num = std::to_string(i);
					shader.SetVec3(lightPositionsPrefix + num + lightArraysSuffix, m_Lights[i]->GetTranslation());
					shader.SetVec3(lightColorsPrefix + num + lightArraysSuffix, m_Lights[i]->GetColor());
				}
				shader.SetInt("u_NumLights", m_Lights.size());

				shader.SetVec3("u_CamPos", cameraPosition);
				shader.SetMat4("u_V", view);
				shader.SetMat4("u_P", projection);

				lastID = shader.GetID();
			}

			shader.SetMat4("u_M", renderable->GetFinalTransformationMatrix());

			renderable->GetMesh().Draw();
		}
	}
	m_TransparentPipeline.Flush();
}
