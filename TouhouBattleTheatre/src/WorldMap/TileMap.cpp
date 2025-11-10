#include "TileMap.h"

#include "../TextureLoader.h"
#include "../ShaderManager.h"

#include <stb_image.h>

struct tileVertex
{
	float x;
	float y;
	float z;
	float w;
};

GLuint _bakeVAO(const int map_x, const int map_y)
{
	void* _vertices = new tileVertex[(map_x + 2) * (map_y + 2)];

	for (int i = 0; i < (map_x + 2) * (map_y + 2); i++)
	{
		((tileVertex*)_vertices)[i].x = (i % (map_y + 2)) * 8;
		((tileVertex*)_vertices)[i].y = 0.0f;
		((tileVertex*)_vertices)[i].z = (i / (map_y + 2)) * 8;

		//虽然这里理论上应该写个威猛的bitmap奇技淫巧来自动判断是否是边角tiletype
		// 但是考虑到之后还有不止一种地形（草地沙漠道路等等）
		//所以先这样吧
		if (i % (map_y + 2) == 0) // x = 0
		{
			if (i / (map_y + 2) == 0)
				((tileVertex*)_vertices)[i].w = 1.0f - 1.0f;
			else if (i / (map_y + 2) == map_x + 1)
				((tileVertex*)_vertices)[i].w = 7.0f - 1.0f;
			else
				((tileVertex*)_vertices)[i].w = 4.0f - 1.0f;
		}
		else if (i % (map_y + 2) == map_y + 1) // x = map_x + 1
		{
			if (i / (map_y + 2) == 0)
				((tileVertex*)_vertices)[i].w = 3.0f - 1.0f;
			else if (i / (map_y + 2) == map_x + 1)
				((tileVertex*)_vertices)[i].w = 9.0f - 1.0f;
			else
				((tileVertex*)_vertices)[i].w = 6.0f - 1.0f;
		}
		else if (i / (map_y + 2) == 0) // y = 0
		{
			((tileVertex*)_vertices)[i].w = 2.0f - 1.0f;
		}
		else if (i / (map_y + 2) == map_x + 1) // y = map_y + 1
		{
			((tileVertex*)_vertices)[i].w = 8.0f - 1.0f;
		}
		else
		{
			((tileVertex*)_vertices)[i].w = 5.0f - 1.0f;
		}
		//spdlog::info("Tile[{}][{}]: {} {} {} {}", i % (map_y + 2), i / (map_y + 2), ((tileVertex*)_vertices)[i].x, ((tileVertex*)_vertices)[i].y, ((tileVertex*)_vertices)[i].z, ((tileVertex*)_vertices)[i].tileType);
	}

		GLuint instanceVBO, vao;
		spdlog::debug("Creating VAO for tile map");

		glCreateBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glNamedBufferStorage(instanceVBO, (sizeof(float) * 4)
			* (map_x + 2) * (map_y + 2), _vertices, GL_DYNAMIC_STORAGE_BIT);

		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glEnableVertexArrayAttrib(vao, 0);
		glVertexArrayAttribBinding(vao, 0, 0);
		glVertexArrayVertexBuffer(vao, 0, instanceVBO, 0, 4 * sizeof(float));
		glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
		return vao;
}



TileMap::TileMap()
{
	_walkableMapSizeX = 16;
	_walkableMapSizeY = 9;
	_tiles = new TerrainTile[(_walkableMapSizeX + 2) * (_walkableMapSizeY + 2)];

	//Load Texture Atlas as texture array
	_tilesheetTexture = TextureLoader::LoadTileAtlas("grass_atlas.png", 32, 32);
	
	//Create VAO for tile map
	_vao = _bakeVAO(_walkableMapSizeX, _walkableMapSizeY);
}

void TileMap::Render()
{
	glUseProgram(ShaderManager::GetInstance().ShaderMap().at("tiles").id);
	glBindVertexArray(_vao);
	glBindTextureUnit(0, _tilesheetTexture);

	glm::mat4 modelMatrixPyramid = glm::mat4(1.0); // We start with identity matrix
	ShaderManager::GetInstance().SetUniform("tiles", "model", UniformType::MAT4, (void*)&modelMatrixPyramid);
	//ShaderManager::GetInstance().SetUniform("tiles", "texarray", UniformType::SAMPLER2DARRAY, (void*)&);
	glDrawArrays(GL_POINTS, 0, (_walkableMapSizeX + 2) * (_walkableMapSizeY + 2));
	glBindVertexArray(0);
}

TerrainTile::TerrainTile()
{
	_tileType = 0;
}

TerrainTile::~TerrainTile()
{
}
