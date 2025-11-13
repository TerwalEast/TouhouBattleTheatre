#include "WorldUI.h"
#include "../TestApplication.h"
#include <spdlog/spdlog.h>
#include <algorithm>

WorldUI::WorldUI() : _cameraController(TestApplication::GetInstance().GetScreenWidth() * 0.1,
										TestApplication::GetInstance().GetScreenHeight() * 0.1,
										glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f)
{
	// 构造函数中的测试按钮现在也使用新机制创建
	auto testButton = std::make_shared<WorldUIItem>( glm::vec2(0.0f, 0.0f), 
		glm::vec2(	TestApplication::GetInstance().GetScreenWidth(), 
					TestApplication::GetInstance().GetScreenHeight()));
	testButton->OnClick = []() {
		spdlog::info("兜底测试按钮接收到点击！");

	};
	testButton->ConsumeClick = true;
	// 注意：这里直接调用注册函数，但为了演示，我们保留一个shared_ptr。
	// 在实际使用中，这个shared_ptr可能由另一个对象持有。
	// 为防止此处的testButton立即失效，我们需要一个地方持有它。
	// 在这个例子中，我们暂时不处理，因为重点是展示注册机制。
	// 更好的做法是在一个管理器中持有它。
	this->RegisterUIItem(testButton);
}

WorldUI::~WorldUI()
{}

void WorldUI::RegisterUIItem(std::shared_ptr<WorldUIItem> item)
{
	if (item)
	{
		// 添加到UI项列表
		_uiItems.push_back(item);
	}
}

void WorldUI::UnregisterUIItem(const std::shared_ptr<WorldUIItem>& item)
{
	std::erase_if(_uiItems, [&](const std::weak_ptr<WorldUIItem>& weak_item) {
		if (weak_item.expired()) {
			return true;
		}
		return weak_item.lock() == item;
	});
}


void WorldUI::_handleClick(SDL_Event mouse_event)
{
	if (mouse_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (mouse_event.button.button == SDL_BUTTON_LEFT)
		{
			// 从后往前遍历，因为后添加的UI在更上层
			for (auto it = _uiItems.rbegin(); it != _uiItems.rend(); ++it)
			{
				if (auto item = it->lock()) 
				{
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
		if (auto weak_item = item.lock())
		{
			weak_item->Update(delta);
		}
	}

	std::erase_if(_uiItems, [](const std::weak_ptr<WorldUIItem>& item) {
		return item.expired();
	});
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


void WorldUI::Render()
{
	for (auto& weak_item : _uiItems)
	{
		if (auto item = weak_item.lock()) 
		{
			item->Render();
		}
	}
	_cursor.Render();
}

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

	_cursor.UpdateCursorPos(_cursorPosX, _cursorPosY, params);

	this->_updateUIItems(delta);
}

