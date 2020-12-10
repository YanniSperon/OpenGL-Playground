#pragma once

#include "Material.h"

#include <string>
#include <memory>
#include <unordered_map>

class MaterialManager {
private:
	std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;

	bool m_ShouldCleanup;
public:
	MaterialManager();
	~MaterialManager();

	std::shared_ptr<Material> GetMaterial(const std::string& shaderPath, int shaderType, const std::string& texturePath, int textureTypes);

	std::unordered_map<std::string, std::shared_ptr<Material>>& GetManagedMaterial();

	void Cleanup();

	void MarkForCleanup();

	static void InitializeSingleton();
	static MaterialManager* GetInstance();
	static void CleanupSingleton();
};
