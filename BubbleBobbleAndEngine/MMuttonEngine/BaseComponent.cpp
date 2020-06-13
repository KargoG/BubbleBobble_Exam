#include "pch.h"
#include "BaseComponent.h"
#include "BoxColliderComponent.h"
#include "RigidbodyComponent.h"
#include "TextComponent.h"
#include "TextureRendererComponent.h"
#include "TransformComponent.h"
#include "InputComponent.h"

std::unordered_map<std::string, BaseComponent*> BaseComponent::m_ComponentPrototypes{};

BaseComponent::BaseComponent( )
{
}

BaseComponent * BaseComponent::CreateFromJson( const std::string &componentName, const nlohmann::json &json )
{
	auto it = m_ComponentPrototypes.find(componentName);

	if (it == m_ComponentPrototypes.end())
		throw std::exception(std::string{ "Component " + componentName + " wasn't registered!\n" }.c_str());
	
	BaseComponent* component{ it->second->Clone() };
	component->LoadFromJson(json);

	return component;
}

void BaseComponent::RegisterEngineComponents()
{
	RegisterComponent("BoxColliderComponent", new BoxColliderComponent{});
	RegisterComponent("RigidbodyComponent", new RigidbodyComponent{});
	RegisterComponent("TextComponent", new TextComponent{"", 0, "test"});
	RegisterComponent("TextureRendererComponent", new TextureRendererComponent{});
	RegisterComponent("TransformComponent", new TransformComponent{0, 0, 0});
	RegisterComponent("InputComponent", new InputComponent{});
}

void BaseComponent::RegisterComponent( const std::string &componentName, BaseComponent *prototype )
{
	if (m_ComponentPrototypes.find(componentName) == m_ComponentPrototypes.end())
	{
		m_ComponentPrototypes.insert(make_pair(componentName, prototype));
	}
}

void BaseComponent::CleanUp()
{
	for( std::pair<std::string, BaseComponent *> componentPrototype : m_ComponentPrototypes )
	{
		delete componentPrototype.second;
	}
	m_ComponentPrototypes.clear();
}

void BaseComponent::SetGameObject( GameObject* pGameObject )
{
	m_pGameObject = pGameObject;
}
