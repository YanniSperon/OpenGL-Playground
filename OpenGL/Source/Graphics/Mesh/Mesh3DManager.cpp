#include "Mesh3DManager.h"
#include "Console.h"

static Mesh3DManager* s_Mesh3DManager;

Mesh3DManager::Mesh3DManager()
{
	m_StaticMeshes = std::unordered_map<std::string, std::shared_ptr<StaticMesh3D>>();
	m_DynamicMeshes = std::vector<std::shared_ptr<DynamicMesh3D>>();
}

Mesh3DManager::~Mesh3DManager()
{
	m_StaticMeshes.clear();
	m_DynamicMeshes.clear();
}

std::shared_ptr<Mesh3D> Mesh3DManager::GetMesh(const std::string& name, bool isDynamic)
{
	if (isDynamic) {
		return GetDynamicMesh(name);
	}
	else {
		return GetStaticMesh(name);
	}
}

std::shared_ptr<StaticMesh3D> Mesh3DManager::GetStaticMesh(const std::string& name)
{
	if (m_StaticMeshes.find(name) != m_StaticMeshes.end()) {
		return m_StaticMeshes[name];
	}
	else {
		m_StaticMeshes[name] = std::make_shared<StaticMesh3D>(name, true);
		return m_StaticMeshes[name];
	}
}

std::shared_ptr<DynamicMesh3D> Mesh3DManager::GetDynamicMesh(const std::string& name)
{
	if (m_StaticMeshes.find(name) != m_StaticMeshes.end()) {
		m_DynamicMeshes.push_back(std::make_shared<DynamicMesh3D>(*m_StaticMeshes[name]));
		return m_DynamicMeshes[m_DynamicMeshes.size() - 1];
	}
	else {
		m_StaticMeshes[name] = std::make_shared<StaticMesh3D>(name, true);
		m_DynamicMeshes.push_back(std::make_shared<DynamicMesh3D>(*m_StaticMeshes[name]));
		return m_DynamicMeshes[m_DynamicMeshes.size() - 1];
	}
}

std::shared_ptr<DynamicMesh3D> Mesh3DManager::GetDynamicMeshAt(int index)
{
	return m_DynamicMeshes[index];
}

std::unordered_map<std::string, std::shared_ptr<StaticMesh3D>>& Mesh3DManager::GetManagedStaticMeshes()
{
	return m_StaticMeshes;
}

std::vector<std::shared_ptr<DynamicMesh3D>>& Mesh3DManager::GetManagedDynamicMeshes()
{
	return m_DynamicMeshes;
}

void Mesh3DManager::Cleanup()
{
	if (m_ShouldCleanup) {
		Mesh3D::Unbind();
		{
			auto it = m_StaticMeshes.begin();
			while (it != m_StaticMeshes.end()) {
				if (it->second.unique() && !it->second->GetHasFakeUser()) {
					it = m_StaticMeshes.erase(it);
				}
				else {
					it++;
				}
			}
		}
		{
			auto it = m_DynamicMeshes.begin();
			while (it != m_DynamicMeshes.end()) {
				if (it->unique() && !it->get()->GetHasFakeUser()) {
					it = m_DynamicMeshes.erase(it);
				}
				else {
					it++;
				}
			}
		}
		m_ShouldCleanup = false;
	}
}

void Mesh3DManager::MarkForCleanup()
{
	m_ShouldCleanup = true;
}

void Mesh3DManager::InitializeSingleton()
{
	s_Mesh3DManager = new Mesh3DManager();
}

Mesh3DManager* Mesh3DManager::GetInstance()
{
	return s_Mesh3DManager;
}

void Mesh3DManager::CleanupSingleton()
{
	delete s_Mesh3DManager;
}
