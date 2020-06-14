#include "pch.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent( float x, float y, float z ) : m_Position{x, y, z}
{
}

TransformComponent::TransformComponent( const glm::vec3 &position ) : m_Position{position}
{
}

TransformComponent::TransformComponent( const glm::vec3 &position, const glm::vec3 &scale ) : m_Position{ position }, m_Scale{ scale }
{
}

void TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void TransformComponent::SetScale( float x, float y, float z )
{
	m_Scale.x = x;
	m_Scale.y = y;
	m_Scale.z = z;
}

BaseComponent * TransformComponent::Clone() const
{
	return new TransformComponent{m_Position, m_Scale};
}

void TransformComponent::LoadFromJson( const nlohmann::json &json )
{
	m_Position.x = json.at("Position").at("X").get<float>();
	m_Position.y = json.at("Position").at("Y").get<float>();
	m_Position.z = json.at("Position").at("Z").get<float>();
	
	m_Position.x = json.at("Scale").at("X").get<float>();
	m_Position.y = json.at("Scale").at("Y").get<float>();
	m_Position.z = json.at("Scale").at("Z").get<float>();
}

