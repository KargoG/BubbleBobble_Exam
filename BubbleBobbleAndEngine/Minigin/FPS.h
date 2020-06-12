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
	
private:
	TextComponent* m_pText;
};

