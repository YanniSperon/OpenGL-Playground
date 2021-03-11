#include "Skybox.h"
#include "Console.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Mesh3DManager.h"

#include "stb_image.h"
#include "glm/gtc/matrix_transform.hpp"

#define SKYBOX_RESOLUTION 512

Skybox::Skybox(const std::string& path)
    : m_FBO(0), m_RBO(0), m_CubeMap(0), m_CubemapShader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/Skybox", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_RenderShader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/SkyboxDisplay", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_VAO(0), m_VBO(0), m_HDRTexture(TextureManager::GetInstance()->GetTexture(path)), m_CubeMesh(Mesh3DManager::GetInstance()->GetStaticMesh("Resources/Skybox.obj"))
{
    glGenFramebuffers(1, &m_FBO);
    glGenRenderbuffers(1, &m_RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, SKYBOX_RESOLUTION, SKYBOX_RESOLUTION);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
    
    glGenTextures(1, &m_CubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        // note that we store each face with 16 bit floating point values
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, SKYBOX_RESOLUTION, SKYBOX_RESOLUTION, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
    glm::mat4 captureViews[] =
    {
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
       glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
    };

    m_CubemapShader->Bind();
    m_CubemapShader->SetMat4("u_P", captureProjection);
    m_HDRTexture->Bind(0);

    glViewport(0, 0, SKYBOX_RESOLUTION, SKYBOX_RESOLUTION);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    for (unsigned int i = 0; i < 6; i++) {
        m_CubemapShader->SetMat4("u_V", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_CubeMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_CubeMesh->Bind();
        m_CubeMesh->Draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Skybox::~Skybox()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteTextures(1, &m_CubeMap);
    glDeleteFramebuffers(1, &m_FBO);
    glDeleteRenderbuffers(1, &m_RBO);
    ShaderManager::GetInstance()->MarkForCleanup();
    Mesh3DManager::GetInstance()->MarkForCleanup();
    TextureManager::GetInstance()->MarkForCleanup();
}

void Skybox::Draw(const glm::mat4& projection, const glm::mat4& view)
{
    m_RenderShader->Bind();
    m_RenderShader->SetMat4("u_P", projection);
    m_RenderShader->SetMat4("u_V", view);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeMap);
    
    m_CubeMesh->Bind();
    m_CubeMesh->Draw();
}
