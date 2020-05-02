#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"

GameObject::~GameObject() = default;

void GameObject::AddComponent(const std::shared_ptr<BaseComponent> newComponent)
{
	m_pComponents.push_back(newComponent);
	std::shared_ptr<GameObject> thisObject{ shared_from_this() };
	newComponent->SetGameObject(thisObject);
}

void GameObject::Start()
{
	for (std::shared_ptr<BaseComponent>& pComponent : m_pComponents)
	{
		pComponent->Start();
	}
}

void GameObject::Update(){
	for (std::shared_ptr<BaseComponent>& pComponent : m_pComponents)
	{
		pComponent->Update();
	}
}

void GameObject::PhysicsUpdate(){
	for (std::shared_ptr<BaseComponent>& pComponent : m_pComponents)
	{
		pComponent->PhysicsUpdate();
	}
}

void GameObject::Render() const
{
	for( const std::shared_ptr<BaseComponent> pComponent : m_pComponents )
	{
		pComponent->Render();
	}
}