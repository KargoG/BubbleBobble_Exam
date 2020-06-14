#pragma once
#include <GameMode.h>
#include "Observer.h"

class VersusMode :
    public GameMode, public Observer
{
public:
	VersusMode(int level) : m_Level{level}{};
	void Start() override;
	
	void OnNotify(void* pData, int events) override;

private:
	int m_Level;

	void StartNextLevel();
	void ShowWinner(int winner); // TODO
};

