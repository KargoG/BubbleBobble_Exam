#include "MiniginPCH.h"
#include "AIControllerComponent.h"

void AIControllerComponent::Update()
{
	m_pCurrentState->Update();
}

void AIControllerComponent::PhysicsUpdate()
{
	m_pCurrentState->PhysicsUpdate();
}

BaseComponent * AIControllerComponent::Clone() const
{
	// TODO fill clone method
	return nullptr;
}

void AIControllerComponent::LoadFromJson( const nlohmann::json & )
{
	// TODO load from json
}

void AIControllerComponent::Jump()
{
	m_pCurrentState->Jump();
}





// States

void WalkingState::Update()
{
	
}

void WalkingState::PhysicsUpdate()
{
	
}

void WalkingState::Jump()
{
	
}
