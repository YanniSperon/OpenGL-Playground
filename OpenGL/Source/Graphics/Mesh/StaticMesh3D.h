#pragma once

#include "glm.hpp"
#include "Mesh3D.h"

#include <glew.h>

#include <string>

class StaticMesh3D : public Mesh3D {
private:

public:
	StaticMesh3D(const std::string& pathToModel, bool shouldLoadToVRAM);
};
