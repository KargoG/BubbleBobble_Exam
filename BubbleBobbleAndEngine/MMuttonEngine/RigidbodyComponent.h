#pragma once
#include "BaseComponent.h"
#include <box2d/b2_body.h>
#pragma warning(push)
#pragma warning (disable:4201)
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

struct Collision
{
	BoxColliderComponent *ownCollider;
	BoxColliderComponent*otherCollider;
};

class RigidbodyComponent :
    public BaseComponent
{
public:
	RigidbodyComponent(b2BodyType bodyType = b2_staticBody);
	~RigidbodyComponent();
	
	void Awake() override;
	void Update() override;
	void PhysicsUpdate() override;

	//void AddCollider(BoxColliderComponent* pBoxCollider) { m_BoxColliders.push_back(pBoxCollider); }
	b2Fixture* AddCollider(const b2FixtureDef &def);
	virtual BaseComponent * Clone() const override;

	void LoadFromJson(const nlohmann::json& json) override;
	void AddVelocity(float x, float y) const;
	void SetVelocity(float x, float y) const;
	glm::vec2 GetVelocity() const { return { m_pRigidBody->GetLinearVelocity().x, m_pRigidBody->GetLinearVelocity().y }; }
	b2Body* GetRB() const { return m_pRigidBody; };
	void Move(float x, float y);
	void SetPosition(float x, float y);
	
	void SetApplyGravity(bool applyGravity){ m_ApplyGravity = applyGravity; if (m_pRigidBody)m_pRigidBody->SetGravityScale(m_pRigidBody ? 1.f : 0.f); }
	void AddForce(float x, float y);

private:
	TransformComponent* m_pPlayerTransform{ nullptr };


	b2Body* m_pRigidBody{ nullptr };
	b2BodyType m_BodyType;

	bool m_ApplyGravity{ true };
};

