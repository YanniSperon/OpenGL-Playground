#include "Material.h"
#include "Console.h"

#include "ShaderManager.h"
#include "TextureManager.h"
#include "MaterialManager.h"

Material::Material(const std::string& shaderPath, int shaderType, const std::string& texturePath, int textureTypes)
	: m_Shader(ShaderManager::GetInstance()->GetShader(shaderPath, shaderType)), m_Textures(), m_TextureTypes(textureTypes), m_FakeUser(false), m_TexturePrefix(texturePath)
{
	if (textureTypes & MATERIAL_ALBEDO_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Albedo" + TEXTURE_FILE_EXTENSION));
	}
	if (textureTypes & MATERIAL_NORMAL_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Normal" + TEXTURE_FILE_EXTENSION));
	}
	if (textureTypes & MATERIAL_METALLIC_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Metallic" + TEXTURE_FILE_EXTENSION));
	}
	if (textureTypes & MATERIAL_SPECULAR_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Specular" + TEXTURE_FILE_EXTENSION));
	}
	if (textureTypes & MATERIAL_ROUGHNESS_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Roughness" + TEXTURE_FILE_EXTENSION));
	}
	if (textureTypes & MATERIAL_EMISSIVE_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Emissive" + TEXTURE_FILE_EXTENSION));
	}
	if (textureTypes & MATERIAL_DISPLACEMENT_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Displacement" + TEXTURE_FILE_EXTENSION));
	}
	if (textureTypes & MATERIAL_AO_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_AO" + TEXTURE_FILE_EXTENSION));
	}
	if (textureTypes & MATERIAL_HDR_TEXTURE) {
		m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_HDR" + TEXTURE_HDR_EXTENSION));
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

void Material::AddTexture(const std::string& name, int textureType)
{
	m_TextureTypes |= textureType;
	m_Textures.push_back(TextureManager::GetInstance()->GetTexture(name));
}

void Material::AddTextures(const std::string& texturePath, int textureTypes)
{
	if (!(textureTypes & m_TextureTypes)) {
		if (textureTypes & MATERIAL_ALBEDO_TEXTURE) {
			m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Albedo" + TEXTURE_FILE_EXTENSION));
		}
		if (textureTypes & MATERIAL_NORMAL_TEXTURE) {
			m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Normal" + TEXTURE_FILE_EXTENSION));
		}
		if (textureTypes & MATERIAL_METALLIC_TEXTURE) {
			m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Metallic" + TEXTURE_FILE_EXTENSION));
		}
		if (textureTypes & MATERIAL_SPECULAR_TEXTURE) {
			m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Specular" + TEXTURE_FILE_EXTENSION));
		}
		if (textureTypes & MATERIAL_ROUGHNESS_TEXTURE) {
			m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Roughness" + TEXTURE_FILE_EXTENSION));
		}
		if (textureTypes & MATERIAL_EMISSIVE_TEXTURE) {
			m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Emissive" + TEXTURE_FILE_EXTENSION));
		}
		if (textureTypes & MATERIAL_DISPLACEMENT_TEXTURE) {
			m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_Displacement" + TEXTURE_FILE_EXTENSION));
		}
		if (textureTypes & MATERIAL_AO_TEXTURE) {
			m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_AO" + TEXTURE_FILE_EXTENSION));
		}
		if (textureTypes & MATERIAL_HDR_TEXTURE) {
			m_Textures.push_back(TextureManager::GetInstance()->GetTexture(texturePath + "_HDR" + TEXTURE_HDR_EXTENSION));
		}
	}
	else {
		Console::Error("Material already has texture of type(s) %i", textureTypes & m_TextureTypes);
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
