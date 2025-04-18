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
	static int run();
	static TestApplication& GetInstance();
	std::string BasePath;
	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }
	void SetScreenSize(int width, int height) {
		screen_width = width;
		screen_height = height;
	}
private:
	TestApplication();
	~TestApplication() {};
	TestApplication(TestApplication const&) = delete;
	void operator=(TestApplication const&) = delete;
	bool quiting = false;
	int screen_width = WINDOW_WIDTH;
	int screen_height = WINDOW_HEIGHT;
};
