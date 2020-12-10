#pragma once

#include "StaticMesh3D.h"

class DynamicMesh3D : public Mesh3D {
private:
	bool m_IsAnimating;
public:
	DynamicMesh3D(const std::string& pathToModel, bool shouldLoadToVRAM);
	DynamicMesh3D(StaticMesh3D& mesh);
};
