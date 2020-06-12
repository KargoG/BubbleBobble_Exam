#pragma once
#include "BaseComponent.h"
class BoxColliderComponent :
    public BaseComponent
{
public:
	BoxColliderComponent(float width, float height);
	// TODO
	virtual void Start(){};
	virtual void Update(){};
	virtual void PhysicsUpdate(){};
	virtual void Render() const{};
private:
	// TODO
	float m_Width, m_Height;
};

