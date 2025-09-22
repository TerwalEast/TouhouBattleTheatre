#pragma once

#include "../../TouhouBattleTheatre.h"

#include "../Stage.h"

#include "Cursor.h"
#include "WorldUIItem.h"
#include "../camera/OrthoCameraController.h"

#include <SDL3/SDL_events.h>




class WorldUI 
{
public:
	WorldUI();
	~WorldUI();
	
	void HandleInput(Uint8* KeyStates);
	void Update(const float delta);
	void Render();
	bool ExitFlag = false;

private:
	Cursor _cursor;
	OrthoCameraController _cameraController;

	float _cursorPosX = 0, _cursorPosY = 0;
	bool _cursorMovedInLastFrame = false;
	std::vector<WorldUIItem> _uiItems; //后创建的UIItem优先级更高

	void _handleClick(SDL_Event mouse_event);
	void _handleHover();
	void _updateUIItems(const float delta);

	

	
};