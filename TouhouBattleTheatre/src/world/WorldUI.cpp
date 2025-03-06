#include "WorldUI.h"
#include "../TestApplication.h"

WorldUI::WorldUI() : _cameraController(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1, glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f)
{}

WorldUI::~WorldUI()
{}

void WorldUI::_handleClick(SDL_Event mouse_event)
{
	if (mouse_event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (mouse_event.button.button == SDL_BUTTON_LEFT)
		{

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

void WorldUI::_handleHover()
{
	for (WorldUIItem item : _uiItems)
	{
		if (item._consumeHover)
		{
			if (item.isWithinUIElement(_cursorPosX, _cursorPosY))
			{

			}
		}
		else
		{
			// Skip this item
			continue;
		}
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
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			this->_handleClick(event);
		}
	}

	if (!_cursorMovedInLastFrame) 
	{
		this->_handleHover();
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
	//     if (KeyStates[SDL_SCANCODE_SPACE])
			 //movement.z += 1.0f;
	//     if (KeyStates[SDL_SCANCODE_LCTRL])
			 //movement.z -= 1.0f;
	if (KeyStates[SDL_SCANCODE_Q])
		rotation -= 1.0f;
	if (KeyStates[SDL_SCANCODE_E])
		rotation += 1.0f;
	if (KeyStates[SDL_SCANCODE_ESCAPE])
		ExitFlag = true;

	// update camera
	if (movement.x != 0 || movement.y != 0 || movement.z != 0)
		_cameraController.Movement(movement);
	//if (rotation != 0.0f)
		//cameraController.ArcballRotate(rotation);
}

void WorldUI::Update(const float delta)
{
	_cameraController.Update(delta);
	_cursor.UpdateCursorPos(_cursorPosX, _cursorPosY, _cameraController.GetPosition().x,
		_cameraController.GetPosition().z, _cameraController.GetZoom(),
		TestApplication::GetInstance().GetScreenWidth(), TestApplication::GetInstance().GetScreenHeight(),
		_cameraController.GetViewWidth(), _cameraController.GetViewHeight());
}

void WorldUI::Render()
{
	_cursor.Render();

}
