//Map builder tool
#pragma once

#include <string>
#include <vector>
#include <map>
#include "glm/glm.hpp"

#define MAX_MAP_SIZE 200


enum TileAlign
{
	NORTH,EAST,SOUTH,WEST
};

struct Tile
{
	std::string Name;
	int PointA;
	int PointB;
	int PointC;
	int PointD;
};

struct TileInstance
{
	std::string Name;
	int X;
	int Y;
	TileAlign Align;
};


class MapTool 
{
public:
	MapTool();
	~MapTool();

	void Init();
	void Update();
	void Render();
	void Release();

	void LoadMap(std::string path);
	void SaveMap(std::string path);
	void BakeMapMesh(std::string path);

	void SetMapSize(int width, int height);
	
	
private:
	
	int _width;
	int _height;
	std::vector<Tile> _tilesDictionary;




};