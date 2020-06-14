#pragma once
class Observer;

class Subject
{
public:
	void AddObserver(Observer *observer);
	void RemoveObserver(Observer *observer);

	void Notify(void* pData, int event);
private:
	static const int MAX_OBSERVERS{ 10 };
	
	Observer* m_Observers[MAX_OBSERVERS]{};
	int m_NumObservers{};
};

