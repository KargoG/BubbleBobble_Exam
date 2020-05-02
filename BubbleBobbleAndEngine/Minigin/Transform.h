#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#include "BaseComponent.h"
#pragma warning(pop)

class Transform final : public BaseComponent
{
public:
	Transform() = default;
	Transform(float x, float y, float z = 0);
	Transform(const glm::vec3& position);
	
	virtual void Start() override{};
	virtual void Update() override{};
	virtual void PhysicsUpdate() override{};
	virtual void Render() const override{};
	
	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z = 0);
private:
	glm::vec3 m_Position;
};
