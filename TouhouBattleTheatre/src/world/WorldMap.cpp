#include "WorldMap.h"
#include "../ShaderManager.h"
#include "../TextureManager.h"

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
			TestApplication::GetInstance().Quit();
		}
		if (event.type == SDL_EVENT_MOUSE_MOTION)
		{
			_cursor.UpdateCursorPos(event.motion.x, event.motion.y, _cameraController.GetPosition().x, 
								_cameraController.GetPosition().z, _cameraController.GetZoom(), 
				TestApplication::GetInstance().GetScreenWidth(), TestApplication::GetInstance().GetScreenHeight(), 
				_cameraController.GetViewWidth(), _cameraController.GetViewHeight());
			
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
		TestApplication::GetInstance().Quit();

	    // update camera
	if (movement.x != 0 || movement.y != 0 || movement.z != 0)
		_cameraController.Movement(movement);
	//if (rotation != 0.0f)
	    //cameraController.ArcballRotate(rotation);
	

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
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		8.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		8.0f, 0.0f, 8.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 8.0f, 0.0f, 1.0f
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
	glUseProgram(ShaderManager::GetInstance().ShaderMap().at("actor").id);
	glBindVertexArray(_vao);
	glBindTextureUnit(0, _cursorTexture);

	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(_cursorTileX * 8.0f, 0.0f, _cursorTileY * 8.0f));

	ShaderManager::GetInstance().SetUniform("actor", "model", UniformType::MAT4, (void*)&model);

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

void cursor::UpdateCursorPos(const float mouse_x, const float mouse_y, const float camera_x, const float camera_y, const float zoom, const float screen_width, const float screen_height, const float view_width, const float view_height)
{
	//spdlog::info("Mouse Pos: {} {}", mouse_x, mouse_y);
	//spdlog::info("Camera Pos: {} {}", camera_x, camera_y);
	//spdlog::info("Screen Size: {} {}", screen_width, screen_height);
	//spdlog::info("View Size: {} {}", view_width, view_height);
	//spdlog::info("Zoom: {}", zoom);

	//map -0.5 to 0.5
	float normalized_x = mouse_x / screen_width - 0.5f;
	float normalized_y = 0.5f - mouse_y / screen_height;

	//map to view size
	float offset_x = normalized_x * view_width;
	float offset_y = normalized_y * view_height;

	//map to world coord
	float world_x = camera_x + offset_x;
	float world_y = camera_y + offset_y;

	//spdlog::info("World Pos: {} {}", world_x, world_y);

	//map to tile
	int tile_x = world_x >= 0 ? (int)world_x / 8 : 0;
	int tile_y = world_y >= 0 ? (int)world_y / 8 : 0;

	spdlog::info("Tile Pos: {} {}", tile_x, tile_y);

	_cursorTileX = tile_x;
	_cursorTileY = tile_y;

}



