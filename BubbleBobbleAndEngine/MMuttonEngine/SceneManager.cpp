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
	m_pActiveScene->Update();
}

void SceneManager::PhysicsUpdate()
{
	m_pActiveScene->PhysicsUpdate();
}

void SceneManager::Render() const
{
	m_pActiveScene->Render();
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

	if (!m_pActiveScene)
		m_pActiveScene = newScene;
	
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

void SceneManager::Swap()
{
	m_pActiveScene->Swap();
}
