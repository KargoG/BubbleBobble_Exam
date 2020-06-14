#pragma once
#include <AnimationComponent.h>

class BubbleBobbleAnimationComponent :
    public AnimationComponent
{
public:
	BubbleBobbleAnimationComponent(SpriteAnimationState* pCurrentState) : AnimationComponent{ pCurrentState } {}
	
	virtual BaseComponent * Clone() const override;
	virtual void LoadFromJson( const nlohmann::json &json ) override;
};

