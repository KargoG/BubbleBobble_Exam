#pragma once
#include "BaseComponent.h"
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class BoxColliderComponent :
    public BaseComponent
{
public:
	BoxColliderComponent(float width, float height);
	// TODO
	void Start() override;
	void Update() override {}
	void PhysicsUpdate() override {};
	void Render() const override;
	
	glm::vec2 GetDimensions() const { return m_Dimensions; };
private:
	// TODO
	glm::vec2 m_Dimensions;
};

