#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "Time.h"
#include "LevelLoader.h"
#include "GameObject.h"
#include "TextureRendererComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "FPS.h"
#include "Scene.h"

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
		640,
		480,
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

	//go = new GameObject();
	//go->AddComponent(new TransformComponent(0.f, 0.f));
	//font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 15);
	//tc = new TextComponent(font, "00 FPS");
	//go->AddComponent(tc);
	//go->AddComponent(new FPS());
	//scene->Add(go);
	
	LevelLoader loader{};
	loader.LoadLevel("Level01.txt");
}

void Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
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
		auto& input = InputManager::GetInstance();

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

			doContinue = input.ProcessInput();
			
			sceneManager.Update();
			renderer.Render();
			
			lastTime = currentTime;
		}
	}

	Cleanup();
}
