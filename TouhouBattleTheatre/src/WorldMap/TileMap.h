#pragma once

#include "../../TouhouBattleTheatre.h"

// Tile Map mask dictionary
// Mask determine the terrain of the tile.
// 
// 0 = void
// 1 = plain ( or other flat land. )
// 2 = forest ( or other thick vegeterain )
// 4 = grass ( or other thin vegeterain )
// 8 = mountain ( or other rugged high land )
// 16 = hill ( or other rugged low land )
// 32 = lake/river ( or other fresh liquid )
// 64 = sea ( or other salty liquid )
// 128 = lava
// 256 = swamp ( or other muddy land )
// 512 = cave ( or other underground land )
// 1024 = cavewall ( or other solid wall )
// For example, 1024 + 4 + 64 = enclosed kelp farm, 512 + 2 = mushroom cave, 8 + 2 = forested mountain



// Land feature and wonder points dictionary
// 0 = nothing
// 1 = shrine

// Land biome dictionary
// 0 = void
// 1 = Makai Normal (Temperatue countryside)
// 2 = Makai Hot
// 4 = Makai Cold
// 8 = Modern Outsideworld (Main storyline)
// 16 = Gensokyo
// 32 = Future Outsideworld (Seal club)
// 64 = Netherworld
// 128 = Celestial Realm
// 256 = Old Hell
// 512 = Lunar Capital
// 1024 = PC-98 Hell
// 2048 = PC-98 Heaven


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
	void _initFogOfWar();

};