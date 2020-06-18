#pragma once
#include "ControllerComponent.h"
#include "InputComponent.h"
#include "Subject.h"

class PlayerState;
class RigidbodyComponent;

class PlayerControllerComponent :
	public ControllerComponent
{
public:
	PlayerControllerComponent();
	~PlayerControllerComponent();
	
	void Start() override;
	void Update() override;
	void PhysicsUpdate() override;
	void OnTriggerEnter(const Collision *) override;
	void OnTriggerExit(const Collision *) override;
	BaseComponent* Clone() const override;
	void LoadFromJson(const nlohmann::json&) override;

	void SetPlayerNumber(int playerNumber) { m_PlayerNumber = playerNumber; if (m_Input) m_Input->SetControllerNumber(m_PlayerNumber); }

	void Jump() override;
	void Shoot() override;
	void TakeDamage() override;
	void Die();

	int GetPlayerNumber() const { return m_PlayerNumber; }
	InputComponent* GetInput() const { return m_Input; };
	void SetLookingRight(bool lookingRight) { m_LookingRight = lookingRight; };

	void AddObserver(Observer* observer) { m_PlayerSubject.AddObserver(observer); };
	void RemoveObserver(Observer* observer) { m_PlayerSubject.RemoveObserver(observer); }

	int GetTouchesBeneath() const { return m_TouchesBeneath; };
private:
	InputComponent* m_Input{ nullptr };

	int m_PlayerNumber{0};
	int m_PlayerLifes{ 4 };

	float m_XInput{};

	PlayerState* m_pCurrentState{};
	int m_TouchesBeneath{ 0 };
	
	Subject m_PlayerSubject{};
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

class PlayerState abstract
{
public:
	virtual ~PlayerState() = default;

	virtual PlayerState* Update(PlayerControllerComponent*){ return nullptr; };
	virtual PlayerState* PhysicsUpdate(PlayerControllerComponent*){ return nullptr; };
	virtual PlayerState* Jump(PlayerControllerComponent*) { return nullptr; };
	virtual PlayerState* Clone() = 0;

protected:
	float m_XInput{};
};

class PlayerWalkingState : public PlayerState
{
public:
	PlayerState* Update(PlayerControllerComponent* controller) override;
	PlayerState* PhysicsUpdate(PlayerControllerComponent* controller) override;
	PlayerState* Jump(PlayerControllerComponent* controller) override;
	PlayerState* Clone() override { return new PlayerWalkingState{}; }
};

class PlayerFallingState : public PlayerState
{
public:
	PlayerState* Update(PlayerControllerComponent*) override;
	PlayerState* PhysicsUpdate(PlayerControllerComponent*) override;
	PlayerState* Clone() override { return new PlayerFallingState{}; }
};

class PlayerJumpingState : public PlayerState
{
public:
	PlayerState* Update(PlayerControllerComponent*) override;
	PlayerState* PhysicsUpdate(PlayerControllerComponent* controller) override;
	PlayerState* Jump(PlayerControllerComponent*) override;
	PlayerState* Clone() override { return new PlayerJumpingState{}; }

private:
	float m_AccumulatedTime{};
	const float m_TimeInBubble{ 0.5f };
};