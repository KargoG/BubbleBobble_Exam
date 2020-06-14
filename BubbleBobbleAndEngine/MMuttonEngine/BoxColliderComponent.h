#pragma once
#include "BaseComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#include "RigidbodyComponent.h"
#pragma warning(pop)

class BoxColliderComponent :
    public BaseComponent
{
public:
	BoxColliderComponent(float width = 1, float height = 1);
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

