#pragma once
struct SDL_Window;

class Minigin
{
public:
	void Initialize();
	void LoadGame() const;
	void Cleanup();
	void Run();

	bool CloseWindow();
private:
	SDL_Window* m_Window{};
};