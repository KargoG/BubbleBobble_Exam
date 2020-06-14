#include "pch.h"
#include "AnimationComponent.h"
#include "Time.h"
#include "TextureRendererComponent.h"
#include "GameData.h"
#include "GameObject.h"
#include "ResourceManager.h"

AnimationComponent::AnimationComponent( SpriteAnimationState *pCurrentState ) : m_pCurrentState{ pCurrentState }
{
}

AnimationComponent::~AnimationComponent()
{
	delete m_pCurrentState;
}

void AnimationComponent::Start()
{
	m_pTextureRenderer = m_pGameObject->GetComponent<TextureRendererComponent>();

	if (!m_pTextureRenderer)
		throw std::exception("Animation Component requires a TextureRendererComponent to work!\n");

	if(!m_pCurrentState)
		throw std::exception("Animation Component requires at least 1 Animation state to work!\n");
}

void AnimationComponent::Update()
{
	SpriteAnimationState* newState = m_pCurrentState->Update(m_pTextureRenderer);

	if(newState)
	{
		delete m_pCurrentState;
		m_pCurrentState = newState;
		m_pCurrentState->OnEnter(m_pTextureRenderer);
	}
}

BaseComponent * AnimationComponent::Clone() const
{
	// TODO !!!!!!!!!!!!!
	return nullptr;
}

void AnimationComponent::LoadFromJson( const nlohmann::json & )
{
	// TODO !!!!!!!!!!!!!
}

SpriteAnimationState::SpriteAnimationState( const std::string &spriteSheetName )
{
	m_TextureName = spriteSheetName;
}

void SpriteAnimationState::OnEnter( TextureRendererComponent *pTextureRenderer )
{
	pTextureRenderer->SetTexture(m_TextureName.c_str());
	pTextureRenderer->SetRenderDimensions(0 * float(GameData::GetInstance().GetSpriteWidth()) + 0 * m_SpriteOffset, float(m_FirstSpriteY), float(GameData::GetInstance().GetSpriteWidth()), float(GameData::GetInstance().GetSpriteWidth()));
}

SpriteAnimationState* SpriteAnimationState::Update(TextureRendererComponent* pTextureRenderer)
{
	m_CycleProgress += Time::GetInstance().GetDeltaTime();
	if (m_CycleProgress >= m_CycleLength)
		m_CycleProgress -= m_CycleLength;
	
	float progressPercentage{ m_CycleProgress / m_CycleLength };

	int currentSprite{ int(progressPercentage * m_NumberOfSprites) };

	pTextureRenderer->SetRenderDimensions(currentSprite * float(GameData::GetInstance().GetSpriteWidth()) + currentSprite * m_SpriteOffset, float(m_FirstSpriteY), float(GameData::GetInstance().GetSpriteWidth()), float(GameData::GetInstance().GetSpriteWidth()));

	return UpdateState(pTextureRenderer->GetGameObject());
}
