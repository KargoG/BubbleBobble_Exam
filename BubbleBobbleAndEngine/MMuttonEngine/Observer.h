#pragma once

class Player;
enum class Event;

class Observer abstract
{
public:
	virtual ~Observer(){};
	virtual void OnNotify(void* pData, int events) = 0;
};

