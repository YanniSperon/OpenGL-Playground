#include "LinearRenderer.h"
#include "Console.h"

#include <algorithm>

LinearRenderer::LinearRenderer()
	: Renderer()
{

}

void LinearRenderer::Flush(int width, int height, Camera& camera)
{
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = camera.GetProjectionMatrix(width, height);
	glm::mat4 view = camera.GetViewMatrix();
	glm::vec3 cameraPosition = camera.GetTranslation();

	Skybox& skybox = camera.GetSkybox();

	while (!m_OpaqueQueue.empty()) {
		Object* renderable = m_OpaqueQueue.front();
		if (renderable->GetIsEnabled()) {
			renderable->Bind();
			skybox.BindIrradianceMap(5);
			skybox.BindPrefilterMap(6);
			skybox.BindBRDFLUT(7);
			
			Shader& shader = renderable->GetMaterial().GetShader();

			shader.SetVec3("u_LightPositions[0]", glm::vec3(0.0f, 7.0f, 0.0f));
			shader.SetVec3("u_LightColors[0]", glm::vec3(300.0f, 300.0f, 300.0f));
			shader.SetVec3("u_LightPositions[1]", glm::vec3(10.0f, 3.0f, 10.0f));
			shader.SetVec3("u_LightColors[1]", glm::vec3(300.0f, 300.0f, 300.0f));
			shader.SetInt("u_NumLights", 2);

			shader.SetMat4("u_M", renderable->GetFinalTransformationMatrix());
			shader.SetMat4("u_V", view);
			shader.SetMat4("u_P", projection);

			shader.SetVec3("u_CamPos", cameraPosition);

			renderable->GetMesh().Draw();
		}
		m_OpaqueQueue.pop_front();
	}

	skybox.Draw(projection, view);

	std::sort(m_TranslucentQueue.begin(), m_TranslucentQueue.end(), [&cameraPosition](Object* obj1, Object* obj2) {
		return glm::length2(cameraPosition - obj1->GetTranslation()) < glm::length2(cameraPosition - obj2->GetTranslation());
	});
	while (!m_TranslucentQueue.empty()) {
		Object* renderable = m_TranslucentQueue.front();
		if (renderable->GetIsEnabled()) {
			renderable->Bind();
			skybox.BindIrradianceMap(5);
			skybox.BindPrefilterMap(6);
			skybox.BindBRDFLUT(7);

			Shader& shader = renderable->GetMaterial().GetShader();

			shader.SetVec3("u_LightPositions[0]", glm::vec3(0.0f, 7.0f, 0.0f));
			shader.SetVec3("u_LightColors[0]", glm::vec3(300.0f, 300.0f, 300.0f));
			shader.SetVec3("u_LightPositions[1]", glm::vec3(10.0f, 3.0f, 10.0f));
			shader.SetVec3("u_LightColors[1]", glm::vec3(300.0f, 300.0f, 300.0f));
			shader.SetInt("u_NumLights", 2);

			shader.SetMat4("u_M", renderable->GetFinalTransformationMatrix());
			shader.SetMat4("u_V", view);
			shader.SetMat4("u_P", projection);

			shader.SetVec3("u_CamPos", cameraPosition);

			renderable->GetMesh().Draw();
		}
		m_TranslucentQueue.pop_front();
	}

	//skybox->RenderBRDF();

}