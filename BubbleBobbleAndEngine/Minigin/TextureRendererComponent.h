#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"

class TextureRendererComponent :
	public BaseComponent
{
public:
	virtual void Start() override;
	virtual void Update() override;
	virtual void PhysicsUpdate() override;
	virtual void Render() const override;
	
	void SetTexture( const char * filename );
private:

	std::shared_ptr<Texture2D> m_pTexture;
};

