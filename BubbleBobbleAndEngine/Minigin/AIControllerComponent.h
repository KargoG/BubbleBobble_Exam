#pragma once
#include "ControllerComponent.h"
#include "Time.h"
#include "Subject.h"

class ColliderComponent;
class AIState;

class AIControllerComponent :
    public ControllerComponent
{
public:
	AIControllerComponent();
	~AIControllerComponent();
	
	void Update() override;
	void PhysicsUpdate() override;
	void OnTriggerEnter(const Collision*) override;
	void OnTriggerExit(const Collision*) override;
	BaseComponent * Clone() const override;
	void LoadFromJson( const nlohmann::json &json ) override;

	void SetLookingRight(bool lookingRight) { m_LookingRight = lookingRight; };
	void Shoot() override;
	void Jump() override;
	void TakeDamage() override;
	void Die() override;
	void OnCollisionEnter(const Collision* other) override;
	
	GameObject* GetTargetedPlayer() const;

	void SetInBubble(bool inBubble) { m_InBubble = inBubble; };
	bool GetInBubble() const { return m_InBubble; }
	
	int GetWallsLeft() const { return m_WallsLeft; }
	int GetWallsRight() const { return m_WallsRight; }
	int GetWallsBeneath() const { return m_WallsBeneath; }

	void AddObserver(Observer* observer) { m_AISubject.AddObserver(observer); };
	void RemoveObserver(Observer* observer){ m_AISubject.RemoveObserver(observer); }
	
private:
	AIState *m_pCurrentState{};
	bool m_InBubble{ false };

	Subject m_AISubject{};
	int m_WallsLeft{0};
	int m_WallsRight{0};
	int m_WallsBeneath{0};
};

class AIState
{
public:
	virtual ~AIState() = default;

	virtual AIState* Update(AIControllerComponent*){ return nullptr; };
	virtual AIState* PhysicsUpdate(AIControllerComponent*) { return nullptr; };
	virtual AIState* Jump(AIControllerComponent*) { return nullptr; };
	virtual AIState* TakeDamage(AIControllerComponent*) { return nullptr; };
	virtual AIState* OnCollision(const ColliderComponent*, AIControllerComponent*) { return nullptr; };
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
	AIState* OnCollision(const ColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new WalkingState{}; }
};

class FallingState : public AIState
{
public:
	AIState* PhysicsUpdate(AIControllerComponent*) override;
	AIState* TakeDamage(AIControllerComponent* controller) override;
	AIState* OnCollision(const ColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new FallingState{}; }
};

class JumpingUpState : public AIState
{
public:
	AIState* PhysicsUpdate(AIControllerComponent* controller) override;
	AIState* TakeDamage(AIControllerComponent* controller) override;
	AIState* OnCollision(const ColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new FallingState{}; }
};

class JumpingForwardState : public AIState
{
public:
	AIState* PhysicsUpdate(AIControllerComponent* controller) override;
	AIState* TakeDamage(AIControllerComponent* controller) override;
	AIState* OnCollision(const ColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new JumpingForwardState{}; }
};

class BubbleState : public AIState
{
public:
	AIState* Update(AIControllerComponent*) override;
	AIState* OnCollision(const ColliderComponent* other, AIControllerComponent* controller) override;
	AIState* Clone() override { return new BubbleState{}; }

private:
	float m_AccumulatedTime{};
	const float m_TimeInBubble{ 5.0f };
	
};