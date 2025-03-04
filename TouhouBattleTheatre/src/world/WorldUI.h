#pragma once

#include "../../TouhouBattleTheatre.h"

#include "../Stage.h"

#include "Cursor.h"

#include <SDL3/SDL_events.h>
#include "../camera/OrthoCameraController.h"


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
	void HandleInput(Uint8* KeyStates);
	void Update(const float delta);
	void Render();
	bool ExitFlag = false;
private:
	Cursor _cursor;
	OrthoCameraController _cameraController;
	float _cursorPosX = 0, _cursorPosY = 0;
};