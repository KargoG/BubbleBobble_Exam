#pragma once
#include "BaseComponent.h"
#include "RigidbodyComponent.h"

struct PhysicsMat
{
	float friction;
	float restitution;
	float density;
};

class ColliderComponent abstract :
    public BaseComponent
{
public:
	ColliderComponent(bool isTrigger = false, glm::vec2 center = { 0, 0 }, const PhysicsMat& pMat = PhysicsMat{ 0.1f, 0, 1 });
	~ColliderComponent();
	void Start() override;
	virtual void PreSolve(b2Contact*, const b2Manifold*){};
	
	glm::vec2 GetCenter() const { return m_Center; };
	BaseComponent* Clone() const override = 0;
	void LoadFromJson(const nlohmann::json& json) override;
	
protected:
	glm::vec2 m_Center;

	RigidbodyComponent* m_pRB{ nullptr };
	b2Fixture* m_pShape{ nullptr };

	bool m_IsTrigger{ false };
	PhysicsMat m_Mat{};
};

