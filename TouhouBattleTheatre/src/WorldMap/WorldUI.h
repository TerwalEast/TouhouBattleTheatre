#pragma once
#include "WorldUIItem.h"
#include "../Camera/OrthoCameraController.h"
#include "Cursor.h"
#include <vector>
#include <memory>

class WorldUI
{
public:
	WorldUI();
	~WorldUI();

	void HandleInput(Uint8* KeyStates);
	void Update(const float delta);
	void Render();

	void RegisterUIItem(std::shared_ptr<WorldUIItem> item);
	void UnregisterUIItem(const std::shared_ptr<WorldUIItem>& item);

	bool ExitFlag = false;
private:
	void _handleClick(SDL_Event mouse_event);
	void _updateUIItems(const float delta);

	std::vector<std::weak_ptr<WorldUIItem>> _uiItems;
	OrthoCameraController _cameraController;

	float _cursorPosX = 0.0f;
	float _cursorPosY = 0.0f;
	bool _cursorMovedInLastFrame = false;

	Cursor _cursor;
};