#pragma once
#include "BaseComponent.h"

class Texture2D;
class Font;

class TextComponent :
	public BaseComponent
{
public:
	TextComponent(const std::shared_ptr<Font>& font, const std::string& text = "Example Text");
	
	virtual void Start() override;
	virtual void Update() override;
	virtual void PhysicsUpdate() override;
	virtual void Render() const override;

	void SetText(const std::string& text);

private:
	
	bool m_NeedsUpdate;
	
	std::string m_Text{};
	std::shared_ptr<Font> m_Font{};
	std::shared_ptr<Texture2D> m_Texture{};
};

