#pragma once
#include "BaseComponent.h"

class Texture2D;
class TextureRendererComponent;
class SpriteAnimationState;

class AnimationComponent :
    public BaseComponent
{
public:
	AnimationComponent(SpriteAnimationState* pCurrentState);
	~AnimationComponent() override;
	
	virtual void Start() override;
	virtual void Update() override;
	
	virtual void PhysicsUpdate() override{}
	virtual void Render() const override{}
	virtual BaseComponent * Clone() const override;
	virtual void LoadFromJson( const nlohmann::json &json ) override;
private:
	// Only sprite animation support
	SpriteAnimationState* m_pCurrentState{ nullptr };
	TextureRendererComponent* m_pTextureRenderer{ nullptr };
};

class SpriteAnimationState
{
public:
	SpriteAnimationState(const std::string& spriteSheetName);
	virtual ~SpriteAnimationState() = default;

	virtual void OnEnter(TextureRendererComponent* pTextureRenderer);
	SpriteAnimationState* Update(TextureRendererComponent* pTextureRenderer);

	virtual SpriteAnimationState* UpdateState(const GameObject*) { return nullptr; };

protected:
	float m_CycleProgress{ 0.f };
	float m_CycleLength{ 1.0f };
	int m_NumberOfSprites{ 8 };
	float m_SpriteOffset{ 18.2f };
	int m_FirstSpriteY{ 0 };
	
private:
	std::string m_TextureName{};
};