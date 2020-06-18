#include "MiniginPCH.h"
#include "PlayerControllerComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Time.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "BulletBehaviourComponent.h"

PlayerControllerComponent::PlayerControllerComponent() : m_pCurrentState{ new PlayerWalkingState{} }
{
}

PlayerControllerComponent::~PlayerControllerComponent()
{
	delete m_pCurrentState;
}

void PlayerControllerComponent::Start()
{
	ControllerComponent::Start();
	
	m_Input = m_pGameObject->GetComponent<InputComponent>();

	if (!m_Input)
		throw std::exception("Player controller needs an InputComponent to work!\n");

	m_Input->SetControllerNumber(m_PlayerNumber);
	m_Input->RegisterCommand(ControllerButton::ButtonA, new JumpCommand{});
	m_Input->RegisterCommand(ControllerButton::ButtonY, new ShootCommand{});
}

void PlayerControllerComponent::Update()
{
	ControllerComponent::Update();

	PlayerState* newState = m_pCurrentState->Update(this);

	if (newState)
	{
		delete m_pCurrentState;
		m_pCurrentState = newState;
	}
}

void PlayerControllerComponent::PhysicsUpdate()
{

	PlayerState* newState = m_pCurrentState->PhysicsUpdate(this);

	if (newState)
	{
		delete m_pCurrentState;
		m_pCurrentState = newState;
	}
}

void PlayerControllerComponent::OnTriggerEnter( const Collision * )
{
	//if(coll->otherCollider->GetGameObject()->GetTag() == "Wall")
		m_TouchesBeneath++;
}

void PlayerControllerComponent::OnTriggerExit( const Collision * )
{
	//if (coll->otherCollider->GetGameObject()->GetTag() == "Wall")
		m_TouchesBeneath--;
}

void PlayerControllerComponent::TakeDamage()
{
	m_PlayerLifes--;

	if (m_PlayerLifes < 0)
		Die();
	
	m_PlayerSubject.Notify(this, BB_PLAYER_TAKES_DAMAGE);
}

void PlayerControllerComponent::Die()
{
	m_PlayerSubject.Notify(this, BB_PLAYER_DIED);
	m_pGameObject->GetScene()->Remove(m_pGameObject);
}

BaseComponent * PlayerControllerComponent::Clone() const
{
	PlayerControllerComponent *pcc{ new PlayerControllerComponent{} };
	pcc->m_WalkSpeed = m_WalkSpeed;
	pcc->m_PlayerNumber = m_PlayerNumber;
	pcc->m_LookingRight = m_LookingRight;

	return pcc;
}

void PlayerControllerComponent::LoadFromJson( const nlohmann::json &json )
{
	m_WalkSpeed = json.at("WalkSpeed").get<float>();
	m_PlayerNumber = json.at("PlayerNumber").get<int>();
}

void PlayerControllerComponent::Jump()
{
	PlayerState* newState = m_pCurrentState->Jump(this);

	if (newState)
	{
		delete m_pCurrentState;
		m_pCurrentState = newState;
	}
}

void PlayerControllerComponent::Shoot()
{
	GameObject *attack{};

	if (GetGameObject()->GetPhysicsLayer() == PhysicsLayer::Layer01) // Bubble
	{
		attack = ResourceManager::GetInstance().SpawnPrototype("Bubble");
	}
	else
	{
		attack = ResourceManager::GetInstance().SpawnPrototype("Bullet");
	}

	m_pGameObject->GetScene()->Add(attack);

	glm::vec3 pos{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };
	pos.y += 0.2f;
	pos.x += m_LookingRight ? 2 : -2;
	
	attack->GetComponent<TransformComponent>()->SetPosition(pos);
	attack->GetComponent<BulletBehaviourComponent>()->SetMovingRight(m_LookingRight);
}


PlayerState* PlayerWalkingState::Update(PlayerControllerComponent* controller)
{
	Command* input = controller->GetInput()->ProcessInput();
	if (input)
		input->Execute(controller);

	m_XInput = controller->GetInput()->GetAxis(ControllerAxis::LeftThumbstickX);
	if (controller->IsLookingRight())
		controller->SetLookingRight(m_XInput >= 0);
	else
		controller->SetLookingRight(m_XInput > 0);

	return nullptr;
}

PlayerState * PlayerWalkingState::PhysicsUpdate( PlayerControllerComponent *controller )
{
	if (abs(m_XInput) > 0)
		controller->GetRigidbody()->SetVelocity(m_XInput * controller->GetWalkSpeed(), controller->GetRigidbody()->GetVelocity().y);
	else
		controller->GetRigidbody()->SetVelocity(0, controller->GetRigidbody()->GetVelocity().y);

	return nullptr;
}

PlayerState * PlayerWalkingState::Jump( PlayerControllerComponent *controller )
{
	controller->GetRigidbody()->SetVelocity(controller->GetRigidbody()->GetVelocity().x, 10);

	return new PlayerJumpingState{};
}

PlayerState * PlayerFallingState::Update( PlayerControllerComponent *controller )
{
	Command* input = controller->GetInput()->ProcessInput();
	if (input)
		input->Execute(controller);

	m_XInput = controller->GetInput()->GetAxis(ControllerAxis::LeftThumbstickX);
	if (controller->IsLookingRight())
		controller->SetLookingRight(m_XInput >= 0);
	else
		controller->SetLookingRight(m_XInput > 0);

	return controller->GetTouchesBeneath() > 0 ? new PlayerWalkingState() : nullptr;
}

PlayerState * PlayerFallingState::PhysicsUpdate( PlayerControllerComponent *controller )
{
	if (abs(m_XInput) > 0)
		controller->GetRigidbody()->SetVelocity(m_XInput * controller->GetWalkSpeed(), min(controller->GetRigidbody()->GetVelocity().y, 0.f));
	else
		controller->GetRigidbody()->SetVelocity(0, min(controller->GetRigidbody()->GetVelocity().y, 0.f));

	return nullptr;
}

PlayerState * PlayerJumpingState::Update( PlayerControllerComponent *controller )
{
	Command* input = controller->GetInput()->ProcessInput();
	if (input)
		input->Execute(controller);

	m_XInput = controller->GetInput()->GetAxis(ControllerAxis::LeftThumbstickX);
	if (controller->IsLookingRight())
		controller->SetLookingRight(m_XInput >= 0);
	else
		controller->SetLookingRight(m_XInput > 0);

	m_AccumulatedTime += Time::GetInstance().GetDeltaTime();
	
	return m_AccumulatedTime >= m_TimeInBubble ? new PlayerFallingState{} : nullptr;
}

PlayerState * PlayerJumpingState::PhysicsUpdate( PlayerControllerComponent *controller )
{
	if (abs(m_XInput) > 0)
		controller->GetRigidbody()->SetVelocity(m_XInput * controller->GetWalkSpeed(), controller->GetRigidbody()->GetVelocity().y);
	else
		controller->GetRigidbody()->SetVelocity(0, controller->GetRigidbody()->GetVelocity().y);

	return nullptr;
}

PlayerState * PlayerJumpingState::Jump( PlayerControllerComponent *controller )
{
	controller->GetRigidbody()->SetVelocity(controller->GetRigidbody()->GetVelocity().x, 10);

	return nullptr;
}
