#include "MiniginPCH.h"
#include "PlayerControllerComponent.h"
#include "GameObject.h"
#include "RigidbodyComponent.h"
#include "Time.h"

void PlayerControllerComponent::Start()
{
	ControllerComponent::Start();
	
	m_Input = m_pGameObject->GetComponent<InputComponent>();

	if (!m_Input)
		throw std::exception("Player controller needs an InputComponent to work!\n");

	m_Input->SetControllerNumber(m_PlayerNumber);
	m_Input->RegisterCommand(ControllerButton::ButtonA, new JumpCommand{});
}

void PlayerControllerComponent::Update()
{
	Command* input = m_Input->ProcessInput();
	if (input)
		input->Execute(this);

	float xInput = m_Input->GetAxis(ControllerAxis::LeftThumbstickX);
	m_pRB->AddVelocity(xInput * m_WalkSpeed * Time::GetInstance().GetDeltaTime(), 0);
}

void PlayerControllerComponent::Jump()
{
	m_pRB->AddVelocity(0, 10);
}
