#pragma once
#include "BaseComponent.h"

class Texture2D;
class Font;

class TextComponent :
	public BaseComponent
{
public:
	TextComponent(const std::string& fontName, int fontSize, const std::string& text = "Example Text");
	~TextComponent();
	
	virtual void Start() override;
	virtual void Update() override;
	virtual void PhysicsUpdate() override;
	virtual void Render() const override;

	void SetFont(const std::string& fontName);
	void SetText(const std::string& text);
	// TODO Font size?

	virtual BaseComponent * Clone() const override;
	virtual void LoadFromJson( const nlohmann::json &json ) override;
private:
	
	bool m_NeedsUpdate;
	
	std::string m_Text{};
	std::string m_FontName{};
	int m_FontSize{};
	
	Font* m_pFont{};
	Texture2D* m_pTexture{};
};

