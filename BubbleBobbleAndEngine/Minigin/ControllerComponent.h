#pragma once
#include "BaseComponent.h"
class RigidbodyComponent;

class ControllerComponent abstract :
    public BaseComponent
{
public:
	void Start() override;
	void Update() override;
	void PhysicsUpdate() override = 0;
	void Render() const override = 0;
	BaseComponent * Clone() const override = 0;
	void LoadFromJson( const nlohmann::json &json ) override = 0;

	virtual void Jump() = 0;
	virtual void Shoot() = 0;
	virtual void TakeDamage() = 0;
	virtual void Die() = 0;

	bool IsLookingRight() const { return m_LookingRight; }
	RigidbodyComponent* GetRigidbody() const { return m_pRB; };
	float GetWalkSpeed() const { return m_WalkSpeed; };
protected:
	RigidbodyComponent* m_pRB{ nullptr };

	float m_WalkSpeed{ 50 };
	bool m_LookingRight{ true };
};

