#pragma once
#include <BaseComponent.h>

struct Collision;
class RigidbodyComponent;

class BulletBehaviourComponent :
	public BaseComponent
{
public:
	virtual void Start() override;
	virtual void PhysicsUpdate() override;
	virtual void OnTriggerEnter(const Collision* collision) override;
	virtual BaseComponent* Clone() const override;
	virtual void LoadFromJson(const nlohmann::json& json) override;

	void SetMovingRight(bool lookingRight) { m_MovingRight = lookingRight; }
private:
	RigidbodyComponent* m_pRB{ nullptr };

	float m_SpeedVertical{ 50.f };
	float m_SpeedHorizontal{ 50.f };

	bool m_TouchedWall{ false };
	bool m_MovingRight{ false };
};

