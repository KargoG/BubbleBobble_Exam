#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include <InputComponent.h>
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "Time.h"
#include "LevelLoader.h"
#include "FPS.h"
#include "GameData.h"
#include "PlayerControllerComponent.h"
#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "TransformComponent.h"
#include "BulletBehaviourComponent.h"
#include "AIControllerComponent.h"
#include "BubbleBobbleAnimationComponent.h"
#include "TextComponent.h"

using namespace std;
using namespace std::chrono;

void Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		GameData::GetInstance().GetWindowWidth(),
		GameData::GetInstance().GetWindowHeight(),
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

class StartSingle : public Command
{
public:
	void Execute( ControllerComponent * ) override{ LevelLoader::GetInstance().LoadLevel(0, BubbleBobbleGameMode::Single); }
};
class StartCoop : public Command
{
public:
	void Execute( ControllerComponent * ) override{ LevelLoader::GetInstance().LoadLevel(0, BubbleBobbleGameMode::Coop); }
};
class StartVersus : public Command
{
public:
	void Execute( ControllerComponent * ) override{ LevelLoader::GetInstance().LoadLevel(0, BubbleBobbleGameMode::Versus); }
};

/**
 * Code constructing the scene world starts here
 */
void Minigin::LoadGame() const
{

	// Register Components
	BaseComponent::RegisterEngineComponents();
	BaseComponent::RegisterComponent("FPS", new FPS{});
	BaseComponent::RegisterComponent("PlayerControllerComponent", new PlayerControllerComponent{});
	BaseComponent::RegisterComponent("BulletBehaviourComponent", new BulletBehaviourComponent{});
	BaseComponent::RegisterComponent("AIControllerComponent", new AIControllerComponent{});
	BaseComponent::RegisterComponent("BubbleBobbleAnimationComponent", new BubbleBobbleAnimationComponent{ nullptr });

	// Load Levels
	LevelLoader::GetInstance().Init();
	


	Scene *startScene = SceneManager::GetInstance().CreateScene("MainMenuScene");
	GameObject* input{ new GameObject{} };

	InputComponent* ic{ new InputComponent() };
	ic->RegisterCommand(ControllerButton::ButtonY, new StartSingle());
	ic->RegisterCommand(ControllerButton::ButtonA, new StartCoop());
	ic->RegisterCommand(ControllerButton::ButtonB, new StartVersus());

	input->AddComponent(ic);
	
	GameObject* singleText{ new GameObject{} };
	GameObject* coopText{ new GameObject{} };
	GameObject* versusText{ new GameObject{} };
	singleText->AddComponent(new TextComponent{ "Lingua.otf", 30, "Press Y to play Solo!" });
	coopText->AddComponent(new TextComponent{ "Lingua.otf", 30, "Press A to play Coop!" });
	versusText->AddComponent(new TextComponent{ "Lingua.otf", 30, "Press B to play Versus!" });

	singleText->GetComponent<TransformComponent>()->SetPosition(0, 0, 0);
	coopText->GetComponent<TransformComponent>()->SetPosition(0, 50, 0);
	versusText->GetComponent<TransformComponent>()->SetPosition(0, 100, 0);

	startScene->Add(input);
	startScene->Add(singleText);
	startScene->Add(coopText);
	startScene->Add(versusText);

	//SceneManager::GetInstance().SetActiveScene("MainMenuScene");

	
	GameObject *gameOverText{ new GameObject{} };

	TextComponent* text{ new TextComponent{"Lingua.otf", 30, "GameOver"} };
	gameOverText->AddComponent(text);

	SceneManager::GetInstance().CreateScene("GameOverScene")->Add(gameOverText);
	
	GameObject *player1WinOverText{ new GameObject{} };
	
	text = new TextComponent{"Lingua.otf", 30, "Player One Wins"};
	player1WinOverText->AddComponent(text);

	SceneManager::GetInstance().CreateScene("PlayerOneWinScene")->Add(player1WinOverText);

	GameObject *player2WinOverText{ new GameObject{} };

	text = new TextComponent{"Lingua.otf", 30, "Player 2 Wins"};
	player2WinOverText->AddComponent(text);

	SceneManager::GetInstance().CreateScene("PlayerTwoWinScene")->Add(player2WinOverText);

	LevelLoader::GetInstance().LoadLevel(5, BubbleBobbleGameMode::Single);
}

void Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	ResourceManager::GetInstance().CleanUp();
	BaseComponent::CleanUp();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	SceneManager::GetInstance().Awake();
	SceneManager::GetInstance().Start();
	
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();

		bool doContinue = true;

		auto lastTime = high_resolution_clock::now();
		float accumulatedPhysicsTime{};

		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();

			float deltaTime = chrono::duration<float>(currentTime - lastTime).count();
			
			accumulatedPhysicsTime += deltaTime;
			Time::GetInstance().Update(deltaTime);
			
			while(accumulatedPhysicsTime >= Time::GetInstance().GetPhysicsDeltaTime())
			{
				sceneManager.PhysicsUpdate();
				accumulatedPhysicsTime -= Time::GetInstance().GetPhysicsDeltaTime();
			}

			doContinue = !Minigin::CloseWindow();
			
			sceneManager.Update();
			sceneManager.Swap();
			renderer.Render();
			
			lastTime = currentTime;
		}
	}

	Cleanup();
}

bool Minigin::CloseWindow()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return true;
		}
	}

	return false;
}
