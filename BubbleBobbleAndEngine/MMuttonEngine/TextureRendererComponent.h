#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"

class TextureRendererComponent :
	public BaseComponent
{
public:
	virtual ~TextureRendererComponent();
	
	virtual void Render() const override;
	
	void SetTexture( const char * filename );
	void SetRenderDimensions( float xPos, float yPos, float width, float height ) const;

	void SetSrcX(float x) const { m_pTexture->SetX( x ); };
	void SetSrcY(float y) const { m_pTexture->SetY( y ); };
	void SetSrcWidth(float width) const { m_pTexture->SetWidth( width ); };
	void SetSrcHeight(float height) const { m_pTexture->SetHeight( height ); }
	
	BaseComponent * Clone() const override;
	void LoadFromJson(const nlohmann::json& json) override;
	
private:

	std::string m_TextureName{};
	Texture2D *m_pTexture{ nullptr };
};

