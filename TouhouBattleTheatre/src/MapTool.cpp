#include "MapTool.h"
#include <glm/ext/matrix_transform.hpp>
#include <random>

void MapTool::SetMapSize(int width, int height)
{
	if (width > MAX_MAP_WIDTH || height > MAX_MAP_HEIGHT)
	{
		spdlog::warn("Map size exceeded max size, cancel");
		return;
	}
	_sizeX = width;
	_sizeY = height;
}

void MapTool::_initTiles(int tileID = 1)
{
	srand((unsigned)time(NULL));
	for (int x = 0; x < MAX_MAP_HEIGHT * MAX_MAP_WIDTH; x++)
		_tiles[x / MAX_MAP_HEIGHT][x % MAX_MAP_WIDTH] = 1;
	//for(int x = 0; x < MAX_MAP_HEIGHT * MAX_MAP_WIDTH; x++)
	//	_elevation[x / MAX_MAP_HEIGHT][x % MAX_MAP_WIDTH] = 3;
	for (int x = 0; x < MAX_MAP_HEIGHT * MAX_MAP_WIDTH; x++)
		_elevation[x / MAX_MAP_HEIGHT][x % MAX_MAP_WIDTH] = rand() % 5;
}

void MapTool::_initTilesDictionary()
{
	//点Z字排布，左下开始
	// C D
	// A B
	
	MapTile plain = { "Plain", 0.0f, 0.0f, 0.0f, 0.0f };
	_tilesDictionary.push_back(plain);

	MapTile slopeNorth = { "SlopeN", 0.0f, 0.0f, 1.0f, 1.0f };
	_tilesDictionary.push_back(slopeNorth);

	MapTile slopeEast = { "SlopeE", 0.0f, 1.0f, 0.0f, 1.0f };
	_tilesDictionary.push_back(slopeEast);

	MapTile slopeSouth = { "SlopeS", 1.0f, 1.0f, 0.0f, 0.0f };
	_tilesDictionary.push_back(slopeSouth);

	MapTile slopeWest = { "SlopeW", 1.0f, 0.0f, 1.0f, 0.0f };
	_tilesDictionary.push_back(slopeWest);

	/*Tile slopeUpperNorthEast = { "SlopeUNE", 0.0f, 1.0f, 1.0f, 1.0f };
	Tile slopeUpperSouthEast = { "SlopeUSE", 1.0f, 1.0f, 1.0f, 0.0f };
	Tile slopeUpperSouthWest = { "SlopeUSW", 1.0f, 1.0f, 0.0f, 1.0f };
	Tile slopeUpperNorthWest = { "SlopeUNW", 1.0f, 0.0f, 1.0f, 1.0f };

	Tile slopeLowerNorthEast = { "SlopeLNE", 0.0f, 0.0f, 1.0f, 0.0f };
	Tile slopeLowerSouthEast = { "SlopeLSE", 0.0f, 1.0f, 0.0f, 0.0f };
	Tile slopeLowerSouthWest = { "SlopeLSW", 1.0f, 0.0f, 0.0f, 0.0f };
	Tile slopeLowerNorthWest = { "SlopeLNW", 0.0f, 0.0f, 0.0f, 1.0f };*/



}

