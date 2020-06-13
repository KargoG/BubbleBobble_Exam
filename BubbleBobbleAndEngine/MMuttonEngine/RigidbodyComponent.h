#pragma once
#include "BaseComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

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

	void AddCollider(BoxColliderComponent* pBoxCollider) { m_BoxColliders.push_back(pBoxCollider); };
private:
	std::vector<BoxColliderComponent*> m_BoxColliders{};
	glm::vec2 m_Velocity{0, 0};

	TouchFlags m_TouchFlags{ TouchFlags::None };
};

