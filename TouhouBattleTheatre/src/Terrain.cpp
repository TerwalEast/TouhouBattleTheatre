#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>


Terrain::Terrain(int x, int y, int generateType = 0)
{
	this->_x = x;
	this->_y = y;

	_vertices = (float*)malloc(sizeof(float) * x * y * 4 * 3 * 2);

	_tiles = (Tile*)malloc(sizeof(Tile) * x * y);

	switch (generateType)
	{
		//平地
	case 0:
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				_tiles[i * y + j].height = 1;
				_tiles[i * y + j].mergeType = ALL;
			}
		}
		break;

	case 1:
		//随机生成柱子山
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				_tiles[i * y + j].height = rand() % 10;
				_tiles[i * y + j].mergeType = NONE;
			}
		}
		break;

	case 2:
		//斜坡
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				_tiles[i * y + j].height = i;
				_tiles[i * y + j].mergeType = ALL;
			}
		}
		break;

	case 3:
		//细浪
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				_tiles[i * y + j].height = i % 3;
				_tiles[i * y + j].mergeType = ALL;
			}
		}
		break;

	default:

		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				_tiles[i * y + j] = { 0, NONE };
			}
		}
		break;
	}
	_bakeMesh();
	_printResult();
	_upLoadMeshToGPU();
}

Terrain::~Terrain()
{
	free(_tiles);
	free(_vertices);
}

void Terrain::Render()
{
	glUseProgram(ShaderManager::GetInstance().ShaderMap().at("terrain").id);
	glBindVertexArray(_vao);
	glm::mat4 modelMatrixPyramid = glm::mat4(1.0); // We start with identity matrix
	modelMatrixPyramid = glm::translate(modelMatrixPyramid, glm::vec3(0.0f, -30.0f, -50.0f)); // Translate first
	modelMatrixPyramid = glm::rotate(modelMatrixPyramid, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Then rotate
	modelMatrixPyramid = glm::scale(modelMatrixPyramid, glm::vec3(3.0f, 3.0f, 3.0f)); // Scale at last

	glm::vec4 color;

	ShaderManager::GetInstance().SetUniform("terrain", "model", UniformType::MAT4, (void*)&modelMatrixPyramid);
	color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	ShaderManager::GetInstance().SetUniform("terrain", "color", UniformType::VEC4, (void*)&color);
	glDrawArraysInstanced(GL_POINTS, 0, 1, _x * _y);
	glBindVertexArray(0);
}

void Terrain::_bakeMesh()
{
	int vertexCount = 0;
	
	int offset = 0;
	RevelentTile revelentTile;
	for (int tileX = 0; tileX < _x; tileX++)
	{
		for (int tileY = 0; tileY < _y; tileY++)
		{
			int baseHeight = _tiles[tileX * _y + tileY].height;
			if (_tiles[tileX * _y + tileY].mergeType == NONE)
			{
				for (int vertexX = tileX; vertexX < tileX + 2; vertexX++)
				{
					for (int vertexY = tileY; vertexY < tileY + 2; vertexY++)
					{
						_vertices[offset] = vertexX;
						_vertices[offset + 1] = baseHeight;
						_vertices[offset + 2] = vertexY;
						offset += 3;
						spdlog::debug("Point:{} {} {}", vertexX, baseHeight, vertexY);
					}
				}
				continue;
			}
			if (_tiles[tileX * _y + tileY].mergeType == ALL)
			{
				for (int vertexX = tileX; vertexX < tileX + 2; vertexX++) 
				{
					for (int vertexY = tileY; vertexY < tileY + 2; vertexY++)
					{
						int sum = 0;
						int total = 0;
						_getRevelentTile(vertexX, vertexY, revelentTile);
						for (int targetVertexIndex = 0; targetVertexIndex < revelentTile.num; targetVertexIndex++) //顺序是-x-y,-x+y,+x-y,+x+y
						{
							Tile* targetTile = &_tiles
								[revelentTile.positions[targetVertexIndex * 2] * _y 
								+ revelentTile.positions[targetVertexIndex * 2 + 1]];
							if (targetTile->mergeType == ALL)
							{
								sum += targetTile->height;
								total++;
							}
						}
						_vertices[offset] = vertexX;
						_vertices[offset + 1] = (float)sum / (float)total;
						_vertices[offset + 2] = vertexY;
						offset += 3;
					}
				}
				continue;
			}
		}
	}
}

void Terrain::_getRevelentTile(int x, int y, RevelentTile& revelentTile)
{
	revelentTile.num = 0;
	for(int i = x - 1; i < x + 1; i++)
	{
		for (int j = y - 1; j < y + 1; j++)
		{
			if (i < 0 || i > (this->_x - 1) || j < 0 || j > (this->_y - 1) )
			{
				continue;
			}
			revelentTile.positions[revelentTile.num * 2 ] = i;
			revelentTile.positions[revelentTile.num * 2 + 1] = j;
			revelentTile.num++;
		}
	}
	
}

void Terrain::_upLoadMeshToGPU()
{
	GLuint instanceVBO;
	spdlog::debug("Mesh Buffer Creating");

	glCreateBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glNamedBufferStorage(instanceVBO, sizeof(float) * 4 * 3 * _x * _y, _vertices, GL_DYNAMIC_STORAGE_BIT);

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

void Terrain::_printResult()
{
	for (int i = 0; i < _x * _y * 4 * 3; i+=12)
	{
		spdlog::info("Tile[{}][{}]: {} {} {}, {} {} {}, {} {} {}, {} {} {}",
			(i / 12) / _y, i / 12 % _y,
			_vertices[i], _vertices[i + 1], _vertices[i + 2],
			_vertices[i + 3], _vertices[i + 4], _vertices[i + 5],
			_vertices[i + 6], _vertices[i + 7], _vertices[i + 8],
			_vertices[i + 9], _vertices[i + 10], _vertices[i + 11]);
	}
}

