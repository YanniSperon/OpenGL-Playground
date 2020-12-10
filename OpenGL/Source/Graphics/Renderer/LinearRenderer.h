#pragma once

#include "Renderer.h"

class LinearRenderer : public Renderer {
private:

public:
	LinearRenderer();

	void Flush(int width, int height, Camera* camera) override;
};
