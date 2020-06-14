#include "MiniginPCH.h"
#include "PlayerControllerComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Time.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "BulletBehaviourComponent.h"

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
	
	Command* input = m_Input->ProcessInput();
	if (input)
		input->Execute(this);

	m_XInput = m_Input->GetAxis(ControllerAxis::LeftThumbstickX);
	if(m_LookingRight)
		m_LookingRight = m_XInput >= 0;
	else
		m_LookingRight = m_XInput > 0;
}

void PlayerControllerComponent::PhysicsUpdate()
{
	m_pRB->Move(m_XInput * m_WalkSpeed * Time::GetInstance().GetPhysicsDeltaTime(), 0);
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
	m_pRB->AddVelocity(0, 10);
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
	pos.y += 2;
	
	attack->GetComponent<TransformComponent>()->SetPosition(pos);
	attack->GetComponent<BulletBehaviourComponent>()->SetMovingRight(m_LookingRight);
}
