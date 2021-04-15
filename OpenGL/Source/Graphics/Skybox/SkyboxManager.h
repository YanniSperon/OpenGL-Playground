#pragma once

#include "Skybox.h"

#include <string>
#include <memory>
#include <unordered_map>

class SkyboxManager {
private:
	std::unordered_map<std::string, std::shared_ptr<Skybox>> m_Skyboxes;

	bool m_ShouldCleanup;
public:
	SkyboxManager();
	~SkyboxManager();

	std::shared_ptr<Skybox> GetSkybox(const std::string& path);

	std::unordered_map<std::string, std::shared_ptr<Skybox>>& GetManagedSkyboxes();

	void Cleanup();

	void MarkForCleanup();

	static void InitializeSingleton();
	static SkyboxManager* GetInstance();
	static void CleanupSingleton();
};
