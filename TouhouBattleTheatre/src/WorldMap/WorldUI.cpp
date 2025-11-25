#include "WorldUI.h"
#include "WorldMap.h"
#include <spdlog/spdlog.h>
#include <algorithm>



WorldUI::WorldUI(WorldMap& worldMap, UnitManager& unitManager)
	: _cameraController(TestApplication::GetInstance().GetScreenWidth() * 0.1,
		TestApplication::GetInstance().GetScreenHeight() * 0.1,
		glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f),
	_cursor(*this), _worldMap(worldMap), _unitManager(unitManager)
{
	_rootUIItem = std::make_shared<WorldUIItem>(glm::vec2(0.0f, 0.0f),
		glm::vec2(TestApplication::GetInstance().GetScreenWidth(),
			TestApplication::GetInstance().GetScreenHeight()));
	_rootUIItem->OnLeftClick = [this]() {
		this->UIMapClick();
		};
	_rootUIItem->ConsumeClick = true;
	_rootUIItem->Name = "BaseUI";

	AddUIRoot(_rootUIItem);
}

WorldUI::~WorldUI()
{}

void WorldUI::Update(const float delta)
{
	_cameraController.Update(delta);

	CameraParams params;
	params.camera_x = _cameraController.GetPosition().x;
	params.camera_y = _cameraController.GetPosition().z; // 注意：世界坐标的 Y 值来自相机的 Z 值
	params.view_width = _cameraController.GetViewWidth();
	params.view_height = _cameraController.GetViewHeight();
	params.screen_width = TestApplication::GetInstance().GetScreenWidth();
	params.screen_height = TestApplication::GetInstance().GetScreenHeight();

	float world_x = params.camera_x + (_cursorPosX / params.screen_width - 0.5f) * params.view_width;
	float world_y = params.camera_y + (0.5f - _cursorPosY / params.screen_height) * params.view_height;

	// 把世界坐标转换为图块坐标，确保非负数
	_cursorTileX = static_cast<int>(glm::max(0.0f, world_x / TILE_SIZE));
	_cursorTileY = static_cast<int>(glm::max(0.0f, world_y / TILE_SIZE));

	_cursor.UpdateCursorPos(_cursorTileX, _cursorTileY);

	_cursor.Update(delta);

	// 更新所有UI根节点及其子节点
	for (auto& root : _uiRoots)
	{
		root->Update(delta);
	}
}

void WorldUI::Render()
{
	_cursor.Render();
	for (auto& root : _uiRoots)
	{
		root->Render();
	}
}

void WorldUI::AddUIRoot(std::shared_ptr<WorldUIItem> item)
{
	_uiRoots.push_back(item);
}

void WorldUI::RemoveUIRoot(const std::shared_ptr<WorldUIItem>& item)
{
	_uiRoots.erase(std::remove(_uiRoots.begin(), _uiRoots.end(), item), _uiRoots.end());
}

void WorldUI::UIMapClick()
{
	spdlog::info("Cursor clicked at tile ({}, {})", _cursorTileX, _cursorTileY);
	SelectUnit(_cursorTileX, _cursorTileY);
}

void WorldUI::SelectUnit(int tileX, int tileY)
{
	auto units = _unitManager.GetUnitsAt(tileX, tileY);
	
	if (units.empty())
	{
		// 点击了空格子
		if (!_shiftPress)
		{
			_selectedUnits.clear();
			spdlog::debug("点击空格清除选区。");
		}
		else
		{
			spdlog::debug("尝试添加选择，无单位");
		}
	}
	else
	{
		// 点击了有单位的格子，简单起见只处理第一个单位
		UnitID clickedUnitId = units[0];
		const auto& unit = _unitManager.GetUnit(clickedUnitId);

		auto it = std::find(_selectedUnits.begin(), _selectedUnits.end(), clickedUnitId);

		if (it != _selectedUnits.end())
		{
			// 单位已被选中
			if (_shiftPress)
			{
				// 按住Shift点击，取消选中该单位
				_selectedUnits.erase(it);
				spdlog::debug("Shift单独取消选择单位 {} 。", unit.GetName());
			}
			else
			{
				// 未按住Shift点击，只选中该单位
				_selectedUnits.clear();
				_selectedUnits.push_back(clickedUnitId);
				spdlog::debug("在选中单位中单独选中单位 {} 。", unit.GetName());
			}
		}
		else
		{
			// 单位未被选中
			if (!_shiftPress)
			{
				spdlog::debug("未按住Shift，清除其他选择。");
				_selectedUnits.clear();
			}
			_selectedUnits.push_back(clickedUnitId);
			spdlog::debug("添加选择单位 {} 。", unit.GetName());
		}

		spdlog::debug("选中的单位列表为:");
		for (auto i : _selectedUnits)
		{
			spdlog::debug("单位ID: {}", i);
		}	
	}

	// 更新选择状态
	if (_selectedUnits.empty())
	{
		_selectionState = SelectionState::NONE;
		_cursor.SetState(CURSOR_DEFAULT);
	}
	else if (_selectedUnits.size() == 1)
	{
		_selectionState = SelectionState::SINGLE_SELECTED;
		_cursor.SetState(CURSOR_SELECT);
	}
	else
	{
		_selectionState = SelectionState::MULTI_SELECTED;
		_cursor.SetState(CURSOR_SELECT);
	}
}

