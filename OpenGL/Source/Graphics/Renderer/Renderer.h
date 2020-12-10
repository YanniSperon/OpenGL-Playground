#pragma once

#include "Object.h"
#include "Light.h"
#include "Camera.h"

#include "glm/glm.hpp"

#include <deque>

class Renderer {
protected:
	std::deque<Object*> m_OpaqueQueue;
	std::deque<Object*> m_TranslucentQueue;

public:
	Renderer();
	~Renderer();

	void AddObject(Object* object);

	virtual void Flush(int width, int height, Camera* camera) = 0;

	static void InitializeSingleton(Renderer* renderer);
	static Renderer* GetInstance();
	static void CleanupSingleton();
};
