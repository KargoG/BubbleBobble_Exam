#pragma once
#include <ControllerComponent.h>
#include "InputComponent.h"

class RigidbodyComponent;

class PlayerControllerComponent :
	public ControllerComponent
{
public:
	void Start() override;
	void Update() override;
	void PhysicsUpdate() override {};
	void TakeDamage();
	void Render() const override {};
	BaseComponent* Clone() const override;
	void LoadFromJson(const nlohmann::json&) override;

	void SetPlayerNumber(int playerNumber) { m_PlayerNumber = playerNumber; if (m_Input) m_Input->SetControllerNumber(m_PlayerNumber); }

	void Jump() override;
	void Shoot() override;

	bool IsLookingRight() const { return m_LookingRight; }
	
private:
	InputComponent* m_Input{ nullptr };

	int m_PlayerNumber{0};
	int m_PlayerLifes{ 4 };

	float m_WalkSpeed{50};
	bool m_LookingRight{ true };
};

class JumpCommand : public Command
{
public:
	virtual void Execute(ControllerComponent* controller) override
	{
		controller->Jump();
	};
};

class ShootCommand : public Command
{
public:
	virtual void Execute(ControllerComponent* controller) override
	{
		controller->Shoot();
	};
};