#pragma once


#include "Stage.h"
#include "TileMap.h"

#include "../TouhouBattleTheatre.h"

class WorldMapBackGround
{
public:
	WorldMapBackGround();
	~WorldMapBackGround();
	void Render();
private:
	GLuint _backgroundTexture;
	GLuint _vao;
	GLuint _vbo;
	float _vertices[20] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};
};

class WorldMap : public Stage
{
public:
	WorldMap();
	~WorldMap();

	void Render();
	void Update();
	void HandleInput();

protected:
	
private:

	TileMap _tileMap;
	int _turnNumber = 1;
	


};