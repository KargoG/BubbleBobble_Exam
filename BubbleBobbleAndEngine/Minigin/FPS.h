#pragma once
#include "BaseComponent.h"

class TextComponent;

class FPS :
	public BaseComponent
{
public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void PhysicsUpdate() override;
	virtual void Render() const override;

	virtual BaseComponent * Clone() const override;
	virtual void LoadFromJson( const nlohmann::json &json ) override;
private:
	TextComponent* m_pText;
};

