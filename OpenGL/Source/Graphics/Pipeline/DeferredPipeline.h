#pragma once

#include "Pipeline.h"

class DeferredPipeline : public Pipeline {
private:

public:
	DeferredPipeline(const std::string& psfxShaderPath, int psfxShaderType, unsigned int width, unsigned int height);
	virtual ~DeferredPipeline() override;

	virtual void Prepare(unsigned int width, unsigned int height) override;
	virtual void Flush() override;
};