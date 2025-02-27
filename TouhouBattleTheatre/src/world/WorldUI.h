#pragma once

#include "../../TouhouBattleTheatre.h"

#include "../Stage.h"

#include <SDL3/SDL_events.h>


enum UIState
{
	hot, active, inactive
};

class WorldUI 
{
public:
	WorldUI();
	~WorldUI();
	void HandleClick(SDL_Event mouse_event);
private:

};