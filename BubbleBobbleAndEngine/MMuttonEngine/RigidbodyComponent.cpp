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
	TransformComponent* pPlayerTransform{ m_pGameObject->GetComponent<TransformComponent>() };
	const glm::vec2& playerPosition{ pPlayerTransform->GetPosition() };
	
	
	const std::vector<BoxColliderComponent*>& levelCollider{ m_pGameObject->GetScene()->GetCollider() };

	bool hasCollided{ false };
	
	for( BoxColliderComponent * pPlayerCollider : m_BoxColliders)
	{
		glm::vec2 playerDimensions{ pPlayerCollider->GetDimensions() };

		for( BoxColliderComponent * const pCollider : levelCollider)
		{
			glm::vec2 colliderDimensions{ pCollider->GetDimensions() };
			const glm::vec2& colliderPosition{ pCollider->GetGameObject()->GetComponent<TransformComponent>()->GetPosition() };

			bool collisionXAxis{ playerPosition.x < colliderPosition.x + colliderDimensions.x &&
								playerPosition.x + playerDimensions.x > colliderPosition.x };
			bool collisionYAxis{ playerPosition.y < colliderPosition.y + colliderDimensions.y &&
								playerPosition.y + playerDimensions.y > colliderPosition.y };


			if(collisionXAxis && collisionYAxis)
			{
				std::cout << "Colliding\n";
				m_Velocity.x = 0;
				m_Velocity.y = 0;

				hasCollided = true;

				if(playerPosition.x < colliderPosition.x + colliderDimensions.x && playerPosition.x > colliderPosition.x)
				{
					m_TouchFlags = TouchFlags(int(m_TouchFlags) | int(TouchFlags::Left));
				}
				if(playerPosition.x + playerDimensions.x > colliderPosition.x && playerPosition.x + playerDimensions.x < colliderPosition.x + colliderDimensions.x)
				{
					m_TouchFlags = TouchFlags(int(m_TouchFlags) | int(TouchFlags::Right));
				}
				if(playerPosition.y < colliderPosition.y + colliderDimensions.y && playerPosition.y > colliderPosition.y)
				{
					m_TouchFlags = TouchFlags(int(m_TouchFlags) | int(TouchFlags::Bottom));
				}
				if(playerPosition.y + playerDimensions.y > colliderPosition.y && playerPosition.y + playerDimensions.y < colliderPosition.y + colliderDimensions.y)
				{
					m_TouchFlags = TouchFlags(int(m_TouchFlags) | int(TouchFlags::Top));
				}
				
			}
		}
	}

	if(!hasCollided)
	{
		float dt{ Time::GetInstance().GetPhysicsDeltaTime() };
		m_Velocity.y += -9.81f * dt;

		pPlayerTransform->SetPosition(glm::vec3{ playerPosition + glm::vec2{ m_Velocity.x * dt, m_Velocity.y * dt } , 0 });
	}
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
