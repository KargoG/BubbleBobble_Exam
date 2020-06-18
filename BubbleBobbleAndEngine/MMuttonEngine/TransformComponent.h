#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
//#include <glm/vec3.hpp>
#include "../3rdParty/glm/glm/detail/type_vec3.hpp"
#pragma warning(pop)
#include "BaseComponent.h"

class RigidbodyComponent;

class TransformComponent final : public BaseComponent
{
public:
	TransformComponent() = default;
	TransformComponent(float x, float y, float z = 0);
	TransformComponent(const glm::vec3& position);
	TransformComponent(const glm::vec3& position, const glm::vec3& scale);
	
	virtual void Start() override;
	virtual void Update() override{};
	virtual void PhysicsUpdate() override{};
	virtual void Render() const override{};
	
	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z = 0, bool updateRB = true);
	void SetPosition(const glm::vec3& position, bool updateRB = true);
	
	const glm::vec3& GetScale() const { return m_Scale; }
	void SetScale(float x, float y, float z = 0);
	void SetScale(const glm::vec3& scale){ m_Scale = scale; }

	BaseComponent * Clone() const override;
	void LoadFromJson( const nlohmann::json &json ) override;
private:
	RigidbodyComponent* m_pRB{};
	
	glm::vec3 m_Position{0, 0, 0};
	glm::vec3 m_Scale{1, 1, 1};
};
