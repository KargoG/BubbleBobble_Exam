#pragma once
#include <GameObject.h>

class AIControllerComponent;

class Enemy :
    public GameObject
{
public:
	Enemy();

private:
	AIControllerComponent* m_pAIController{ nullptr };
};

