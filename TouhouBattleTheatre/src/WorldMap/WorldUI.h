#pragma once

#include "../TestApplication.h"
#include "UnitManager.h"
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

// 前向声明
class WorldUIItem;
class UnitManager;
class WorldMap;

// 单位选择状态机
enum class SelectionState
{
	NONE,
	SINGLE_SELECTED,
	MULTI_SELECTED
};

class WorldUI
{
public:
	WorldUI(WorldMap& worldMap, UnitManager& unitManager);

	~WorldUI();

	void Update(const float delta);
	void Render();

	void AddUIRoot(std::shared_ptr<WorldUIItem> item);
	void RemoveUIRoot(const std::shared_ptr<WorldUIItem>& item);

	void UIMapClick();

	Cursor& GetCursor();
	SelectionState GetSelectionState() const { return _selectionState; }

	void HandleInput(const Uint8* KeyStates);

	bool ExitFlag = false;

private:
	void _handleClick(SDL_MouseButtonEvent& mouse_event);
	void SelectUnit(int tileX, int tileY);


	/**
	 * UI类管理指针、摄像机和UI节点树，引用世界地图（主场景）和单位管理器
	 * 渲染体系优化完后可能会挪走摄像机控制器
	 */
	WorldMap& _worldMap;
	UnitManager& _unitManager;

	Cursor _cursor;
	OrthoCameraController _cameraController;
	std::shared_ptr<WorldUIItem> _rootUIItem;
	std::vector<std::shared_ptr<WorldUIItem>> _uiRoots; // UI树的各个根节点

	float _cursorPosX = 0.0f;
	float _cursorPosY = 0.0f;
	int _cursorTileX = 0;
	int _cursorTileY = 0;
	bool _cursorMovedInLastFrame = false;
	bool _shiftPress = false;

	SelectionState _selectionState = SelectionState::NONE;
	std::vector<UnitID> _selectedUnits;
};