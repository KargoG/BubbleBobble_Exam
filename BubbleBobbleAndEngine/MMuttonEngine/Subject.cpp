#include "pch.h"
#include "Subject.h"
#include "Observer.h"

void Subject::AddObserver( Observer *observer )
{
	if (m_NumObservers < MAX_OBSERVERS)
		m_Observers[m_NumObservers++] = observer;
	else
		throw std::exception(std::string{ "A subject can only have a max of " + std::to_string(MAX_OBSERVERS) + " Observers at a time!\n" }.c_str());
}

void Subject::RemoveObserver( Observer *observer )
{
	if (m_Observers[m_NumObservers - 1] == observer)
		m_Observers[--m_NumObservers] = nullptr;
}

void Subject::Notify(void *pData, int event)
{
	for( int i = 0; i < m_NumObservers; ++i )
	{
		if(m_Observers[i])
			m_Observers[i]->OnNotify(pData, event);
	}
}
