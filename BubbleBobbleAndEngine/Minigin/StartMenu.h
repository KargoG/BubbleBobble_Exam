#pragma once
#include <GameMode.h>
#include "InputComponent.h"
#include "LevelLoader.h"

class StartMenu :
    public GameMode
{
public:
	void Awake() override;
	void Update() override;

private:
	InputComponent* m_pInput{};
};

class StartSingle : public Command
{
public:
	void Execute(ControllerComponent*) override { LevelLoader::GetInstance().LoadLevel(1, BubbleBobbleGameMode::Single); }
};
class StartCoop : public Command
{
public:
	void Execute(ControllerComponent*) override { LevelLoader::GetInstance().LoadLevel(1, BubbleBobbleGameMode::Coop); }
};
class StartVersus : public Command
{
public:
	void Execute(ControllerComponent*) override { LevelLoader::GetInstance().LoadLevel(1, BubbleBobbleGameMode::Versus); }
};


