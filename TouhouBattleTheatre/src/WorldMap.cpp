#include "WorldMap.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include <stb_image.h>


WorldMap::WorldMap() : _cameraController(WINDOW_WIDTH * 0.1, WINDOW_HEIGHT * 0.1, glm::vec3(0.0f, -100.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 1.0f)
{
}

WorldMap::~WorldMap()
{

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
}

void WorldMap::Update(const float delta)
{
	//Update Background
	_background.Update(delta);
	//Update Tilemap

	//Update Actors

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
			TestApplication::Unload();
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION)
		{

			//cameraController.Rotate(event.motion.xrel, event.motion.yrel);
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
		goto END;

}



WorldMapBackGround::WorldMapBackGround()
{
	_backgroundTexture = Texture_Load("res/texture/background.jpg");
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

cursor::cursor()
{
	_cursorTexture = Texture_Load("res/cursor/cursor.png");
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

cursor::~cursor()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
	glDeleteTextures(1, &_cursorTexture);
}

void cursor::Render()
{
	glUseProgram(ShaderManager::GetInstance().ShaderMap().at("cursor").id);
	glBindVertexArray(_vao);
	glBindTextureUnit(0, _cursorTexture);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

void cursor::Update(const float delta)
{
	float x, y;
	SDL_GetMouseState(&x, &y);
	//_updateCursorPos(x, y);
}

void cursor::HandleInput()
{
}

void cursor::_updateCursorPos(const float mouse_x, const float mouse_y, const float camera_x, const float camera_y, const float zoom, const float screen_width, const float screen_height, const float view_width, const float view_height)
{
	spdlog::info("Mouse Pos: {} {}", mouse_x, mouse_y);
	spdlog::info("Camera Pos: {} {}", camera_x, camera_y);
	spdlog::info("Screen Size: {} {}", screen_width, screen_height);
	spdlog::info("View Size: {} {}", view_width, view_height);
	spdlog::info("Zoom: {}", zoom);
}



