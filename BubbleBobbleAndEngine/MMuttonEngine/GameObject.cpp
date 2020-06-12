#include "pch.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "TransformComponent.h"

GameObject::~GameObject()
{
	for( BaseComponent * pComponent : m_pComponents )
	{
		delete pComponent;
	}
	m_pComponents.clear();
};

void GameObject::AddComponent(BaseComponent* pComponent)
{
	m_pComponents.push_back(pComponent);
	pComponent->SetGameObject(this);
}

void GameObject::Start()
{	
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Start();
	}
}

void GameObject::Update(){
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Update();
	}
}

void GameObject::PhysicsUpdate(){
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->PhysicsUpdate();
	}
}

void GameObject::Render() const
{
	for( BaseComponent* pComponent : m_pComponents )
	{
		pComponent->Render();
	}
}

GameObject::GameObject()
{
	AddComponent(new TransformComponent{});
}
