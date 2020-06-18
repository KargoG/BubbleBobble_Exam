#include "MiniginPCH.h"
#include "AIControllerComponent.h"
#include "Time.h"
#include "RigidbodyComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "PlayerControllerComponent.h"
#include "TransformComponent.h"
#include "ResourceManager.h"
#include "BulletBehaviourComponent.h"

AIControllerComponent::AIControllerComponent()
{
	m_pCurrentState = new WalkingState();
}

AIControllerComponent::~AIControllerComponent()
{
	delete m_pCurrentState;
}

void AIControllerComponent::Update()
{
	ControllerComponent::Update();

	AIState* newState = m_pCurrentState->Update(this);

	if (newState)
	{
		delete m_pCurrentState;
		m_pCurrentState = newState;
	}
}

void AIControllerComponent::PhysicsUpdate()
{
	AIState* newState = m_pCurrentState->PhysicsUpdate(this);

	if (newState)
	{
		delete m_pCurrentState;
		m_pCurrentState = newState;
	}
}

void AIControllerComponent::OnTriggerEnter( const Collision* other )
{
	glm::vec2 collCenter{ other->ownCollider->GetCenter() };
	if (collCenter.x < 0.5f)
		m_WallsLeft++;
	else if (collCenter.x > 0.5f)
		m_WallsRight++;
	else if (collCenter.y < 0.5f)
		m_WallsBeneath++;
}

void AIControllerComponent::OnTriggerExit( const Collision* other )
{
	glm::vec2 collCenter{ other->ownCollider->GetCenter() };
	if (collCenter.x < 0.5f)
		m_WallsLeft--;
	else if (collCenter.x > 0.5f)
		m_WallsRight--;
	else if (collCenter.y < 0.5f)
		m_WallsBeneath--;
}

BaseComponent * AIControllerComponent::Clone() const
{
	AIControllerComponent *cc{ new AIControllerComponent{} };

	delete cc->m_pCurrentState;
	
	cc->m_pCurrentState = m_pCurrentState->Clone();
	cc->m_LookingRight = m_LookingRight;
	cc->m_WalkSpeed = m_WalkSpeed;
	
	return cc;
}

void AIControllerComponent::LoadFromJson( const nlohmann::json &json )
{
	delete m_pCurrentState;
	m_pCurrentState = new WalkingState{};
	m_WalkSpeed = json.at("WalkSpeed").get<float>();
}

void AIControllerComponent::Shoot()
{
	GameObject* bubble{ ResourceManager::GetInstance().SpawnPrototype("Bullet") };
	m_pGameObject->GetScene()->Add(bubble);

	glm::vec3 pos{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };

	bubble->GetComponent<TransformComponent>()->SetPosition(pos);
	bubble->GetComponent<BulletBehaviourComponent>()->SetMovingRight(m_LookingRight);
}

void AIControllerComponent::Jump()
{
	AIState* newState = m_pCurrentState->Jump(this);

	if (newState)
	{
		delete m_pCurrentState;
		m_pCurrentState = newState;
	}
}

void AIControllerComponent::TakeDamage()
{
	AIState* newState = m_pCurrentState->TakeDamage(this);

	if (newState)
	{
		delete m_pCurrentState;
		m_pCurrentState = newState;
	}
}

void AIControllerComponent::Die()
{
	m_AISubject.Notify(this, BB_ENEMY_DIED);
	m_pGameObject->GetScene()->Remove(m_pGameObject);
	// TODO spawn points?
}

void AIControllerComponent::OnCollisionEnter( const Collision* other )
{
	AIState* newState = m_pCurrentState->OnCollision(other->otherCollider, this);

	if (newState)
	{
		delete m_pCurrentState;
		m_pCurrentState = newState;
	}
}

GameObject* AIControllerComponent::GetTargetedPlayer() const
{
	PlayerControllerComponent *pPlayer = m_pGameObject->GetScene()->GetObjectWithComponent<PlayerControllerComponent>();

	if (!pPlayer)
		return nullptr;
	
	return pPlayer->GetGameObject();
}

