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

	newGameObject->m_Tag = json.at("Tag").get<std::string>();
	newGameObject->m_Layer = PhysicsLayer(1 << json.at("PhysicsLayer").get<int>());


	for( auto it = json.items().begin(); it != json.items().end(); ++it )
	{
		nlohmann::json value = it.value();
		if(it.key() == "IgnoredLayers")
		{
			for (auto layerIt = value.items().begin(); layerIt != value.items().end(); ++layerIt)
			{
				int layer{ layerIt.value().get<int>() };
				newGameObject->AddIgnoredPhysicsLayers(PhysicsLayer(1 << layer));
			}
			continue;
		}
		if (value.is_structured())
		{
			if(value.contains("Multiple"))
			{
				for( auto componentJson = value.items().begin(); componentJson != value.items().end(); ++componentJson)
				{
					if (componentJson.value().is_structured())
						newGameObject->AddComponent(BaseComponent::CreateFromJson(it.key(), componentJson.value()));
				}
			}
			else
				newGameObject->AddComponent(BaseComponent::CreateFromJson(it.key(), value));
		}
	}
	
	return newGameObject;
}

void GameObject::Awake()
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->Awake();
	}
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

void GameObject::OnCollisionEnter(const Collision* collision)
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->OnCollisionEnter(collision);
	}
}

void GameObject::OnCollisionExit(const Collision* collision)
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->OnCollisionExit(collision);
	}
}

void GameObject::OnTriggerEnter( const Collision* collision)
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->OnTriggerEnter(collision);
	}
}

void GameObject::OnTriggerExit(const Collision* collision)
{
	for (BaseComponent* pComponent : m_pComponents)
	{
		pComponent->OnTriggerExit(collision);
	}
}

void GameObject::PostSolve( b2Contact *contact, const b2ContactImpulse *impulse )
{
	for( BaseComponent * pComponent : m_pComponents )
	{
		pComponent->PostSolve(contact, impulse);
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

	clone->m_Tag = m_Tag;
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
