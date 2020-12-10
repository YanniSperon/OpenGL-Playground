#include "StaticMesh3D.h"
#include "Console.h"
#include "Global.h"
#include "Mesh3DManager.h"

StaticMesh3D::StaticMesh3D(const std::string& pathToModel, bool shouldLoadToVRAM)
	: Mesh3D(pathToModel, shouldLoadToVRAM)
{

}
