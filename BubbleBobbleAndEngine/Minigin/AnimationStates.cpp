#include "MiniginPCH.h"
#include "AnimationStates.h"
#include "PlayerControllerComponent.h"
#include "GameObject.h"
#include "Time.h"

SpriteAnimationState * PlayerWalkingRight::UpdateState( const GameObject *animatedObject )
{
	if (!animatedObject->GetComponent<PlayerControllerComponent>()->IsLookingRight())
		return new PlayerWalkingLeft{};
	
	return nullptr;
}

SpriteAnimationState * PlayerWalkingLeft::UpdateState( const GameObject *animatedObject )
{
	if (animatedObject->GetComponent<PlayerControllerComponent>()->IsLookingRight())
		return new PlayerWalkingRight{};

	return nullptr;
}

SpriteAnimationState * PlayerShootingRight::UpdateState( const GameObject * )
{
	m_AccumulatedTime += Time::GetInstance().GetDeltaTime();
	if (m_AccumulatedTime > m_CycleLength)
		return new PlayerWalkingRight{};

	return nullptr;
}

SpriteAnimationState * PlayerShootingLeft::UpdateState( const GameObject * )
{
	m_AccumulatedTime += Time::GetInstance().GetDeltaTime();
	if (m_AccumulatedTime > m_CycleLength)
		return new PlayerWalkingLeft{};

	return nullptr;
}

SpriteAnimationState * MaitaWalkingRight::UpdateState( const GameObject *animatedObject )
{
	if (!animatedObject->GetComponent<PlayerControllerComponent>()->IsLookingRight())
		return new MaitaWalkingLeft{};

	return nullptr;
}

SpriteAnimationState * MaitaWalkingLeft::UpdateState( const GameObject *animatedObject )
{
	if (animatedObject->GetComponent<PlayerControllerComponent>()->IsLookingRight())
		return new MaitaWalkingRight{};

	return nullptr;
}
