#pragma once
#include "BaseComponent.h"
class RigidbodyComponent;

class ControllerComponent abstract :
    public BaseComponent
{
public:
	void Start() override;
	void Update() override = 0;
	void PhysicsUpdate() override = 0;
	void Render() const override = 0;
	BaseComponent * Clone() const override = 0;
	void LoadFromJson( const nlohmann::json &json ) override = 0;

	virtual void Jump() = 0;
	
protected:
	RigidbodyComponent* m_pRB{ nullptr };
};

