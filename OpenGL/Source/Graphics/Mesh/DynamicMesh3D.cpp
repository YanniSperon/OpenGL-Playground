#include "DynamicMesh3D.h"
#include "Console.h"
#include "Global.h"
#include "Mesh3DManager.h"

#include <glew.h>
//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

DynamicMesh3D::DynamicMesh3D(const std::string& pathToModel, bool shouldLoadToVRAM)
	: Mesh3D(pathToModel, shouldLoadToVRAM), m_IsAnimating(false)
{

}

DynamicMesh3D::DynamicMesh3D(StaticMesh3D& mesh)
	: Mesh3D(mesh)
{

}
