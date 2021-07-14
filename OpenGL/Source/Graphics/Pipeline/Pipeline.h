#pragma once

#include <memory>

#include "Shader.h"

class Pipeline {
protected:
	unsigned int m_Width;
	unsigned int m_Height;

public:
	Pipeline(unsigned int width, unsigned int height);
	virtual ~Pipeline();

	virtual void Prepare(unsigned int width, unsigned int height) = 0;
	virtual void Flush() = 0;
};