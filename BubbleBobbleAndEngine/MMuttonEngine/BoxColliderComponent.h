#pragma once
#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include <box2d/b2_body.h>

class b2Body;

class BoxColliderComponent :
    public ColliderComponent
{
public:
	BoxColliderComponent(float width = 1, float height = 1, bool isTrigger = false, glm::vec2 center = {0, 0}, const PhysicsMat& pMat = PhysicsMat{ 0.1f, 0, 1 });
	void Start() override;
	void PreSolve( b2Contact * contact, const b2Manifold * manifold ) override;

	void SetUseOneWay(bool useOneWay) { m_UseOneWay = useOneWay; };
	
	glm::vec2 GetDimensions() const { return m_Dimensions; }
	BaseComponent * Clone() const override;
	void LoadFromJson(const nlohmann::json& json) override;
private:
	
	bool m_UseOneWay{ false };

	glm::vec2 m_Dimensions;
};

