#pragma once
#include "BaseComponent.h"
#include "RigidbodyComponent.h"

class BoxColliderComponent :
    public BaseComponent
{
public:
	BoxColliderComponent(float width = 1, float height = 1);
	~BoxColliderComponent() override;
	void Start() override;
	void Update() override {}
	void PhysicsUpdate() override {};
	void Render() const override;

	TouchFlags CalculateCollisions(const BoxColliderComponent* pOtherCollider);
	
	glm::vec2 GetDimensions() const { return m_Dimensions; }
	BaseComponent * Clone() const override;
	void LoadFromJson(const nlohmann::json& json) override;
private:
	glm::vec2 m_Dimensions;

	bool m_UseOneWay{ false };

	const float m_TouchEpsilon{ 0.1f };
};

