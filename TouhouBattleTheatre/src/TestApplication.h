#pragma	once


#include "../TouhouBattleTheatre.h"
#include "Terrain.h"
#include <glm/glm.hpp>

#include <SDL3/SDL.h>


class TestApplication {
public:
	static int run();
	static TestApplication& getInstance();
	std::string BasePath;
private:
	TestApplication();
	~TestApplication() {};
	TestApplication(TestApplication const&) = delete;
	void operator=(TestApplication const&) = delete;
};
