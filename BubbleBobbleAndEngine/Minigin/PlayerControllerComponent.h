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
	void PhysicsUpdate() override{};
	void Render() const override{};
	BaseComponent* Clone() const override { return nullptr; }; // TODO!!!!!!!!
	void LoadFromJson( const nlohmann::json & ) override{}; // TODO!!!!!!!!

	void SetPlayerNumber(int playerNumber) { m_PlayerNumber = playerNumber; m_Input->SetControllerNumber(m_PlayerNumber); }

	void Jump() override;
	
private:
	InputComponent* m_Input{ nullptr };

	int m_PlayerNumber{0};

	float m_WalkSpeed{50};
};

class JumpCommand : public Command
{
public:
	virtual void Execute(ControllerComponent* controller) override
	{
		controller->Jump();
	};
};