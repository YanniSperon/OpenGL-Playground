#pragma once

#include <glew.h>
#include <string>

#define TEXTURE_FILE_EXTENSION ".png"
#define TEXTURE_HDR_EXTENSION ".hdr"

class Texture {
private:
	GLuint m_ID;

	bool m_FakeUser;

	std::string m_Path;

public:
	Texture(const std::string& name);
	~Texture();

	void LoadTexture(const std::string& name);
	void LoadHDR(const std::string& name);

	void Bind(int slot);

	GLuint GetID();

	void SetHasFakeUser(bool fakeUser);
	bool GetHasFakeUser();

	std::string GetPath();

	static void Unbind();
};
