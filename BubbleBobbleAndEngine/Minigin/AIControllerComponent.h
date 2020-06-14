#pragma once
#include "ControllerComponent.h"
#include "Time.h"
#include "Subject.h"

class AIState;

class AIControllerComponent :
    public ControllerComponent
{
public:
	AIControllerComponent();
	~AIControllerComponent();
	
	void Update() override;
	void PhysicsUpdate() override;
	BaseComponent * Clone() const override;
	void LoadFromJson( const nlohmann::json &json ) override;

	void Render() const override{}
	void SetLookingRight(bool lookingRight) { m_LookingRight = lookingRight; };
	void Shoot() override;
	void Jump() override;
	void TakeDamage() override;
	void Die() override;
	void OnCollision(const BoxColliderComponent* other) override;
	
	GameObject* GetTargetedPlayer() const;

	void SetInBubble(bool inBubble) { m_InBubble = inBubble; };
	bool GetInBubble() const { return m_InBubble; }

	void AddObserver(Observer* observer) { m_AISubject.AddObserver(observer); };
	void RemoveObserver(Observer* observer){ m_AISubject.RemoveObserver(observer); }
private:
	AIState *m_pCurrentState{};
	bool m_InBubble{ false };

	Subject m_AISubject{};
};

class AIState
{
public:
	virtual ~AIState() = default;

	virtual AIState* Update(AIControllerComponent*controller) = 0;
	virtual AIState* PhysicsUpdate(AIControllerComponent* controller) = 0;
	virtual AIState* Jump(AIControllerComponent* controller) = 0;
	virtual AIState* TakeDamage(AIControllerComponent* controller) = 0;
	virtual AIState* OnCollision(const BoxColliderComponent* other, AIControllerComponent* controller) = 0;
	virtual AIState * Clone() = 0;

protected:
	ControllerComponent* m_pController{};
};

class WalkingState : public AIState
{
public:
	AIState* Update(AIControllerComponent* controller) override;
	AIState* PhysicsUpdate(AIControllerComponent* controller) override;
	AIState* Jump(AIControllerComponent* controller) override;
	AIState* TakeDamage(AIControllerComponent* controller) override;
	AIState* OnCollision(const BoxColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new WalkingState{}; }
};

class FallingState : public AIState
{
public:
	AIState* Update(AIControllerComponent*) override { return nullptr; };
	AIState* PhysicsUpdate(AIControllerComponent*) override;
	AIState* Jump(AIControllerComponent*) override{ return nullptr; };
	AIState* TakeDamage(AIControllerComponent* controller) override;
	AIState* OnCollision(const BoxColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new FallingState{}; }
};

class JumpingUpState : public AIState
{
public:
	AIState* Update(AIControllerComponent*) override{ return nullptr; };
	AIState* PhysicsUpdate(AIControllerComponent* controller) override;
	AIState* Jump(AIControllerComponent*) override{ return nullptr; };
	AIState* TakeDamage(AIControllerComponent* controller) override;
	AIState* OnCollision(const BoxColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new FallingState{}; }
};

class JumpingForwardState : public AIState
{
public:
	AIState* Update(AIControllerComponent*) override{ return nullptr; };
	AIState* PhysicsUpdate(AIControllerComponent* controller) override;
	AIState* Jump(AIControllerComponent*) override{ return nullptr; };
	AIState* TakeDamage(AIControllerComponent* controller) override;
	AIState* OnCollision(const BoxColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new JumpingForwardState{}; }
};

class BubbleState : public AIState
{
public:
	AIState* Update(AIControllerComponent*) override;
	AIState* PhysicsUpdate(AIControllerComponent*) override { return nullptr; };
	AIState* Jump(AIControllerComponent*) override{ return nullptr; };
	AIState* TakeDamage(AIControllerComponent*) override { return nullptr; };
	AIState* OnCollision(const BoxColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new BubbleState{}; }

private:
	float m_AccumulatedTime{};
	const float m_TimeInBubble{ 5.0f };
	
};