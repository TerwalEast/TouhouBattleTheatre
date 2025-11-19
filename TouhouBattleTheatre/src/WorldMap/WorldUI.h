#pragma once
#include "WorldUIItem.h"
#include "../Camera/OrthoCameraController.h"
#include "Cursor.h"
#include <vector>
#include <memory>

namespace {
	constexpr float TILE_SIZE = 8.0f;
	constexpr float VERTICES[] = {
		// Positions        // Texture Coords
		0.0f,      0.0f, 0.0f,       0.0f, 0.0f,
		TILE_SIZE, 0.0f, 0.0f,       1.0f, 0.0f,
		TILE_SIZE, 0.0f, TILE_SIZE,  1.0f, 1.0f,
		0.0f,      0.0f, TILE_SIZE,  0.0f, 1.0f
	};
	constexpr size_t VERTEX_COUNT = 4;
	constexpr size_t COMPONENTS_PER_VERTEX = 5;
	constexpr size_t VERTEX_STRIDE = COMPONENTS_PER_VERTEX * sizeof(float);
}


class WorldMap;

class WorldUI
{
public:
	WorldUI(WorldMap& worldMap) : _cameraController(TestApplication::GetInstance().GetScreenWidth() * 0.1,
		TestApplication::GetInstance().GetScreenHeight() * 0.1,
		glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f),
		_cursor(*this), _worldMap(worldMap) {};

	~WorldUI();

	void HandleInput(const Uint8* KeyStates);
	void Update(const float delta);
	void Render();

	void AddUIRoot(std::shared_ptr<WorldUIItem> item);
	void RemoveUIRoot(const std::shared_ptr<WorldUIItem>& item);

	Cursor& GetCursor();

	bool ExitFlag = false;

private:
	void _handleClick(SDL_MouseButtonEvent& mouse_event);
	
	std::vector<std::shared_ptr<WorldUIItem>> _uiRoots; // 只存储根节点

	OrthoCameraController _cameraController;

	float _cursorPosX = 0.0f;
	float _cursorPosY = 0.0f;
	bool _cursorMovedInLastFrame = false;

	Cursor _cursor;
	WorldMap& _worldMap;
};