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
	m_ColliderToRemove.push_back(colliderToRemove);
}

void Scene::Add(GameObject* object)
{
	if (object->GetScene())
		throw std::exception{"A GameObject can only be part of 1 Scene!"};
	
	m_ObjectsToAdd.push_back(object);
}

void Scene::Remove( GameObject *toRemove )
{
	if (toRemove->GetScene() != this)
		throw std::exception{ "You cant remove an object from a Scene its not part of!" };

	m_ObjectsToRemove.push_back(toRemove);
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
	
	for (BoxColliderComponent* colliderToRemove : m_ColliderToRemove)
	{
		m_Collider.erase(std::find(m_Collider.cbegin(), m_Collider.cend(), colliderToRemove));
	}
	m_ColliderToRemove.clear();
	
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

