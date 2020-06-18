#pragma once
#include "BaseComponent.h"
#include "RigidbodyComponent.h"
#include <box2d/b2_body.h>

class b2Body;

struct PhysicsMat
{
	float friction;
	float restitution;
	float density;
};

class BoxColliderComponent :
    public BaseComponent
{
public:
	BoxColliderComponent(float width = 1, float height = 1, bool isTrigger = false, glm::vec2 center = {0, 0}, const PhysicsMat& pMat = PhysicsMat{ 0.1f, 0, 1 });
	~BoxColliderComponent();
	void Start() override;
	void PreSolve( b2Contact * contact, const b2Manifold * manifold );

	void SetUseOneWay(bool useOneWay) { m_UseOneWay = useOneWay; };
	
	glm::vec2 GetDimensions() const { return m_Dimensions; }
	glm::vec2 GetCenter() const { return m_Center; };
	BaseComponent * Clone() const override;
	void LoadFromJson(const nlohmann::json& json) override;
private:
	glm::vec2 m_Center;
	
	bool m_UseOneWay{ false };
	
	RigidbodyComponent* m_pRB{ nullptr };
	b2Fixture* m_pBox{ nullptr };

	glm::vec2 m_Dimensions;
	bool m_IsTrigger{ false };
	PhysicsMat m_Mat{};
};

