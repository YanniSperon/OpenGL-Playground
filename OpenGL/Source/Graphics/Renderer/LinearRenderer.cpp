#include "LinearRenderer.h"
#include "Console.h"

#include <algorithm>

LinearRenderer::LinearRenderer()
	: Renderer()
{

}

void LinearRenderer::Flush(int width, int height, Camera* camera)
{
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 projection = camera->GetProjectionMatrix(width, height);
	glm::mat4 view = camera->GetViewMatrix();
	glm::vec3 cameraPosition = camera->GetTranslation();

	camera->GetSkybox()->Draw(projection, view);

	while (!m_OpaqueQueue.empty()) {
		Object* renderable = m_OpaqueQueue.front();
		renderable->Bind();

		renderable->GetMaterial().GetShader().SetMat4("u_M", renderable->GetFinalTransformationMatrix());
		renderable->GetMaterial().GetShader().SetMat4("u_V", view);
		renderable->GetMaterial().GetShader().SetMat4("u_P", projection);

		renderable->GetMaterial().GetShader().SetVec3("u_CamPos", cameraPosition);

		renderable->GetMesh().Draw();
		m_OpaqueQueue.pop_front();
	}

	struct SortingKey
	{
		glm::vec3 m_CamPos;
		SortingKey(glm::vec3 camPos)
			: m_CamPos(camPos)
		{

		}
		inline bool operator() (Object* obj1, Object* obj2)
		{
			return glm::length2(m_CamPos - obj1->GetTranslation()) < glm::length2(m_CamPos - obj2->GetTranslation());
		}
	};

	std::sort(m_TranslucentQueue.begin(), m_TranslucentQueue.end(), SortingKey(cameraPosition));
	while (!m_TranslucentQueue.empty()) {
		Object* renderable = m_TranslucentQueue.front();
		renderable->Bind();

		renderable->GetMaterial().GetShader().SetMat4("u_M", renderable->GetFinalTransformationMatrix());
		renderable->GetMaterial().GetShader().SetMat4("u_V", view);
		renderable->GetMaterial().GetShader().SetMat4("u_P", projection);

		renderable->GetMaterial().GetShader().SetVec3("u_CamPos", cameraPosition);

		renderable->GetMesh().Draw();
		m_TranslucentQueue.pop_front();
	}
}