#include "MiniginPCH.h"
#include "AnimationStates.h"
#include "PlayerControllerComponent.h"
#include "GameObject.h"
#include "Time.h"
#include "AIControllerComponent.h"

SpriteAnimationState * PlayerWalkingRight::UpdateState( const GameObject *animatedObject )
{
	if (!animatedObject->GetComponent<PlayerControllerComponent>()->IsLookingRight())
		return new PlayerWalkingLeft{};
	
	return nullptr;
}

SpriteAnimationState * PlayerWalkingRight::Clone()
{
	PlayerWalkingRight* state{ new PlayerWalkingRight{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}

SpriteAnimationState * PlayerWalkingLeft::UpdateState( const GameObject *animatedObject )
{
	if (animatedObject->GetComponent<PlayerControllerComponent>()->IsLookingRight())
		return new PlayerWalkingRight{};

	return nullptr;
}

SpriteAnimationState * PlayerWalkingLeft::Clone()
{
	PlayerWalkingLeft* state{ new PlayerWalkingLeft{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}

SpriteAnimationState * PlayerShootingRight::UpdateState( const GameObject * )
{
	m_AccumulatedTime += Time::GetInstance().GetDeltaTime();
	if (m_AccumulatedTime > m_CycleLength)
		return new PlayerWalkingRight{};

	return nullptr;
}

SpriteAnimationState * PlayerShootingRight::Clone()
{
	PlayerShootingRight* state{ new PlayerShootingRight{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}

SpriteAnimationState * PlayerShootingLeft::UpdateState( const GameObject * )
{
	m_AccumulatedTime += Time::GetInstance().GetDeltaTime();
	if (m_AccumulatedTime > m_CycleLength)
		return new PlayerWalkingLeft{};

	return nullptr;
}

SpriteAnimationState * PlayerShootingLeft::Clone()
{
	PlayerShootingLeft* state{ new PlayerShootingLeft{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}

SpriteAnimationState * MaitaWalkingRight::UpdateState( const GameObject *animatedObject )
{
	AIControllerComponent *aiController = animatedObject->GetComponent<AIControllerComponent>();
	if (aiController && aiController->GetInBubble())
		return new MaitaInBubble{};
	if (!animatedObject->GetComponent<ControllerComponent>()->IsLookingRight())
		return new MaitaWalkingLeft{};

	return nullptr;
}

SpriteAnimationState * MaitaWalkingRight::Clone()
{
	MaitaWalkingRight* state{ new MaitaWalkingRight{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}

SpriteAnimationState * MaitaWalkingLeft::UpdateState( const GameObject *animatedObject )
{
	AIControllerComponent* aiController = animatedObject->GetComponent<AIControllerComponent>();
	if (aiController && aiController->GetInBubble())
		return new MaitaInBubble{};
	if (animatedObject->GetComponent<ControllerComponent>()->IsLookingRight())
		return new MaitaWalkingRight{};

	return nullptr;
}

SpriteAnimationState * MaitaWalkingLeft::Clone()
{
	MaitaWalkingLeft* state{ new MaitaWalkingLeft{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}

SpriteAnimationState * MaitaInBubble::UpdateState( const GameObject *animatedObject )
{
	if (!animatedObject->GetComponent<AIControllerComponent>()->GetInBubble())
	{
		if (animatedObject->GetComponent<AIControllerComponent>()->IsLookingRight())
			return new MaitaWalkingRight{};

		return new MaitaWalkingLeft{};
	}
	return nullptr;
}

SpriteAnimationState * MaitaInBubble::Clone()
{
	MaitaInBubble* state{ new MaitaInBubble{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}

SpriteAnimationState * ZenChanWalkingRight::UpdateState( const GameObject *animatedObject )
{
	if (animatedObject->GetComponent<AIControllerComponent>()->GetInBubble())
		return new ZenChanInBubble{};
	
	if (!animatedObject->GetComponent<AIControllerComponent>()->IsLookingRight())
		return new ZenChanWalkingLeft{};

	return nullptr;
}

SpriteAnimationState * ZenChanWalkingRight::Clone()
{
	ZenChanWalkingRight* state{ new ZenChanWalkingRight{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}

SpriteAnimationState * ZenChanWalkingLeft::UpdateState( const GameObject *animatedObject )
{
	if (animatedObject->GetComponent<AIControllerComponent>()->GetInBubble())
		return new ZenChanInBubble{};
	if (animatedObject->GetComponent<AIControllerComponent>()->IsLookingRight())
		return new ZenChanWalkingRight{};

	return nullptr;
}

SpriteAnimationState * ZenChanWalkingLeft::Clone()
{
	ZenChanWalkingLeft* state{ new ZenChanWalkingLeft{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}

SpriteAnimationState * ZenChanInBubble::UpdateState( const GameObject *animatedObject )
{
	if (!animatedObject->GetComponent<AIControllerComponent>()->GetInBubble())
	{
		if (animatedObject->GetComponent<AIControllerComponent>()->IsLookingRight())
			return new ZenChanWalkingRight{};
		
		return new ZenChanWalkingLeft{};
	}
	return nullptr;
}

SpriteAnimationState * ZenChanInBubble::Clone()
{
	ZenChanInBubble* state{ new ZenChanInBubble{} };
	state->m_CycleLength = m_CycleLength;
	state->m_CycleProgress = m_CycleProgress;
	state->m_FirstSpriteY = m_FirstSpriteY;
	state->m_NumberOfSprites = m_NumberOfSprites;
	state->m_SpriteOffset = m_SpriteOffset;

	return state;
}
