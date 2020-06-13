#include "pch.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "TransformComponent.h"
#include "Scene.h"
#include <typeinfo>

GameObject::GameObject()
{
	AddComponent(new TransformComponent{0, 0, 0});
}


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

GameObject * GameObject::CreateFromJson( const nlohmann::json &json )
{
	UNREFERENCED_PARAMETER(json);
	GameObject *newGameObject{ new GameObject{} };
	
	for( auto it = json.items().begin(); it != json.items().end(); ++it )
	{
		newGameObject->AddComponent(BaseComponent::CreateFromJson(it.key(), it.value()));
	}
	
	return newGameObject;
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

GameObject* GameObject::Clone() const
{
	GameObject* clone{ new GameObject{} };

	for( BaseComponent * const pComponent : m_pComponents )
	{
		clone->AddComponent( pComponent->Clone() );
	}

	if (m_pScene)
		m_pScene->Add(clone);
	
	return clone;
}