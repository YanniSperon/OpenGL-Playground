#pragma once

#include "Texture.h"
#include "Shader.h"

#include <memory>
#include <string>
#include <vector>

#define MATERIAL_ALBEDO_TEXTURE 1
#define MATERIAL_NORMAL_TEXTURE 2
#define MATERIAL_METALLIC_TEXTURE 4
#define MATERIAL_SPECULAR_TEXTURE 8
#define MATERIAL_ROUGHNESS_TEXTURE 16
#define MATERIAL_EMISSIVE_TEXTURE 32
#define MATERIAL_DISPLACEMENT_TEXTURE 64
#define MATERIAL_AO_TEXTURE 128

class Material {
private:
	std::shared_ptr<Shader> m_Shader;
	std::vector<std::shared_ptr<Texture>> m_Textures;

	std::string m_TexturePrefix;

	int m_TextureTypes;

	bool m_FakeUser;
public:
	Material(std::string shaderPath, int shaderType, std::string texturePath, int textureTypes);
	~Material();

	Shader& GetShader();

	bool GetHasFakeUser();
	void SetHasFakeUser(bool fakeUser);

	int GetTextureTypes();
	std::string GetTexturePath();

	void Bind();

	static void Unbind();
};