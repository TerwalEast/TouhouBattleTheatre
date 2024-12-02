
#pragma once

#include <string>
#include <vector>
#include <map>

#include "../TouhouBattleTheatre.h"
#include "TestApplication.h"

#define MAX_MAP_WIDTH 100
#define MAX_MAP_HEIGHT 100
#define MAX_TILE_INDEX 255

enum EditorTool
{
	PICK,PLACE
};

struct MapTile
{
	std::string Name;
	float PointA;
	float PointB;
	float PointC;
	float PointD;
};

class MapTool 
{
public:
	MapTool() = delete;
	MapTool(int width = 20, int height = 20);
	~MapTool();

	void Init();
	void PickTile();
	void Render();
	void UpdateMap();

	void LoadMap(std::string path);
	void SaveMap(std::string path);
	void BakeMapMesh(std::string path);

	void SetMapSize(int width, int height);
	
	
private:
	
	void _initTiles(int tileID);
	void _initTilesDictionary();
	void _initVertices();
	void _updateVertices();
	void _exportToObj();
	float _vertices[MAX_MAP_HEIGHT * MAX_MAP_WIDTH * 4 * 3];
	int _sizeX = 20;
	int _sizeY = 20;
	std::vector<MapTile> _tilesDictionary;
	int _tiles[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = { 0 };
	int _elevation[MAX_MAP_HEIGHT][MAX_MAP_WIDTH] = { 1 };
	int _pickedTile = 0;
	GLuint _vao;
	GLuint _vbo;

};