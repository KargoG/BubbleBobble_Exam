#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)
#include "BaseComponent.h"

class TransformComponent final : public BaseComponent
{
public:
	TransformComponent(float x, float y, float z = 0);
	TransformComponent(const glm::vec3& position);
	
	virtual void Start() override{};
	virtual void Update() override{};
	virtual void PhysicsUpdate() override{};
	virtual void Render() const override{};
	
	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z = 0);
	void SetPosition(const glm::vec3& position){ m_Position = position;	}

	BaseComponent * Clone() const override;
	void LoadFromJson( const nlohmann::json &json ) override;
private:
	glm::vec3 m_Position{};
};
