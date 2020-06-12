#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"

void SceneManager::Start()
{
	for (auto scene : m_Scenes)
	{
		scene->Start();
	}
}

void SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void SceneManager::PhysicsUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->PhysicsUpdate();
	}
}

void SceneManager::Render()
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

SceneManager::~SceneManager()
{
	for (Scene* pScene : m_Scenes)
	{
		delete pScene;
	}
	m_Scenes.clear();
}

Scene* SceneManager::CreateScene(const std::string& name)
{
	Scene* newScene = new Scene(name);
	m_Scenes.push_back(newScene);
	return newScene;
}

Scene* SceneManager::GetScene( const std::string &name )
{
	for( Scene* scene : m_Scenes )
	{
		if (scene->GetName() == name)
			return scene;
	}
	
	return nullptr;
}