void MapTool::_initVertices()
{
	int offset = 0;
	for (int tileX = 0; tileX < _sizeY; tileX++)
	{
		for (int tileY = 0; tileY < _sizeX; tileY++)
		{
			int vertexX = tileX;
			int vertexY = tileY;

			//A
			_vertices[offset] = vertexX;
			_vertices[offset + 1] = _elevation[tileX][tileY] + _tilesDictionary[ _tiles[tileX][tileY] ].PointA;
			_vertices[offset + 2] = vertexY;
			offset += 3;

			//C
			_vertices[offset] = vertexX;
			_vertices[offset + 1] = _elevation[tileX][tileY] + _tilesDictionary[ _tiles[tileX][tileY] ].PointC;
			_vertices[offset + 2] = vertexY + 1;
			offset += 3;

			//B
			_vertices[offset] = vertexX + 1;
			_vertices[offset + 1] = _elevation[tileX][tileY] + _tilesDictionary[ _tiles[tileX][tileY] ].PointB;
			_vertices[offset + 2] = vertexY;
			offset += 3;

			//D
			_vertices[offset] = vertexX + 1;
			_vertices[offset + 1] = _elevation[tileX][tileY] + _tilesDictionary[ _tiles[tileX][tileY] ].PointD;
			_vertices[offset + 2] = vertexY + 1;
			offset += 3;

		}
	}
}

void MapTool::_exportToObj()
{
	FILE* fp = fopen("C:\\Users\\xhs\\Downloads\\recastnavigation-main\\RecastDemo\\Bin\\Meshes\\test.obj", "wb");
	for (int i = 0; i < _sizeX * _sizeY * 4 * 3; i += 12)
	{
		//上四个点
		fprintf(fp, "v %f %f %f\nv %f %f %f\nv %f %f %f\nv %f %f %f\n",
			_vertices[i], _vertices[i + 1], _vertices[i + 2],
			_vertices[i + 3], _vertices[i + 4], _vertices[i + 5],
			_vertices[i + 6], _vertices[i + 7], _vertices[i + 8],
			_vertices[i + 9], _vertices[i + 10], _vertices[i + 11]);
		//下四个点
		fprintf(fp, "v %f %f %f\nv %f %f %f\nv %f %f %f\nv %f %f %f\n",
			_vertices[i], 0.0f, _vertices[i + 2],
			_vertices[i + 3], 0.0f, _vertices[i + 5],
			_vertices[i + 6], 0.0f, _vertices[i + 8],
			_vertices[i + 9], 0.0f, _vertices[i + 11]);
	}

	for (int i = 0; i < _sizeX * _sizeY * 8; i += 8)
	{
		//面调用的顶点序号从1开始
		fprintf(fp, "f %d %d %d\nf %d %d %d\nf %d %d %d\nf %d %d %d\nf %d %d %d\nf %d %d %d\nf %d %d %d\nf %d %d %d\nf %d %d %d\nf %d %d %d\nf %d %d %d\nf %d %d %d\n",
			i + 1, i + 2, i + 3,
			i + 2, i + 4, i + 3,
			i + 3, i + 4, i + 7,
			i + 4, i + 8, i + 7,
			i + 6, i + 8, i + 2,
			i + 8, i + 4, i + 2,
			i + 7, i + 5, i + 3,
			i + 5, i + 1, i + 3,
			i + 5, i + 6, i + 1,
			i + 6, i + 2, i + 1,
			i + 5, i + 8, i + 6,
			i + 5, i + 7, i + 8);
	}


	fclose(fp);
}

MapTool::MapTool(int width, int height)
{
	SetMapSize(width, height);
	Init();
}

MapTool::~MapTool()
{
	_exportToObj();
}

