#include "SkyboxManager.h"
#include "Console.h"

static SkyboxManager* s_SkyboxManager;

SkyboxManager::SkyboxManager()
{
	m_Skyboxes = std::unordered_map<std::string, std::shared_ptr<Skybox>>();
}

SkyboxManager::~SkyboxManager()
{
	m_Skyboxes.clear();
}

std::shared_ptr<Skybox> SkyboxManager::GetSkybox(const std::string& path)
{
	if (m_Skyboxes.find(path) != m_Skyboxes.end()) {
		return m_Skyboxes[path];
	}
	else {
		m_Skyboxes[path] = std::make_shared<Skybox>(path);
		return m_Skyboxes[path];
	}
}

std::unordered_map<std::string, std::shared_ptr<Skybox>>& SkyboxManager::GetManagedSkyboxes()
{
	return m_Skyboxes;
}

void SkyboxManager::Cleanup()
{
	if (m_ShouldCleanup) {
		Skybox::Unbind();
		auto it = m_Skyboxes.begin();
		while (it != m_Skyboxes.end()) {
			if (it->second.unique() && !it->second->GetHasFakeUser()) {
				it = m_Skyboxes.erase(it);
			}
			else {
				it++;
			}
		}
		m_ShouldCleanup = false;
	}
}

void SkyboxManager::MarkForCleanup()
{
	m_ShouldCleanup = true;
}

void SkyboxManager::InitializeSingleton()
{
	s_SkyboxManager = new SkyboxManager();
}

SkyboxManager* SkyboxManager::GetInstance()
{
	return s_SkyboxManager;
}

void SkyboxManager::CleanupSingleton()
{
	delete s_SkyboxManager;
}
