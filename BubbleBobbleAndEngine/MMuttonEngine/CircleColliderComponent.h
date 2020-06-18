#pragma once
#include "ColliderComponent.h"


class CircleColliderComponent :
    public ColliderComponent
{
public:
	CircleColliderComponent(float radius = 0.5f, bool isTrigger = false, glm::vec2 center = { 0, 0 }, const PhysicsMat& pMat = PhysicsMat{ 0.1f, 0, 1 });
	void Start() override;

	void SetUseOneWay(bool useOneWay) { m_UseOneWay = useOneWay; };

	float GetRadius() const { return m_Radius; }
	BaseComponent* Clone() const override;
	void LoadFromJson(const nlohmann::json& json) override;
private:

	bool m_UseOneWay{ false };

	float m_Radius;
};

