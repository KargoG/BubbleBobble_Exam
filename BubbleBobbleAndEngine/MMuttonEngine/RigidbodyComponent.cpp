#include "pch.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "BoxColliderComponent.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Time.h"
#include "GameData.h"
#if _DEBUG
#include "Renderer.h"
#endif

void RigidbodyComponent::Start()
{
	m_pPlayerTransform = m_pGameObject->GetComponent<TransformComponent>();
	
	BoxColliderComponent* rbCollider{ m_pGameObject->GetComponent<BoxColliderComponent>() };
	if (!rbCollider)
		throw std::exception("The Rigidbody needs a Collider to get instantiated!\n");
	
	m_pBoxCollider = rbCollider;
}

void RigidbodyComponent::Update()
{
}

void RigidbodyComponent::PhysicsUpdate()
{
	m_TouchFlags = TouchFlags::None;
	const glm::vec2& playerPosition{ m_pPlayerTransform->GetPosition() };
	
	
	const std::vector<BoxColliderComponent*>& levelCollider{ m_pGameObject->GetScene()->GetCollider() };

	glm::vec2 adjustedPosition{-1, -1};
	
	
	glm::vec2 playerDimensions{ m_pBoxCollider->GetDimensions() };

	for( BoxColliderComponent * const pCollider : levelCollider)
	{
		if (m_pBoxCollider == pCollider) // avoid selfcollision
			continue;
		if (int(m_pGameObject->GetIgnoredPhysicsLayers()) & int(pCollider->GetGameObject()->GetPhysicsLayer())) // avoid collision with ignored layers
			continue;
		
		int newFlags{ int(m_pBoxCollider->CalculateCollisions(pCollider)) };
		m_TouchFlags = TouchFlags(int(m_TouchFlags) | newFlags);
		
		if (newFlags == int(TouchFlags::None))
			continue;

		const glm::vec2& colliderPosition{ pCollider->GetGameObject()->GetComponent<TransformComponent>()->GetPosition() };
		const glm::vec2& colliderScale{ pCollider->GetGameObject()->GetComponent<TransformComponent>()->GetScale() };
		const glm::vec2 colliderDimensions{ pCollider->GetDimensions() * colliderScale };

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

		// Call OnCollision
		m_pGameObject->OnCollision(pCollider);
		pCollider->GetGameObject()->OnCollision(m_pBoxCollider);
	}
	

	float dt{ Time::GetInstance().GetPhysicsDeltaTime() };


	
	// Gravity
	if(m_ApplyGravity && (int(m_TouchFlags) & int(TouchFlags::Bottom)) == 0)
		m_Velocity.y += -98.1f * dt;


	// Velocity handling
	if (int(m_TouchFlags) & int(TouchFlags::Left))
		m_Velocity.x = max(0, m_Velocity.x);

	if (int(m_TouchFlags) & int(TouchFlags::Right))
		m_Velocity.x = min(0, m_Velocity.x);

	if (int(m_TouchFlags) & int(TouchFlags::Bottom))
		m_Velocity.y = max(0, m_Velocity.y);

	if (int(m_TouchFlags) & int(TouchFlags::Top))
		m_Velocity.y = min(0, m_Velocity.y);
	
	// Movement
	float newXPos{ playerPosition.x + m_Velocity.x * dt };
	float newYPos{ playerPosition.y + m_Velocity.y * dt };
	
	m_pPlayerTransform->SetPosition( (adjustedPosition.x > 0) ? adjustedPosition.x : newXPos, (adjustedPosition.y > 0) ? adjustedPosition.y : newYPos);
}

void RigidbodyComponent::Render() const
{
#if _DEBUG
	if(int(m_TouchFlags) != 0)
	{
		Renderer::GetInstance().SetRenderColor(255, 0, 0);

		const glm::vec2& bottomLeft{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };
		const glm::vec2& colliderScale{ m_pGameObject->GetComponent<TransformComponent>()->GetScale() };
		
		glm::vec2 topLeft{ bottomLeft };
		topLeft.y += GameData::GetInstance().GetSpriteHeight() * colliderScale.y;

		glm::vec2 bottomRight{ bottomLeft };
		bottomRight.x += GameData::GetInstance().GetSpriteWidth() * colliderScale.x;

		glm::vec2 topRight{ bottomRight };
		topRight.y += GameData::GetInstance().GetSpriteHeight() * colliderScale.y;
		
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

void RigidbodyComponent::SetCollider( BoxColliderComponent *pBoxCollider )
{
	if (m_pBoxCollider)
		delete m_pBoxCollider;

	m_pBoxCollider = pBoxCollider;
}

BaseComponent * RigidbodyComponent::Clone() const
{
	RigidbodyComponent* rb{ new RigidbodyComponent{} };
	rb->m_Velocity = m_Velocity;
	rb->m_ApplyGravity = m_ApplyGravity;
	return rb;
}

void RigidbodyComponent::LoadFromJson( const nlohmann::json &json )
{
	m_ApplyGravity = json.at("ApplyGravity").get<bool>();
}

void RigidbodyComponent::Move( float x, float y )
{
	m_pPlayerTransform->SetPosition(m_pPlayerTransform->GetPosition() + glm::vec3{ x , y, 0});
}
