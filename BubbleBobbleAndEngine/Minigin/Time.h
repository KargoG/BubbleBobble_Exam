#pragma once
#include "Singleton.h"
class Time :
	public Singleton<Time>
{
public:
	float GetPhysicsDeltaTime() const;
	float GetDeltaTime() const;

	float GetTotalTime();
	float GetFPS();
	
	void Update(float deltaTime);
private:
	const float m_PhysicsDeltaTime{ 1.f / 60.f };
	float m_DeltaTime{};
	float m_SummedTime{};
	float m_SummedFrames{};
};

