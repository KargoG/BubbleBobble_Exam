#include "pch.h"
#include "Scene.h"
#include "GameObject.h"

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene()
{
	for (GameObject* pGameObject : m_Objects)
	{
		delete pGameObject;
	}
	m_Objects.clear();
}

void Scene::RemoveCollider( BoxColliderComponent *colliderToRemove )
{
	auto it = std::find(m_Collider.cbegin(), m_Collider.cend(), colliderToRemove);

	if (it == m_Collider.cend())
		return;

	m_Collider.erase(it);
}

void Scene::Add(GameObject* object)
{
	if (object->GetScene())
		throw std::exception{"A GameObject can only be part of 1 Scene!"};
	
	m_ObjectsToAdd.push_back(object);
}

void Scene::Start()
{
	for (GameObject* objectToAdd : m_ObjectsToAdd)
	{
		m_Objects.push_back(objectToAdd);
		objectToAdd->SetScene(this);
	}
	m_ObjectsToAdd.clear();
	
	for(GameObject * pGameObject : m_Objects )
	{
		pGameObject->Start();
	}

	m_IsInitialized = true;
}

void Scene::Update()
{
	if (!m_IsInitialized)
		Start();
	
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::PhysicsUpdate()
{
	for (auto& object : m_Objects)
	{
		object->PhysicsUpdate();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void Scene::Swap()
{
	for (GameObject* objectToRemove : m_ObjectsToRemove)
	{
		m_Objects.erase(std::find(m_Objects.cbegin(), m_Objects.cend(), objectToRemove));
		delete objectToRemove;
	}
	m_ObjectsToRemove.clear();
	
	for (GameObject* objectToAdd : m_ObjectsToAdd)
	{
		m_Objects.push_back(objectToAdd);
		objectToAdd->SetScene(this);
		objectToAdd->Start();
	}
	m_ObjectsToAdd.clear();

	for( GameObject * object : m_Objects )
	{
		object->Swap();
	}
}