// States
AIState* WalkingState::Update(AIControllerComponent* controller)
{
	GameObject *m_pTargetedPlayer{ controller->GetTargetedPlayer() };
	if (m_pTargetedPlayer && m_pTargetedPlayer->GetComponent<TransformComponent>()->GetPosition().y > controller->GetGameObject()->GetComponent<TransformComponent>()->GetPosition().y)
	{
		controller->Jump();
		return nullptr;
	}
	
	return nullptr;
}

AIState * WalkingState::PhysicsUpdate( AIControllerComponent * controller)
{
	if (controller->IsLookingRight())
		controller->SetLookingRight(!(controller->GetWallsRight() > 0));
	else
		controller->SetLookingRight(controller->GetWallsLeft() > 0);

	if (controller->GetWallsBeneath() <= 0)
		return new FallingState{};

	
	if (controller->IsLookingRight())
		controller->GetRigidbody()->SetVelocity(controller->GetWalkSpeed(), 0);
	else
		controller->GetRigidbody()->SetVelocity(-controller->GetWalkSpeed(), 0);

	return nullptr;
}

AIState* WalkingState::Jump(AIControllerComponent*)
{
	return new JumpingUpState();
}

AIState * WalkingState::TakeDamage( AIControllerComponent * )
{
	return new BubbleState();
}

AIState * WalkingState::OnCollision( const BoxColliderComponent *other, AIControllerComponent * )
{
	if (other->GetGameObject()->GetTag() == "Player")
	{
		other->GetGameObject()->GetComponent<ControllerComponent>()->TakeDamage();
	}

	return nullptr;
}

AIState * FallingState::PhysicsUpdate(AIControllerComponent * controller)
{
	if (controller->GetWallsBeneath() > 0)
		return new WalkingState{};

	return nullptr;
}

AIState * FallingState::TakeDamage( AIControllerComponent * )
{
	return new BubbleState();
}

AIState * FallingState::OnCollision( const BoxColliderComponent *other, AIControllerComponent * )
{
	if (other->GetGameObject()->GetTag() == "Player")
	{
		other->GetGameObject()->GetComponent<ControllerComponent>()->TakeDamage();
	}

	return nullptr;
}

AIState* JumpingUpState::PhysicsUpdate(AIControllerComponent* controller)
{
	controller->GetRigidbody()->SetVelocity(0, 10);
	return new WalkingState{};
}

AIState * JumpingUpState::TakeDamage( AIControllerComponent * )
{
	return new BubbleState();
}

AIState * JumpingUpState::OnCollision( const BoxColliderComponent *other, AIControllerComponent * )
{
	if (other->GetGameObject()->GetTag() == "Player")
	{
		other->GetGameObject()->GetComponent<ControllerComponent>()->TakeDamage();
	}

	return nullptr;
}

AIState* JumpingForwardState::PhysicsUpdate( AIControllerComponent *controller )
{
	controller->GetRigidbody()->SetVelocity(50, 10);
	return new WalkingState{};
}

AIState * JumpingForwardState::TakeDamage( AIControllerComponent * )
{
	return new BubbleState();
}

AIState * JumpingForwardState::OnCollision( const BoxColliderComponent *other, AIControllerComponent * )
{
	if (other->GetGameObject()->GetTag() == "Player")
	{
		other->GetGameObject()->GetComponent<ControllerComponent>()->TakeDamage();
	}

	return nullptr;
}

AIState * BubbleState::Update( AIControllerComponent *controller )
{
	m_AccumulatedTime += Time::GetInstance().GetDeltaTime();
	
	if (m_AccumulatedTime >= m_TimeInBubble)
	{
		controller->SetInBubble(false);
		return new WalkingState();
	}
	
	controller->SetInBubble(true);
	return nullptr;
}

AIState * BubbleState::OnCollision( const BoxColliderComponent *other, AIControllerComponent *controller )
{
	if (other && other->GetGameObject()->GetTag() == "Player")
	{
		controller->Die();
	}

	return nullptr;
}
