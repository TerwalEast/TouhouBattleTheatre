#pragma once


#include "../../TouhouBattleTheatre.h"

#include "../Stage.h"

#include "../camera/OrthoCameraController.h"
#include "TileMap.h"
#include "WorldUI.h"

#include <stb_truetype.h>
#include <vector>


enum StageState { command, animation, resolve, sleep };

class WorldMapBackGround
{
public:
	WorldMapBackGround();
	~WorldMapBackGround();
	void Render();
	void Update(const float delta);
private:
	GLuint _backgroundTexture;
	GLuint _vao;
	GLuint _vbo;
	float _scrollingTime = 0.0f;
};

class WorldMap : public Stage
{
public:
	WorldMap();
	~WorldMap();
	void Start(SDL_Window* window, Uint8* KeyStates);
	void Render();
	void Update(const float delta);
	void HandleInput(Uint8* KeyStates);

protected:
	
private:
	bool _quit = false;

	//OrthoCameraController _cameraController;
	TileMap _tileMap;
	WorldMapBackGround _background;
	int _turnNumber = 1;
	//std::vector<Faction> _factions;
	WorldUI _ui;
	
	float _cursorPosX = 0, _cursorPosY = 0;
	bool _click = false;
	float wheel = 0.0f;
	StageState _state = command;
	

};