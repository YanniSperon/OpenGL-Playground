#include "MaterialManager.h"

static MaterialManager* s_MaterialManager;

MaterialManager::MaterialManager()
	: m_ShouldCleanup(false), m_Materials()
{
}

MaterialManager::~MaterialManager()
{
}

std::shared_ptr<Material> MaterialManager::GetMaterial(const std::string& shaderPath, int shaderType, const std::string& texturePath, int textureTypes)
{
	std::string combo = shaderPath + texturePath;
	if (m_Materials.find(combo) != m_Materials.end()) {
		return m_Materials[combo];
	}
	else {
		m_Materials[combo] = std::make_shared<Material>(shaderPath, shaderType, texturePath, textureTypes);
		return m_Materials[combo];
	}
}

std::unordered_map<std::string, std::shared_ptr<Material>>& MaterialManager::GetManagedMaterial()
{
	return m_Materials;
}

void MaterialManager::Cleanup()
{
	if (m_ShouldCleanup) {
		Material::Unbind();
		auto it = m_Materials.begin();
		while (it != m_Materials.end()) {
			if (it->second.unique() && !it->second->GetHasFakeUser()) {
				it = m_Materials.erase(it);
			}
			else {
				it++;
			}
		}
		m_ShouldCleanup = false;
	}
}

void MaterialManager::MarkForCleanup()
{
	m_ShouldCleanup = true;
}

void MaterialManager::InitializeSingleton()
{
	s_MaterialManager = new MaterialManager();
}

MaterialManager* MaterialManager::GetInstance()
{
	return s_MaterialManager;
}

void MaterialManager::CleanupSingleton()
{
	delete s_MaterialManager;
}
