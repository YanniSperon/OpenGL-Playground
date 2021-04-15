#include "Skybox.h"
#include "Console.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Mesh3DManager.h"

#include "stb_image.h"
#include "glm/gtc/matrix_transform.hpp"

#define ENVIRONMENT_MAP_RESOLUTION 512 // 512
#define IRRADIANCE_MAP_RESOLUTION 32 // 32
#define PREFILTER_MAP_RESOLUTION 128 // 128
#define BRDF_LUT_RESOLUTION 512 // 512

Skybox::Skybox(const std::string& path, float gamma)
    : m_FBO(0), m_RBO(0), m_EnvironmentMap(0), m_CubeMapShader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/Skybox", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_RenderShader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/SkyboxDisplay", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_VAO(0), m_VBO(0), m_HDRTexture(TextureManager::GetInstance()->GetTexture(path)), m_CubeMesh(Mesh3DManager::GetInstance()->GetStaticMesh("Resources/Skybox.obj")), m_IrradianceShader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/Irradiance", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_Gamma(gamma), m_PrefilterShader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/Prefilter", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_BRDFShader(ShaderManager::GetInstance()->GetShader("Resources/Shaders/BRDF", SHADER_VERTEX_SHADER | SHADER_FRAGMENT_SHADER)), m_PrefilterMap(0), m_BRDFLookUpTexture(0)
{
    PrepareFramebuffer();

    PrepareEnvironmentMap();

    PrepareIrradianceMap();

    PreparePrefilterMap();

    PrepareBRDFLUT();
}

Skybox::~Skybox()
{
    glDeleteBuffers(1, &m_VBO);
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteTextures(1, &m_EnvironmentMap);
    glDeleteTextures(1, &m_IrradianceMap);
    glDeleteTextures(1, &m_PrefilterMap);
    glDeleteTextures(1, &m_BRDFLookUpTexture);
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
    m_RenderShader->SetFloat("u_Gamma", m_Gamma);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMap);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap);
    
    m_CubeMesh->Bind();
    m_CubeMesh->Draw();
}

void Skybox::BindIrradianceMap(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
}

void Skybox::BindPrefilterMap(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap);
}

void Skybox::BindBRDFLUT(int slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_BRDFLookUpTexture);
}

void Skybox::RenderBRDF()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_BRDFShader->Bind();

    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // render quad
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

float Skybox::GetGamma()
{
    return m_Gamma;
}

void Skybox::SetGamma(float gamma)
{
    m_Gamma = gamma;
}

void Skybox::PrepareFramebuffer()
{
    glGenFramebuffers(1, &m_FBO);
    glGenRenderbuffers(1, &m_RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, ENVIRONMENT_MAP_RESOLUTION, ENVIRONMENT_MAP_RESOLUTION);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
}

void Skybox::PrepareEnvironmentMap()
{
    // Set cubemap to render to and attach to framebuffer
    glGenTextures(1, &m_EnvironmentMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, ENVIRONMENT_MAP_RESOLUTION, ENVIRONMENT_MAP_RESOLUTION, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    // Set up projection and view matrices for the 6 cubemap faces
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



    // convert the equirectangular map to a cubemap
    m_CubeMapShader->Bind();
    m_CubeMapShader->SetMat4("u_P", captureProjection);
    m_HDRTexture->Bind(0);

    glViewport(0, 0, ENVIRONMENT_MAP_RESOLUTION, ENVIRONMENT_MAP_RESOLUTION);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    for (unsigned int i = 0; i < 6; i++) {
        m_CubeMapShader->SetMat4("u_V", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_EnvironmentMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_CubeMesh->Bind();
        m_CubeMesh->Draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMap);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void Skybox::PrepareIrradianceMap()
{
    // prepare cubemap
    glGenTextures(1, &m_IrradianceMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_IrradianceMap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, IRRADIANCE_MAP_RESOLUTION, IRRADIANCE_MAP_RESOLUTION, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



    // rescale framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, IRRADIANCE_MAP_RESOLUTION, IRRADIANCE_MAP_RESOLUTION);



    // Set up projection and view matrices for the 6 cubemap faces
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



    // create the cubemap
    m_IrradianceShader->Bind();
    m_IrradianceShader->SetMat4("u_P", captureProjection);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMap);

    glViewport(0, 0, IRRADIANCE_MAP_RESOLUTION, IRRADIANCE_MAP_RESOLUTION);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    for (unsigned int i = 0; i < 6; ++i) {
        m_IrradianceShader->SetMat4("u_V", captureViews[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_IrradianceMap, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_CubeMesh->Bind();
        m_CubeMesh->Draw();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Skybox::PreparePrefilterMap()
{
    // prepare cubemap
    glGenTextures(1, &m_PrefilterMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_PrefilterMap);
    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, PREFILTER_MAP_RESOLUTION, PREFILTER_MAP_RESOLUTION, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);



    // Set up projection and view matrices for the 6 cubemap faces
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



    // create prefilter map
    m_PrefilterShader->Bind();
    m_PrefilterShader->SetMat4("u_P", captureProjection);
    m_PrefilterShader->SetFloat("u_CubeMapResolution", ENVIRONMENT_MAP_RESOLUTION);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_EnvironmentMap);

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    unsigned int maxMipLevels = 5;
    for (unsigned int mip = 0; mip < maxMipLevels; ++mip) {
        //resize framebuffer according to mip-level size
        unsigned int mipSize = PREFILTER_MAP_RESOLUTION * std::pow(0.5, mip);
        glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipSize, mipSize);
        glViewport(0, 0, mipSize, mipSize);

        float roughness = (float)mip / (float)(maxMipLevels - 1);
        m_PrefilterShader->SetFloat("u_Roughness", roughness);
        for (unsigned int i = 0; i < 6; ++i) {
            m_PrefilterShader->SetMat4("u_V", captureViews[i]);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_PrefilterMap, mip);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_CubeMesh->Bind();
            m_CubeMesh->Draw();
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Skybox::PrepareBRDFLUT()
{
    glGenTextures(1, &m_BRDFLookUpTexture);

    glBindTexture(GL_TEXTURE_2D, m_BRDFLookUpTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, BRDF_LUT_RESOLUTION, BRDF_LUT_RESOLUTION, 0, GL_RG, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // pre-allocate enough memory for the LUT texture.
    


    // reconfigure fbo
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, BRDF_LUT_RESOLUTION, BRDF_LUT_RESOLUTION);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BRDFLookUpTexture, 0);



    // render quad
    glViewport(0, 0, BRDF_LUT_RESOLUTION, BRDF_LUT_RESOLUTION);
    m_BRDFShader->Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // create quad
    unsigned int quadVAO = 0;
    unsigned int quadVBO = 0;
    float quadVertices[] = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // render quad
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);



    // unbind framebuffer to finish
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
