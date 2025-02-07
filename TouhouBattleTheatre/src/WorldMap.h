#pragma once


#include "Stage.h"
#include "TileMap.h"
#include "OrthoCameraController.h"

#include "../TouhouBattleTheatre.h"

#include <stb_truetype.h>
#include <vector>

class test_actor
{
public:
	test_actor();
	~test_actor();
private:
	std::string _name;

};

class cursor
{
public:
	cursor();
	~cursor();
	void Render();
	void Update(const float delta);
	void HandleInput();
	void UpdateCursorPos(const float mouse_x, const float mouse_y, const float camera_x, const float camera_y, const float zoom, const float screen_width, const float screen_height, const float view_width, const float view_height);
private:
	GLuint _cursorTexture;
	GLuint _vao;
	GLuint _vbo;
	unsigned int _cursorTileX = 0;
	unsigned int _cursorTileY = 0;
	//void _updateCursorPos(const float mouse_x, const float mouse_y,
	//					  const float camera_x, const float camera_y, 
	//	                  const float zoom, const float screen_width, const float screen_height,
	//					  const float view_width, const float view_height);
};

class Leader 
{
public:
	Leader();
	~Leader();
private:
	std::string _name;
};

class Faction
{
public:
	Faction();
	~Faction();
private:
	std::vector<Leader> _leaders;
};

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

	void Render();
	void Update(const float delta);
	void HandleInput(Uint8* KeyStates);

protected:
	
private:

	OrthoCameraController _cameraController;
	TileMap _tileMap;
	WorldMapBackGround _background;
	int _turnNumber = 1;
	//std::vector<Faction> _factions;
	cursor _cursor;




};