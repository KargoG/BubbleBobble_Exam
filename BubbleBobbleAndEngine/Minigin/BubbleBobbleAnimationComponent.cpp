#include "MiniginPCH.h"
#include "BubbleBobbleAnimationComponent.h"
#include "AnimationStates.h"

BaseComponent * BubbleBobbleAnimationComponent::Clone() const
{
	SpriteAnimationState *stateCopy{};

	if (m_pCurrentState)
		stateCopy = m_pCurrentState->Clone();
	
	return new BubbleBobbleAnimationComponent{ stateCopy };
}

void BubbleBobbleAnimationComponent::LoadFromJson( const nlohmann::json &json )
{
	std::string startingState = json.at("State").get<std::string>();
	if(startingState == "PlayerWalkingRight")
	{
		m_pCurrentState = new PlayerWalkingRight{};
	}
	else if(startingState == "PlayerWalkingLeft")
	{
		m_pCurrentState = new PlayerWalkingLeft{};
	}
	else if(startingState == "PlayerShootingRight")
	{
		m_pCurrentState = new PlayerShootingRight{};
	}
	else if(startingState == "PlayerShootingLeft")
	{
		m_pCurrentState = new PlayerShootingLeft{};
	}
	else if(startingState == "MaitaWalkingRight")
	{
		m_pCurrentState = new MaitaWalkingRight{};
	}
	else if(startingState == "MaitaWalkingLeft")
	{
		m_pCurrentState = new MaitaWalkingLeft{};
	}
	else if(startingState == "ZenChanWalkingRight")
	{
		m_pCurrentState = new ZenChanWalkingRight{};
	}
	else if(startingState == "ZenChanWalkingLeft")
	{
		m_pCurrentState = new ZenChanWalkingLeft{};
	}
	
}
