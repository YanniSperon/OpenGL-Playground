#include "Material.h"

#include "ShaderManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"

Material::Material(std::string shaderPath, int shaderType, std::string texturePath, int textureTypes)
	: m_Shader(ShaderManager::GetInstance()->GetShader(shaderPath, shaderType)), m_Textures(), m_TextureTypes(textureTypes), m_FakeUser(false), m_TexturePrefix(texturePath)
{
	if (textureTypes & MATERIAL_ALBEDO_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Albedo.png"));
	}
	if (textureTypes & MATERIAL_NORMAL_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Normal.png"));
	}
	if (textureTypes & MATERIAL_METALLIC_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Metallic.png"));
	}
	if (textureTypes & MATERIAL_SPECULAR_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Specular.png"));
	}
	if (textureTypes & MATERIAL_ROUGHNESS_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Roughness.png"));
	}
	if (textureTypes & MATERIAL_EMISSIVE_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Emissive.png"));
	}
	if (textureTypes & MATERIAL_DISPLACEMENT_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Displacement.png"));
	}
	if (textureTypes & MATERIAL_AO_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_AO.png"));
	}
}

Material::~Material()
{
	ShaderManager::GetInstance()->MarkForCleanup();
	TextureManager::GetInstance()->MarkForCleanup();
}

Shader& Material::GetShader()
{
	return *m_Shader;
}

bool Material::GetHasFakeUser()
{
	return m_FakeUser;
}

void Material::SetHasFakeUser(bool fakeUser)
{
	m_FakeUser = fakeUser;
	if (!fakeUser) {
		MaterialManager::GetInstance()->MarkForCleanup();
	}
}

int Material::GetTextureTypes()
{
	return m_TextureTypes;
}

std::string Material::GetTexturePath()
{
	return m_TexturePrefix;
}

void Material::Bind()
{
	m_Shader->Bind();
	for (int i = 0; i < m_Textures.size(); i++) {
		m_Textures[i]->Bind(i);
	}
}

void Material::Unbind()
{
	Shader::Unbind();
	Texture::Unbind();
}
