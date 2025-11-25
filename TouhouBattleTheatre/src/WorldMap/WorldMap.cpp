#include "WorldMap.h"
#include "../ShaderManager.h"
#include "../TextureLoader.h"

WorldMap::WorldMap()
	: _ui(*this, _unitManager)
{
	UnitID testUnit = _unitManager.AddUnit(0, 0);
	spdlog::debug("test unit 1 name: {}",_unitManager.GetUnit(testUnit).GetName());
	UnitID testUnit2 = _unitManager.AddUnit(1, 1);
	spdlog::debug("test unit 2 name: {}", _unitManager.GetUnit(testUnit).GetName());
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

		//------------------------------------------------
		// Quit
		//------------------------------------------------
		if (_ui.ExitFlag)
		{
			break;
		}// Quit End

	}// Main Loop End
}

void WorldMap::Render()
{
	//Render background
	_background.Render();
	//Render tilemap
	_tileMap.Render();
	//Render Actors
	_unitRenderer.RenderUnits(_unitManager);
	//Render Effects
	
	//Render UI
	_ui.Render();
}

void WorldMap::Update(const float delta)
{
	//Update Background
	_background.Update(delta);
	//Update Tilemap

	//Update Actors

	//Update UI
	_ui.Update(delta);
}

void WorldMap::HandleInput(Uint8* KeyStates)
{
	_ui.HandleInput(KeyStates);
}


WorldMapBackGround::WorldMapBackGround()
{
	_backgroundTexture = TextureLoader::Load("background.jpg");
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