void MapTool::Init()
{
	_initTilesDictionary();
	_initTiles(1);
	_initVertices();
	spdlog::debug("MapTool Init");
	spdlog::debug("---------------------------- Tiles Index ----------------------------");
	for (int i = 0; i < _tilesDictionary.size(); i++)
	{
		spdlog::debug("Tile Index: {} Name: {}", i, _tilesDictionary[i].Name);
	}
	spdlog::debug("---------------------------- Tiles ----------------------------");
	for (int i = 0; i < _sizeY; i++)
	{
		for (int j = 0; j < _sizeX; j++)
		{
			spdlog::debug("Tile[{}][{}]: {}", i, j, _tiles[i][j]);
		}
	}
	spdlog::debug("---------------------------- Elevation ----------------------------");
	for (int i = 0; i < _sizeY; i++)
	{
		for (int j = 0; j < _sizeX; j++)
		{
			spdlog::debug("Elevation[{}][{}]: {}", i, j, _elevation[i][j]);
		}
	}
	spdlog::debug("---------------------------- Vertices ----------------------------");
	for (int i = 0; i < _sizeY * _sizeX * 4 * 3; i += 12)
	{
		spdlog::debug("Tile[{}][{}]: {} {} {}, {} {} {}, {} {} {}, {} {} {}",
			(i / 12) / _sizeY, i / 12 % _sizeY,
			_vertices[i], _vertices[i + 1], _vertices[i + 2],
			_vertices[i + 3], _vertices[i + 4], _vertices[i + 5],
			_vertices[i + 6], _vertices[i + 7], _vertices[i + 8],
			_vertices[i + 9], _vertices[i + 10], _vertices[i + 11]);
	}

	GLuint instanceVBO;
	spdlog::debug("Mesh Buffer Creating");

	glCreateBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glNamedBufferStorage(instanceVBO, sizeof(float) * 4 * 3 * _sizeX * _sizeY, _vertices, GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glEnableVertexArrayAttrib(_vao, 0);
	glVertexArrayAttribBinding(_vao, 0, 0);
	glVertexArrayVertexBuffer(_vao, 0, instanceVBO, 0, 12 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayBindingDivisor(_vao, 0, 1);

	glEnableVertexArrayAttrib(_vao, 1);
	glVertexArrayAttribBinding(_vao, 1, 1);
	glVertexArrayVertexBuffer(_vao, 1, instanceVBO, 0, 12 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

	glVertexArrayBindingDivisor(_vao, 1, 1);

	glEnableVertexArrayAttrib(_vao, 2);
	glVertexArrayAttribBinding(_vao, 2, 2);
	glVertexArrayVertexBuffer(_vao, 2, instanceVBO, 0, 12 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float));

	glVertexArrayBindingDivisor(_vao, 2, 1);

	glEnableVertexArrayAttrib(_vao, 3);
	glVertexArrayAttribBinding(_vao, 3, 3);
	glVertexArrayVertexBuffer(_vao, 3, instanceVBO, 0, 12 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 3, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float));

	glVertexArrayBindingDivisor(_vao, 3, 1);


}

void MapTool::Render()
{
	glUseProgram(ShaderManager::GetInstance().ShaderMap().at("terrain").id);
	glBindVertexArray(_vao);
	glm::mat4 modelMatrixPyramid = glm::mat4(1.0); // We start with identity matrix
	modelMatrixPyramid = glm::translate(modelMatrixPyramid, glm::vec3(0.0f, 0.0f, 0.0f)); // Translate first
	modelMatrixPyramid = glm::rotate(modelMatrixPyramid, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Then rotate
	modelMatrixPyramid = glm::scale(modelMatrixPyramid, glm::vec3(3.0f, 3.0f, 3.0f)); // Scale at last

	glm::vec4 color;

	ShaderManager::GetInstance().SetUniform("terrain", "model", UniformType::MAT4, (void*)&modelMatrixPyramid);
	color = glm::vec4((64.0f / 255.0f), (224.0f / 255.0f), (205.0f / 255.0f), 1.0f);
	ShaderManager::GetInstance().SetUniform("terrain", "colorSurface", UniformType::VEC4, (void*)&color);
	color = glm::vec4((232.0f / 255.0f), (139.0f / 255.0f), (0.0f / 255.0f), 1.0f);
	ShaderManager::GetInstance().SetUniform("terrain", "colorSide", UniformType::VEC4, (void*)&color);
	glDrawArraysInstanced(GL_POINTS, 0, 1, _sizeX * _sizeY);
	//glDrawArraysInstanced(GL_POINTS, 0, 1, 1);
	glBindVertexArray(0);
}



void _accessTileMeshByXY(int X, int Y) 
{
	
}