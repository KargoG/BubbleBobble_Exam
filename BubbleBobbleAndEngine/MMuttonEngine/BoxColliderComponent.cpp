#include "pch.h"
#include "BoxColliderComponent.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "RigidbodyComponent.h"

BoxColliderComponent::BoxColliderComponent(float width, float height) : m_Dimensions { width, height }
{
}

void BoxColliderComponent::Start()
{
	RigidbodyComponent *rb = m_pGameObject->GetComponent<RigidbodyComponent>();
	
	if (rb)
		rb->AddCollider(this);
	else
		m_pGameObject->GetScene()->AddCollider(this);
}

void BoxColliderComponent::Render() const
{
#if _DEBUG
	glm::vec2 pos = m_pGameObject->GetComponent<TransformComponent>()->GetPosition();
	
	Renderer::GetInstance().SetRenderColor(0, 255, 0);
	Renderer::GetInstance().RenderRect(pos, m_Dimensions);
#endif
}

TouchFlags BoxColliderComponent::CalculateCollisions(const BoxColliderComponent *pOtherCollider)
{
	TouchFlags flags{ TouchFlags::None };
	glm::vec2 position{ GetGameObject()->GetComponent<TransformComponent>()->GetPosition() };
	glm::vec2 dimensions{ GetDimensions() };
	
	glm::vec2 colliderDimensions{ pOtherCollider->GetDimensions() };
	const glm::vec2& colliderPosition{ pOtherCollider->GetGameObject()->GetComponent<TransformComponent>()->GetPosition() };
	
	bool collisionXAxis{ position.x < colliderPosition.x + colliderDimensions.x &&
								   position.x + dimensions.x > colliderPosition.x };
	bool collisionYAxis{ position.y < colliderPosition.y + colliderDimensions.y &&
						position.y + dimensions.y > colliderPosition.y };


	if (!collisionXAxis || !collisionYAxis)
		return flags;
	
		
	if (position.x < colliderPosition.x + colliderDimensions.x && position.x > colliderPosition.x) // Left
	{
		if (colliderPosition.y + colliderDimensions.y - position.y > m_TouchEpsilon /*||
			playerPosition.y + playerDimensions.y - colliderPosition.y > m_TouchEpsilon*/) // check if left is an actual wall and the touch isn't a fluke
		{
			flags = TouchFlags(int(flags) | int(TouchFlags::Left));
			//m_Velocity.x = max(0, m_Velocity.x);
		}
	}
	if (position.x + dimensions.x > colliderPosition.x && position.x + dimensions.x < colliderPosition.x + colliderDimensions.x) // Right
	{
		if (colliderPosition.y + colliderDimensions.y - position.y > m_TouchEpsilon/* ||
			playerPosition.y + playerDimensions.y - colliderPosition.y > m_TouchEpsilon*/) // check if right is an actual wall and the touch isn't a fluke
		{
			flags = TouchFlags(int(flags) | int(TouchFlags::Right));
			//m_Velocity.x = min(0, m_Velocity.x);
		}
	}
	if (position.y < colliderPosition.y + colliderDimensions.y && position.y > colliderPosition.y) // Bottom
	{
		if (colliderPosition.x + colliderDimensions.x - position.x > m_TouchEpsilon/* ||
			position.x + dimensions.x - colliderPosition.x > m_TouchEpsilon*/) // check if beneath is an actual wall and the touch isn't a fluke
		{
			flags = TouchFlags(int(flags) | int(TouchFlags::Bottom));
			//m_Velocity.y = max(0, m_Velocity.y);
		}
	}
	if (m_UseOneWay)
		return flags;
	
	if (position.y + dimensions.y > colliderPosition.y && position.y + dimensions.y < colliderPosition.y + colliderDimensions.y) // Top
	{
		if (colliderPosition.x + colliderDimensions.x - position.x > m_TouchEpsilon/* ||
			position.x + dimensions.x - colliderPosition.x > m_TouchEpsilon*/) // check if above is an actual wall and the touch isn't a fluke
		{
			flags = TouchFlags(int(flags) | int(TouchFlags::Top));
			//m_Velocity.y = min(0, m_Velocity.y);
		}
	}

	return flags;
}

BaseComponent * BoxColliderComponent::Clone() const
{
	return new BoxColliderComponent{m_Dimensions.x, m_Dimensions.y};
}

void BoxColliderComponent::LoadFromJson( const nlohmann::json &json )
{
	m_Dimensions.x = json.at("Width").get<float>();
	m_Dimensions.y = json.at("Height").get<float>();
}
