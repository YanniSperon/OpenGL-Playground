#include "Renderer.h"

Renderer* s_Renderer;

Renderer::Renderer()
	: m_OpaqueQueue(), m_TranslucentQueue()
{

}

Renderer::~Renderer()
{

}

void Renderer::AddObject(Object* object)
{
	if (object->GetIsTranslucent()) {
		m_TranslucentQueue.push_back(object);
	}
	else {
		m_OpaqueQueue.push_back(object);
	}
}

void Renderer::InitializeSingleton(Renderer* renderer)
{
	s_Renderer = renderer;
}

Renderer* Renderer::GetInstance()
{
	return s_Renderer;
}

void Renderer::CleanupSingleton()
{
	delete s_Renderer;
}
