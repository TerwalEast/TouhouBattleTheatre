#pragma once

#include "../TouhouBattleTheatre.h"
#include "ShaderManager.h"
#include <string>

enum MergeType {
	NONE,ALL
};


struct Tile {
	// 高度意为这个方格中心点的高度。最小为0。
	int height;
	MergeType mergeType;
};

struct RevelentTile {
	int num;
	int positions[8];
};

class Terrain {

public:
	int x;
	int y;
	int vertexSize;
	float* vertices;
	Tile* tiles;
	GLuint vao;
	//ShaderInfo shaderInfo;
	Terrain() = delete;
	//bool updateTerrain( std::string tilesXMLPath );
	Terrain(int x, int y, int generateType);
	~Terrain();
	void render();

private:
	GLuint _vbo;
	//void _initShaderProgram();
	void _bakeMesh();
	void _getRevelentTile(int x, int y, RevelentTile &revelentTile);
	void _upLoadMeshToGPU();
	void _printResult();
	
};
