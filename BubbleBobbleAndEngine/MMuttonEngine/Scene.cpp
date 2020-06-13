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
	
	m_Objects.push_back(object);
	object->SetScene(this);
}

void Scene::Start()
{
	for(GameObject * pGameObject : m_Objects )
	{
		pGameObject->Start();
	}
}

void Scene::Update()
{
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

