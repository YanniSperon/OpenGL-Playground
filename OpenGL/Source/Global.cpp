#include "Global.h"
#include "Console.h"

#include "Mesh3DManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "MaterialManager.h"
#include "SkyboxManager.h"

#include <memory>

static std::vector<std::string> g_ConsoleLog;
static std::chrono::steady_clock::time_point g_SystemStartTime;

void Global::Initialize()
{
	g_ConsoleLog = std::vector<std::string>();
	g_SystemStartTime = std::chrono::high_resolution_clock::now();

	Mesh3DManager::InitializeSingleton();
	TextureManager::InitializeSingleton();
	ShaderManager::InitializeSingleton();
	MaterialManager::InitializeSingleton();
	SkyboxManager::InitializeSingleton();
}

std::chrono::steady_clock::time_point& Global::GetStartTime()
{
	return g_SystemStartTime;
}

std::vector<std::string>& Global::GetConsoleLog()
{
	return g_ConsoleLog;
}

void Global::Update()
{
	MaterialManager::GetInstance()->Cleanup();
	Mesh3DManager::GetInstance()->Cleanup();
	TextureManager::GetInstance()->Cleanup();
	ShaderManager::GetInstance()->Cleanup();
	SkyboxManager::GetInstance()->Cleanup();
}

void Global::Cleanup()
{
	MaterialManager::CleanupSingleton();
	Mesh3DManager::CleanupSingleton();
	TextureManager::CleanupSingleton();
	ShaderManager::CleanupSingleton();
	SkyboxManager::CleanupSingleton();
}
