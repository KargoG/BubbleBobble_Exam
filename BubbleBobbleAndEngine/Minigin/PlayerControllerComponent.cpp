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
	m_Input->RegisterCommand(ControllerButton::ButtonX, new ShootCommand{});
}

void PlayerControllerComponent::Update()
{
	ControllerComponent::Update();
	
	Command* input = m_Input->ProcessInput();
	if (input)
		input->Execute(this);

	float xInput = m_Input->GetAxis(ControllerAxis::LeftThumbstickX);
	m_pRB->AddVelocity(xInput * m_WalkSpeed * Time::GetInstance().GetDeltaTime(), 0);

	if(m_LookingRight)
		m_LookingRight = xInput >= 0;
	else
		m_LookingRight = xInput > 0;
}

void PlayerControllerComponent::TakeDamage()
{
	m_PlayerLifes--;
	// TODO
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
}

void PlayerControllerComponent::Jump()
{
	m_pRB->AddVelocity(0, 10);
}

void PlayerControllerComponent::Shoot()
{
	GameObject *bubble{ ResourceManager::GetInstance().SpawnPrototype("Bubble") };
	m_pGameObject->GetScene()->Add(bubble);

	glm::vec3 pos{ m_pGameObject->GetComponent<TransformComponent>()->GetPosition() };

	bubble->GetComponent<TransformComponent>()->SetPosition(pos);
	bubble->GetComponent<BulletBehaviourComponent>()->SetMovingRight(m_LookingRight);
}
