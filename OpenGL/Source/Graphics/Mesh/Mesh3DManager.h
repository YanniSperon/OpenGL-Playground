#pragma once

#include "StaticMesh3D.h"
#include "DynamicMesh3D.h"

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

class Mesh3DManager {
private:
	std::unordered_map<std::string, std::shared_ptr<StaticMesh3D>> m_StaticMeshes;
	std::vector<std::shared_ptr<DynamicMesh3D>> m_DynamicMeshes;

	bool m_ShouldCleanup;
public:
	Mesh3DManager();
	~Mesh3DManager();

	std::shared_ptr<Mesh3D> GetMesh(const std::string& name, bool isDynamic);
	std::shared_ptr<StaticMesh3D> GetStaticMesh(const std::string& name);
	std::shared_ptr<DynamicMesh3D> GetDynamicMesh(const std::string& name);
	std::shared_ptr<DynamicMesh3D> GetDynamicMeshAt(int index);

	std::unordered_map<std::string, std::shared_ptr<StaticMesh3D>>& GetManagedStaticMeshes();
	std::vector<std::shared_ptr<DynamicMesh3D>>& GetManagedDynamicMeshes();

	void Cleanup();

	void MarkForCleanup();

	static void InitializeSingleton();
	static Mesh3DManager* GetInstance();
	static void CleanupSingleton();
};
