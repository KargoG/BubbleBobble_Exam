#pragma once

class GameMode
{
public:
	virtual void Start(){};
	virtual void Update(){};
	virtual void PhysicsUpdate(){};
	virtual void Render(){};
	virtual void Swap(){};

	GameMode() = default;
	virtual ~GameMode() = default;
	GameMode(const GameMode& other) = delete;
	GameMode(GameMode&& other) = delete;
	GameMode& operator=(const GameMode& other) = delete;
	GameMode& operator=(GameMode&& other) = delete;
};

