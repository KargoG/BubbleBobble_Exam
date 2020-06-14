#pragma once
#include <ControllerComponent.h>

class AIState;

class AIControllerComponent :
    public ControllerComponent
{
public:
	void Update() override;
	void PhysicsUpdate() override;
	BaseComponent * Clone() const override;
	void LoadFromJson( const nlohmann::json &json ) override;
	virtual void Jump() override;

private:
	AIState *m_pCurrentState{};
};

class AIState
{
public:
	virtual ~AIState() = default;

	virtual void Update() = 0;
	virtual void PhysicsUpdate() = 0;
	virtual void Jump() = 0;

protected:
	ControllerComponent* m_pController{};
};

class WalkingState : public AIState
{
	void Update() override;
	void PhysicsUpdate() override;
	void Jump() override;
};