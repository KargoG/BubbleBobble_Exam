#pragma once
#include "Transform.h"
#include "GameObject.h"

class Font;
class Texture2D;
class TextObject final : public GameObject
{
public:
	void Update() override;
	void PhysicsUpdate() override;
	void Render() const override;

	//void SetText(const std::string& text);
	//void SetPosition(float x, float y);

	explicit TextObject(const std::string& text, const std::shared_ptr<Font>& font);
	virtual ~TextObject() = default;
	TextObject(const TextObject& other) = delete;
	TextObject(TextObject&& other) = delete;
	TextObject& operator=(const TextObject& other) = delete;
	TextObject& operator=(TextObject&& other) = delete;
private:
	//bool m_NeedsUpdate;
	//std::string m_Text;
	//std::shared_ptr<Font> m_Font;
	//std::shared_ptr<Texture2D> m_Texture;
};