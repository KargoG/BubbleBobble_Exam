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

	newGameObject->m_Layer = PhysicsLayer(1 << json.at("PhysicsLayer").get<int>());
	
	for( auto it = json.items().begin(); it != json.items().end(); ++it )
	{
		if(it.key() == "IgnoredLayers")
		{
			nlohmann::json ignoredLayers = it.value();
			for (auto layerIt = ignoredLayers.items().begin(); layerIt != ignoredLayers.items().end(); ++layerIt)
			{
				int layer{ layerIt.value().get<int>() };
				newGameObject->AddIgnoredPhysicsLayers(PhysicsLayer(1 << layer));
			}
			continue;
		}
		if(it.value().is_structured())
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

void GameObject::OnCollision(const BoxColliderComponent *otherCollider )
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->OnCollision(otherCollider);
	}
}

void GameObject::Swap()
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Swap();
	}
}

GameObject* GameObject::Clone() const
{
	GameObject* clone{ new GameObject{} };

	clone->m_Layer = m_Layer;
	clone->m_IgnoredLayers = m_IgnoredLayers;
	
	bool skippedTransform{false};
	for( BaseComponent * const pComponent : m_pComponents )
	{
		if (!skippedTransform)
		{
			skippedTransform = true;
			continue;
		}
		clone->AddComponent( pComponent->Clone() );
	}

	if (m_pScene)
		m_pScene->Add(clone);
	
	return clone;
}
