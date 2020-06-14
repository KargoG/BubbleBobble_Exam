#pragma once
#include "AnimationComponent.h"

class PlayerWalkingLeft;

class PlayerWalkingRight final : public SpriteAnimationState
{
public:
	PlayerWalkingRight() : SpriteAnimationState("Textures/Sprites0.png") {}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState * Clone() override;
};

class PlayerWalkingLeft final : public SpriteAnimationState
{
public:
	PlayerWalkingLeft( ) : SpriteAnimationState( "Textures/Sprites0.png" )
	{
		m_FirstSpriteY = 16;
	}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState* Clone() override;
};

class PlayerShootingRight final : public SpriteAnimationState
{
public:
	PlayerShootingRight() : SpriteAnimationState("Textures/Sprites1.png")
	{
		m_FirstSpriteY = 32;
		m_CycleLength = 0.3f;
	}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState* Clone() override;

private:
	float m_AccumulatedTime{ 0 };
};

class PlayerShootingLeft final : public SpriteAnimationState
{
public:
	PlayerShootingLeft( ) : SpriteAnimationState( "Textures/Sprites1.png" )
	{
		m_FirstSpriteY = 48;
		m_CycleLength = 0.3f;
	}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState* Clone() override;
	
	float m_AccumulatedTime{ 0 };
};


class MaitaWalkingRight final : public SpriteAnimationState
{
public:
	MaitaWalkingRight() : SpriteAnimationState("Textures/Sprites0.png")
	{
		m_FirstSpriteY = 240;
	}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState* Clone() override;
};

class MaitaWalkingLeft final : public SpriteAnimationState
{
public:
	MaitaWalkingLeft( ) : SpriteAnimationState( "Textures/Sprites1.png" )
	{
		m_FirstSpriteY = 0;
	}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState* Clone() override;
};

class MaitaInBubble final : public SpriteAnimationState
{
public:
	MaitaInBubble() : SpriteAnimationState("Textures/Sprites2.png")
	{
		m_FirstSpriteY = 64;
	}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState* Clone() override;
};

class ZenChanWalkingRight final : public SpriteAnimationState
{
public:
	ZenChanWalkingRight() : SpriteAnimationState("Textures/Sprites0.png")
	{
		m_FirstSpriteY = 64;
	}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState* Clone() override;
};

class ZenChanWalkingLeft final : public SpriteAnimationState
{
public:
	ZenChanWalkingLeft( ) : SpriteAnimationState( "Textures/Sprites0.png" )
	{
		m_FirstSpriteY = 80;
	}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState* Clone() override;
};

class ZenChanInBubble final : public SpriteAnimationState
{
public:
	ZenChanInBubble( ) : SpriteAnimationState( "Textures/Sprites1.png" )
	{
		m_FirstSpriteY = 224;
	}

	SpriteAnimationState* UpdateState(const GameObject* animatedObject) override;
	virtual SpriteAnimationState* Clone() override;
};