#include "pch.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "BoxColliderComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Time.h"
#if _DEBUG
#include "Renderer.h"
#endif

void RigidbodyComponent::Start()
{
	BoxColliderComponent* rbCollider{ m_pGameObject->GetComponent<BoxColliderComponent>() };
	m_BoxColliders.push_back(rbCollider);
	m_pGameObject->GetScene()->RemoveCollider(rbCollider);
}

void RigidbodyComponent::Update()
{
}

void RigidbodyComponent::PhysicsUpdate()
{
	m_TouchFlags = TouchFlags::None;
	TransformComponent* pPlayerTransform{ m_pGameObject->GetComponent<TransformComponent>() };
	const glm::vec2& playerPosition{ pPlayerTransform->GetPosition() };
	
	
	const std::vector<BoxColliderComponent*>& levelCollider{ m_pGameObject->GetScene()->GetCollider() };

	glm::vec2 adjustedPosition{-1, -1};
	
	for( BoxColliderComponent * pPlayerCollider : m_BoxColliders)
	{
		glm::vec2 playerDimensions{ pPlayerCollider->GetDimensions() };

		for( BoxColliderComponent * const pCollider : levelCollider)
		{
			int newFlags{ int(pPlayerCollider->CalculateCollisions(pCollider)) };
			m_TouchFlags = TouchFlags(int(m_TouchFlags) | newFlags);


			if (newFlags == int(TouchFlags::None))
				continue;

			glm::vec2 colliderDimensions{ pCollider->GetDimensions() };
			const glm::vec2& colliderPosition{ pCollider->GetGameObject()->GetComponent<TransformComponent>()->GetPosition() };
			
			if(newFlags & int(TouchFlags::Left))
			{
				adjustedPosition.x = colliderPosition.x + colliderDimensions.x;
			}
			if(newFlags & int(TouchFlags::Right))
			{
				adjustedPosition.x = colliderPosition.x - playerDimensions.x;
			}
			if(newFlags & int(TouchFlags::Bottom))
			{
				adjustedPosition.y = colliderPosition.y + colliderDimensions.y;
			}
			if(newFlags & int(TouchFlags::Top))
			{
				adjustedPosition.y = colliderPosition.y - playerDimensions.y;
			}
			
		}
	}

	float dt{ Time::GetInstance().GetPhysicsDeltaTime() };


	if(int(m_TouchFlags) & int(TouchFlags::Left))
		m_Velocity.x = max(0, m_Velocity.x);
	
	if(int(m_TouchFlags) & int(TouchFlags::Right))
		m_Velocity.x = min(0, m_Velocity.x);

	if(int(m_TouchFlags) & int(TouchFlags::Bottom))
		m_Velocity.y = max(0, m_Velocity.y);

	if(int(m_TouchFlags) & int(TouchFlags::Top))
		m_Velocity.y = min(0, m_Velocity.y);

	
	
	if((int(m_TouchFlags) & int(TouchFlags::Bottom)) == 0)
		m_Velocity.y += -98.1f * dt;

	float newXPos{ playerPosition.x + m_Velocity.x * dt };
	float newYPos{ playerPosition.y + m_Velocity.y * dt };
	
	pPlayerTransform->SetPosition( (adjustedPosition.x > 0) ? adjustedPosition.x : newXPos, (adjustedPosition.y > 0) ? adjustedPosition.y : newYPos);
}

void RigidbodyComponent::Render() const
{
#if _DEBUG
	if(int(m_TouchFlags) != 0)
	{
		Renderer::GetInstance().SetRenderColor(255, 0, 0);

		const glm::vec2& bottomLeft{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };
		
		glm::vec2 topLeft{ bottomLeft };
		topLeft.y += 8;

		glm::vec2 bottomRight{ bottomLeft };
		bottomRight.x += 8;

		glm::vec2 topRight{ bottomRight };
		topRight.y += 8; // TODO bad literal
		
		if(int(m_TouchFlags) & int(TouchFlags::Right))
		{
			Renderer::GetInstance().RenderLine(bottomRight, topRight);
		}
		if(int(m_TouchFlags) & int(TouchFlags::Left))
		{
			Renderer::GetInstance().RenderLine(topLeft, bottomLeft);
		}
		if(int(m_TouchFlags) & int(TouchFlags::Top))
		{
			Renderer::GetInstance().RenderLine(topLeft, topRight);
		}
		if(int(m_TouchFlags) & int(TouchFlags::Bottom))
		{
			Renderer::GetInstance().RenderLine(bottomLeft, bottomRight);
		}
	}
#endif
}

BaseComponent * RigidbodyComponent::Clone() const
{
	RigidbodyComponent* rb{ new RigidbodyComponent{} };
	rb->m_Velocity = m_Velocity;
	return rb;
}
