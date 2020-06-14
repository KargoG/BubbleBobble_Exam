#pragma once
#include "BaseComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
//#include <glm/vec2.hpp>
#include "../3rdParty/glm/glm/detail/type_vec2.hpp"
#pragma warning(pop)

class TransformComponent;
class BoxColliderComponent;

enum class TouchFlags
{
	Bottom = 1,
	Top = 2,
	Left = 4,
	Right = 8,
	None = 0
};


class RigidbodyComponent :
    public BaseComponent
{
public:
	void Start() override;
	void Update() override;
	void PhysicsUpdate() override;
	void Render() const override;

	//void AddCollider(BoxColliderComponent* pBoxCollider) { m_BoxColliders.push_back(pBoxCollider); }
	void SetCollider(BoxColliderComponent* pBoxCollider);
	virtual BaseComponent * Clone() const override;

	void LoadFromJson(const nlohmann::json& json) override;
	void AddVelocity(float x, float y) { m_Velocity += glm::vec2{ x, y }; }
	void Move(float x, float y);
	
	TouchFlags GetTouchFlags() const { return m_TouchFlags; };

	
private:
	TransformComponent* m_pPlayerTransform{ nullptr };
	
	BoxColliderComponent* m_pBoxCollider{};
	glm::vec2 m_Velocity{0, 0};

	bool m_ApplyGravity{ true };
	
	TouchFlags m_TouchFlags{ TouchFlags::None };
};

