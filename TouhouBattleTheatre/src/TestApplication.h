#pragma	once
#pragma execution_character_set("utf-8")

#include "../TouhouBattleTheatre.h"
#include "Terrain.h"
#include <glm/glm.hpp>

#include <SDL3/SDL.h>

#define WINDOW_WIDTH 2400
#define WINDOW_HEIGHT 1100

class TestApplication {
public:
	static TestApplication& GetInstance();

	TestApplication(TestApplication const&) = delete;
	void operator=(TestApplication const&) = delete;

	int run();

	std::string BasePath;
	int GetScreenWidth() const { return _screenWidth; }
	int GetScreenHeight() const { return _screenHeight; }
	void SetScreenSize(int width, int height) {
		_screenWidth = width;
		_screenHeight = height;
	}

private:
	TestApplication();
	~TestApplication();

	bool Init();
	void Cleanup();

	bool _quiting = false;
	int _screenWidth = WINDOW_WIDTH;
	int _screenHeight = WINDOW_HEIGHT;

	SDL_Window* _window = nullptr;
};
