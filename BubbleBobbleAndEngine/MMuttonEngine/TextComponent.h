#pragma once
#include "BaseComponent.h"

class Texture2D;
class Font;

class TextComponent :
	public BaseComponent
{
public:
	TextComponent(Font* font, const std::string& text = "Example Text");
	~TextComponent();
	
	virtual void Start() override;
	virtual void Update() override;
	virtual void PhysicsUpdate() override;
	virtual void Render() const override;

	void SetText(const std::string& text);

private:
	
	bool m_NeedsUpdate;
	
	std::string m_Text{};
	Font* m_pFont{};
	Texture2D* m_pTexture{};
};

