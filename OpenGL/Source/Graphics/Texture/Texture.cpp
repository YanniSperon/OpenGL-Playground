#include "Texture.h"
#include "Console.h"
#include "TextureManager.h"

#include "stb_image.h"

Texture::Texture(const std::string& name)
	: m_ID(0), m_FakeUser(false), m_Path(name)
{
	if (name.find(".hdr") != std::string::npos) {
		LoadHDR(name);
	}
	else {
		LoadTexture(name);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
	Console::Warning("Deleted texture");
}

void Texture::LoadTexture(const std::string& name)
{
	glGenTextures(1, &m_ID);
	stbi_set_flip_vertically_on_load(1);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format = GL_RED;
		if (nrChannels == 1) {
			format = GL_RED;
		}
		else if (nrChannels == 3) {
			format = GL_RGB;
		}
		else if (nrChannels == 4) {
			format = GL_RGBA;
		}
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

		stbi_image_free(data);
	}
	else
	{
		stbi_image_free(data);
		Console::Error("Failed to load texture \"%s\"", name.c_str());
	}
}

void Texture::LoadHDR(const std::string& name)
{
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrComponents;
	float* data = stbi_loadf(name.c_str(), &width, &height, &nrComponents, 0);
	if (data) {
		glGenTextures(1, &m_ID);
		glBindTexture(GL_TEXTURE_2D, m_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else {
		stbi_image_free(data);
		Console::Error("Failed to load hdr texture \"%s\"", name.c_str());
	}
}

void Texture::Bind(int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

GLuint Texture::GetID()
{
	return m_ID;
}

void Texture::SetHasFakeUser(bool fakeUser)
{
	m_FakeUser = fakeUser;
	if (!fakeUser) {
		TextureManager::GetInstance()->MarkForCleanup();
	}
}

bool Texture::GetHasFakeUser()
{
	return m_FakeUser;
}

std::string Texture::GetPath()
{
	return m_Path;
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
