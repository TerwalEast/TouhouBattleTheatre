
#pragma once

#include "../TouhouBattleTheatre.h"


//Tile Map mask dictionary
// 0 = void
// 1 = grassfield
// 2 = forest
// 3 = mountain
// 4 = river
// 5 = dirt road


class TerrainTile
{
public:
	TerrainTile();
	~TerrainTile();
protected:
	// ------------------------------------------------
	//+Y
	// Ʌ
	// |
	// |T+X+1  T+X+2  T+X+3
	// |
	// | T-1     T     T+1
	// |
	// |T-X-3  T-X-2  T-X-1
	// O---------------------> +X
	// -----------------------------------------------
	int _tileType = 0;
};


class TileMap
{
public:
	TileMap();
	void Render();

private:
	int _walkableMapSizeX; //Beware there is a circle of false tiles around the map
	int _walkableMapSizeY; //So actual size of map is X+2 * Y+2
	TerrainTile* _tiles; //Dynamic allocate memory
	GLuint _tilesheetTexture;
	GLuint _vao;
	//void _updateMap();

};