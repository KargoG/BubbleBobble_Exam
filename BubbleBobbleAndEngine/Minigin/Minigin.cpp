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

using namespace std;
using namespace std::chrono;

void Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	GameData::GetInstance().Init(640, 480);

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
	//BaseComponent::RegisterComponent("AIControllerComponent", new AIControllerComponent{});

	// Load Levels
	LevelLoader::GetInstance().Init();
	LevelLoader::GetInstance().LoadLevel(4);
	
	//auto scene = SceneManager::GetInstance().CreateScene("Demo");

	//auto go{ new GameObject() };
	//go->AddComponent(new TextureRendererComponent());
	//go->GetComponent<TextureRendererComponent>()->SetTexture("background.jpg");
	//scene->Add(go);

	//go = new GameObject();
	//go->AddComponent(new TransformComponent(216.f, 180.f));
	//go->AddComponent(new TextureRendererComponent());
	//go->GetComponent<TextureRendererComponent>()->SetTexture("logo.png");
	//scene->Add(go);


	//go = new GameObject();
	//go->AddComponent(new TransformComponent(80.f, 20.f));
	//
	//auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	//auto tc = new TextComponent(font, "Programming 4 Assignment");
	//go->AddComponent(tc);
	//scene->Add(go);

	//auto go = new GameObject();
	//go->AddComponent(new TransformComponent(0.f, 0.f));
	////auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	//auto tc = new TextComponent("Lingua.otf", 15, "00 FPS");
	//go->AddComponent(tc);
	//go->AddComponent(new FPS());
	//scene->Add(go);

	GameObject *player{ new Player{} };
	GameObject *enemy{ new Enemy{} };

	player->GetComponent<TransformComponent>()->SetPosition(20, 15, 0);
	enemy->GetComponent<TransformComponent>()->SetPosition(220, 15, 0);
	enemy->GetComponent<PlayerControllerComponent>()->SetPlayerNumber(1);
	
	SceneManager::GetInstance().GetActiveScene()->Add(player);
	SceneManager::GetInstance().GetActiveScene()->Add(enemy);
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
