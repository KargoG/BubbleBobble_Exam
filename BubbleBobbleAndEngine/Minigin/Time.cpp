#include "MiniginPCH.h"
#include "Time.h"

float Time::GetPhysicsDeltaTime() const
{
	return m_DeltaTime;
}

float Time::GetDeltaTime() const
{
	return m_DeltaTime;
}

float Time::GetTotalTime()
{
	return m_SummedTime;
}

float Time::GetFPS()
{
	return m_SummedFrames / m_SummedTime;
}

void Time::Update( float deltaTime )
{
	m_DeltaTime = deltaTime;
	m_SummedTime += deltaTime;
	m_SummedFrames++;
}
