#include "MiniginPCH.h"
#include "StartMenu.h"
#include "TextComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"

void StartMenu::Awake()
{
	GameObject* input{ new GameObject{} };

	m_pInput = new InputComponent();
	m_pInput->RegisterCommand(ControllerButton::ButtonY, new StartSingle());
	m_pInput->RegisterCommand(ControllerButton::ButtonA, new StartCoop());
	m_pInput->RegisterCommand(ControllerButton::ButtonB, new StartVersus());

	input->AddComponent(m_pInput);

	GameObject* singleText{ new GameObject{} };
	GameObject* coopText{ new GameObject{} };
	GameObject* versusText{ new GameObject{} };
	singleText->AddComponent(new TextComponent{ "Lingua.otf", 30, "Press Y to play Solo!" });
	coopText->AddComponent(new TextComponent{ "Lingua.otf", 30, "Press A to play Coop!" });
	versusText->AddComponent(new TextComponent{ "Lingua.otf", 30, "Press B to play Versus!" });

	singleText->GetComponent<TransformComponent>()->SetPosition(0, 0, 0);
	coopText->GetComponent<TransformComponent>()->SetPosition(0, 50, 0);
	versusText->GetComponent<TransformComponent>()->SetPosition(0, 100, 0);

	SceneManager::GetInstance().GetActiveScene()->Add(input);
	SceneManager::GetInstance().GetActiveScene()->Add(singleText);
	SceneManager::GetInstance().GetActiveScene()->Add(coopText);
	SceneManager::GetInstance().GetActiveScene()->Add(versusText);
}

void StartMenu::Update()
{
	Command *pCommand = m_pInput->ProcessInput();
	if (pCommand)
		pCommand->Execute(nullptr);
}
