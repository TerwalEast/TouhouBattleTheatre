#include "WorldMap.h"
#include "../ShaderManager.h"
#include "../TextureManager.h"

#include <stb_image.h>
#include <spdlog/spdlog.h>


WorldMap::WorldMap() : _cameraController(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1, glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f)
{

}

WorldMap::~WorldMap()
{

}

void WorldMap::Start(SDL_Window* window, Uint8* KeyStates)
{
	//------------------------------------------------
	// Main Loop
	//------------------------------------------------

	int lastFrame = SDL_GetTicks();
	int thisFrame = lastFrame;
	float deltaTime;

	StageState currentState = command;

	while (1)
	{
		//------------------------------------------------
		// Quit
		//------------------------------------------------
		if (_quit)
		{
			break;
		}// Quit End

		if (_state == sleep)
		{
			spdlog::error("Invoking a sleeping stage, something is wrong!");
			break;
		}
		//------------------------------------------------
		// Handle Input
		//------------------------------------------------
		{
			this->HandleInput(KeyStates);
		}// Input End

		//------------------------------------------------
		// Update
		//------------------------------------------------
		{
			// Get Delta time
			thisFrame = SDL_GetTicks();
			deltaTime = (thisFrame - lastFrame) / 1000.0f;
			lastFrame = thisFrame;

			// Update Stage
			this->Update(deltaTime);
		}// Update End


		//------------------------------------------------
		// Render
		//------------------------------------------------
		{
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//mapTool.Render();
			this->Render();
			SDL_Delay(1);
			SDL_GL_SwapWindow(window);
		}// Render End

		

	}// Main Loop End
}

void WorldMap::Render()
{
	//Render background
	_background.Render();
	//Render tilemap
	_tileMap.Render();
	//Render Actors
	
	//Render Effects
	
	//Render UI
	_cursor.Render();
}

void WorldMap::Update(const float delta)
{
	//Update Background
	_background.Update(delta);
	//Update Tilemap

	//Update Actors

	//Update Camera
	_cameraController.Update(delta);

	//Update UI
	_cursor.UpdateCursorPos(_cursorPosX, _cursorPosY, _cameraController.GetPosition().x,
		_cameraController.GetPosition().z, _cameraController.GetZoom(),
		TestApplication::GetInstance().GetScreenWidth(), TestApplication::GetInstance().GetScreenHeight(),
		_cameraController.GetViewWidth(), _cameraController.GetViewHeight());
}

void WorldMap::HandleInput(Uint8* KeyStates)
{
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
	float rotation = 0.0f;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			_quit = true;
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION)
		{
			_cursorPosX = event.motion.x;
			_cursorPosY = event.motion.y;
		}
		if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
		{
			_click = true;
		}


	}
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
		_quit = true;

	    // update camera
	if (movement.x != 0 || movement.y != 0 || movement.z != 0)
		_cameraController.Movement(movement);
	//if (rotation != 0.0f)
	    //cameraController.ArcballRotate(rotation);
	

}



WorldMapBackGround::WorldMapBackGround()
{
	_backgroundTexture = Texture_Load(GET_TEXTURE_PATH(background.jpg));
	glCreateBuffers(1, &_vbo);

	float _vertices[20] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glNamedBufferStorage(_vbo, (sizeof(float) * 5)
		* 4, _vertices, GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glEnableVertexArrayAttrib(_vao, 0);
	glVertexArrayAttribBinding(_vao, 0, 0);
	glVertexArrayVertexBuffer(_vao, 0, _vbo, 0, 5 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(_vao, 1);
	glVertexArrayAttribBinding(_vao, 1, 0);
	glVertexArrayVertexBuffer(_vao, 1, _vbo, 0, 5 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));


}

WorldMapBackGround::~WorldMapBackGround()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
	glDeleteTextures(1, &_backgroundTexture);
}

void WorldMapBackGround::Render()
{
	glUseProgram(ShaderManager::GetInstance().ShaderMap().at("scrolling").id);
	glBindVertexArray(_vao);
	glBindTextureUnit(0, _backgroundTexture);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

void WorldMapBackGround::Update(const float delta)
{
	_scrollingTime += delta * 0.01;
	ShaderManager::GetInstance().SetUniform("scrolling", "time", UniformType::FLOAT, (void*)&_scrollingTime);
}