Cursor& WorldUI::GetCursor()
{
	return _cursor;
}

void WorldUI::_handleClick(SDL_MouseButtonEvent& mouse_event)
{
	// 从最上层的根节点开始遍历整棵UI树
	for (auto it = _uiRoots.rbegin(); it != _uiRoots.rend(); ++it)
	{
		//spdlog::debug("Checking click for UI element: {}", (*it)->Name);
		if ((*it)->ConsumeClick)
		{
			//spdlog::debug("UI element {} consumed the click event.", (*it)->Name);
			(*it)->HandleClick(mouse_event.x, mouse_event.y, mouse_event.button == SDL_BUTTON_LEFT ? MouseButton::LEFT : 
														   ( mouse_event.button == SDL_BUTTON_RIGHT ? MouseButton::RIGHT : MouseButton::MIDDLE) );
			return; // 事件已被某个UI元素处理
		}
	}
}


void WorldUI::HandleInput(const Uint8* KeyStates)
{
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
	float rotation = 0.0f;
	SDL_Event event;
	_cursorMovedInLastFrame = false;

	// Handle keyboard input
	if (KeyStates[SDL_SCANCODE_W])
		movement.y += 1.0f;
	if (KeyStates[SDL_SCANCODE_S])
		movement.y -= 1.0f;
	if (KeyStates[SDL_SCANCODE_A])
		movement.x -= 1.0f;
	if (KeyStates[SDL_SCANCODE_D])
		movement.x += 1.0f;
	if (KeyStates[SDL_SCANCODE_ESCAPE])
		ExitFlag = true;
	if (KeyStates[SDL_SCANCODE_LSHIFT])
		_shiftPress = true;
	else
		_shiftPress = false;


	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			ExitFlag = true;
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION)
		{
			_cursorPosX = event.motion.x;
			_cursorPosY = event.motion.y;
			_cursorMovedInLastFrame = true;
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			_handleClick(event.button);
		}
		if (event.type == SDL_EVENT_WINDOW_RESIZED) //测试用，运行时禁止动态调整窗口大小，因为实现可变UI太难了
		{
			TestApplication::GetInstance().SetScreenSize(event.window.data1, event.window.data2);
			_cameraController.SetViewPort(event.window.data1 * 0.1, event.window.data2 * 0.1);
			glViewport(0, 0, event.window.data1, event.window.data2);
		}
	}

	

	//handle cursor on edge
	const float screenWidth = TestApplication::GetInstance().GetScreenWidth();
	const float screenHeight = TestApplication::GetInstance().GetScreenHeight();
	const float edgeRegionSize = 0.05f; // 5%

	const float edgeLeft = screenWidth * edgeRegionSize;
	const float edgeRight = screenWidth * (1.0f - edgeRegionSize);
	const float edgeTop = screenHeight * edgeRegionSize;
	const float edgeBottom = screenHeight * (1.0f - edgeRegionSize);

	if (_cursorPosX < edgeLeft)
	{
		const float speedFactor = 1.0f - (_cursorPosX / edgeLeft);
		movement.x -= speedFactor;
	}
	else if (_cursorPosX > edgeRight)
	{
		const float speedFactor = (_cursorPosX - edgeRight) / (screenWidth - edgeRight);
		movement.x += speedFactor;
	}

	if (_cursorPosY < edgeTop)
	{
		const float speedFactor = 1.0f - (_cursorPosY / edgeTop);
		movement.y += speedFactor;
	}
	else if (_cursorPosY > edgeBottom)
	{
		const float speedFactor = (_cursorPosY - edgeBottom) / (screenHeight - edgeBottom);
		movement.y -= speedFactor;
	}

	// update camera
	if (movement.x != 0 || movement.y != 0 || movement.z != 0)
		_cameraController.Movement(movement);
}


