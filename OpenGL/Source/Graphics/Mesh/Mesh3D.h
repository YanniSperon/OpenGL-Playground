#pragma once

#include <glm/glm.hpp>
#include <glew.h>

#include <string>

struct Vertex3D {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Mesh3D {
private:

protected:
	bool m_FakeUser;

	Vertex3D* m_Vertices;
	GLuint m_NumVertices;
	GLuint* m_Indices;
	GLuint m_NumIndices;
	GLuint m_VRAMHandleVAO;
	GLuint m_VRAMHandleVBO;
	GLuint m_VRAMHandleIBO;

	std::string m_Path;
public:
	Mesh3D(const std::string& pathToModel, bool shouldLoadToVRAM);
	Mesh3D(Mesh3D& mesh);
	~Mesh3D();

	void SetHasFakeUser(bool fakeUser);
	bool GetHasFakeUser();

	static void Unbind();

	void Bind();
	void Draw();

	GLsizeiptr VertexBufferSize();
	GLsizeiptr IndexBufferSize();

	void LoadToRAM(const std::string& path);
	void UnloadFromRAM();
	void LoadToVRAM();
	void UnloadFromVRAM();

	void SetVertices(Vertex3D* vertices);
	Vertex3D* GetVertices();
	void SetNumVertices(GLuint num);
	GLuint GetNumVertices();
	void SetIndices(GLuint* indices);
	GLuint* GetIndices();
	void SetNumIndices(GLuint num);
	GLuint GetNumIndices();

	std::string GetPath();

	bool IsLoadedToVRAM();
};
