#include "MiniginPCH.h"
#include "GameObject.h"
#include "FPS.h"
#include "TextComponent.h"
#include "Time.h"
#include <string>

void FPS::Start()
{
	if (m_pGameObject)
	{
		m_pText = m_pGameObject->GetComponent<TextComponent>();
	}
}

void FPS::Update()
{
	if(m_pText)
	{
		m_pText->SetText( std::to_string(int(Time::GetInstance().GetFPS())) + " FPS" );
	}
}

void FPS::PhysicsUpdate()
{
}

void FPS::Render() const
{
}
