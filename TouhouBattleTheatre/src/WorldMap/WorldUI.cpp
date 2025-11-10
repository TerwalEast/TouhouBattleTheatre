#include "WorldUI.h"
#include "../TestApplication.h"
#include <spdlog/spdlog.h>

WorldUI::WorldUI() : _cameraController(TestApplication::GetInstance().GetScreenWidth() * 0.1,
										TestApplication::GetInstance().GetScreenHeight() * 0.1,
										glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f)
{
	auto testButton = std::make_unique<WorldUIItem>(glm::vec2(100, 100), glm::vec2(200, 50));
	testButton->OnClick = []() {
		spdlog::info("Button clicked!");
	};
	testButton->ConsumeClick = true;
	_uiItems.push_back(std::move(testButton));
}

WorldUI::~WorldUI()
{}

void WorldUI::_handleClick(SDL_Event mouse_event)
{
	if (mouse_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (mouse_event.button.button == SDL_BUTTON_LEFT)
		{
			for (auto it = _uiItems.rbegin(); it != _uiItems.rend(); ++it)
			{
				auto& item = *it;
				if (item->isWithinUIElement(_cursorPosX, _cursorPosY))
				{
					if (item->ConsumeClick)
					{
						item->Click(); // 触发点击事件
						break; // 事件被消耗，则停止传递
					}
				}
			}
		}
		else if (mouse_event.button.button == SDL_BUTTON_RIGHT)
		{
			// Handle right click
		}
	}
	else if (mouse_event.type == SDL_EVENT_MOUSE_BUTTON_UP)
	{
		if (mouse_event.button.button == SDL_BUTTON_LEFT)
		{
			// Handle left click release
		}
		else if (mouse_event.button.button == SDL_BUTTON_RIGHT)
		{
			// Handle right click release
		}
	}
}

void WorldUI::_updateUIItems(const float delta)
{
	for (auto& item : _uiItems)
	{
		item->Update(delta);
	}
}

void WorldUI::HandleInput(Uint8* KeyStates)
{
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
	float rotation = 0.0f;
	SDL_Event event;
	_cursorMovedInLastFrame = false;
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
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP)
		{
			_handleClick(event);
		}
		if (event.type == SDL_EVENT_WINDOW_RESIZED) //测试用，运行时禁止动态调整窗口大小
		{
			TestApplication::GetInstance().SetScreenSize(event.window.data1, event.window.data2);
			_cameraController.SetViewPort(event.window.data1 * 0.1, event.window.data2 * 0.1);
			glViewport(0, 0, event.window.data1, event.window.data2);
		}
	}

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

	//handle cursor on edge
	const float screenWidth = TestApplication::GetInstance().GetScreenWidth();
	const float screenHeight = TestApplication::GetInstance().GetScreenHeight();
	const float edgeRegionSize = 0.1f; // 10% of the screen dimension

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

void WorldUI::Update(const float delta)
{
	_cameraController.Update(delta);
	_cursor.UpdateCursorPos(_cursorPosX, _cursorPosY, _cameraController.GetPosition().x,
		_cameraController.GetPosition().z, _cameraController.GetZoom(),
		TestApplication::GetInstance().GetScreenWidth(), TestApplication::GetInstance().GetScreenHeight(),
		_cameraController.GetViewWidth(), _cameraController.GetViewHeight());
	this->_updateUIItems(delta);
}

void WorldUI::Render()
{
	for (auto& item : _uiItems)
	{
		item->Render();
	}
	_cursor.Render();
}
