#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>


Terrain::Terrain(int x, int y, int generateType = 0)
{
	this->x = x;
	this->y = y;

	vertices = (float*)malloc(sizeof(float) * x * y * 4 * 3 * 2);

	tiles = (Tile*)malloc(sizeof(Tile) * x * y);

	switch (generateType)
	{
		//平地
	case 0:
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				tiles[i * y + j].height = 1;
				tiles[i * y + j].mergeType = ALL;
			}
		}
		break;

	case 1:
		//随机生成柱子山
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				tiles[i * y + j].height = rand() % 10;
				tiles[i * y + j].mergeType = NONE;
			}
		}
		break;

	case 2:
		//斜坡
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				tiles[i * y + j].height = i;
				tiles[i * y + j].mergeType = ALL;
			}
		}
		break;

	case 3:
		//细浪
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				tiles[i * y + j].height = i % 3;
				tiles[i * y + j].mergeType = ALL;
			}
		}
		break;

	default:

		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				tiles[i * y + j] = { 0, NONE };
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
	free(tiles);
	free(vertices);
}

void Terrain::render()
{
	glUseProgram(ShaderManager::getInstance().shaderMap["terrain"].id);
	glBindVertexArray(vao);
	glm::mat4 modelMatrixPyramid = glm::mat4(1.0); // We start with identity matrix
	modelMatrixPyramid = glm::translate(modelMatrixPyramid, glm::vec3(0.0f, -30.0f, -50.0f)); // Translate first
	modelMatrixPyramid = glm::rotate(modelMatrixPyramid, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Then rotate
	modelMatrixPyramid = glm::scale(modelMatrixPyramid, glm::vec3(3.0f, 3.0f, 3.0f)); // Scale at last

	glm::vec4 color;

	ShaderManager::getInstance().setUniform("terrain", "model", UniformType::MAT4, (void*)&modelMatrixPyramid);
	color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	ShaderManager::getInstance().setUniform("terrain", "color", UniformType::VEC4, (void*)&color);
	/*for (int i = 0; i < y; i++)
	{
		color = glm::vec4(84.0f / 255.0f, 174.0f / 255.0f, 247.0f / 255.0f, 0.5f);
		ShaderManager::getInstance().setUniform("basic", "color", UniformType::VEC4, (void*)&color);
		glDrawArrays(GL_TRIANGLE_STRIP, 0 + 4 * x * i, x * 4);
	}*/
	//glDrawArrays(GL_LINE_STRIP, 0, x * 4 * y);
	glDrawArraysInstanced(GL_POINTS, 0, 1, x * y);
	glBindVertexArray(0);
}

void Terrain::_bakeMesh()
{
	int vertexCount = 0;
	
	int offset = 0;
	RevelentTile revelentTile;
	for (int tileX = 0; tileX < x; tileX++)
	{
		for (int tileY = 0; tileY < y; tileY++)
		{
			int baseHeight = tiles[tileX * y + tileY].height;
			if (tiles[tileX * y + tileY].mergeType == NONE)
			{
				for (int vertexX = tileX; vertexX < tileX + 2; vertexX++)
				{
					for (int vertexY = tileY; vertexY < tileY + 2; vertexY++)
					{
						vertices[offset] = vertexX;
						vertices[offset + 1] = baseHeight;
						vertices[offset + 2] = vertexY;
						offset += 3;
						spdlog::debug("Point:{} {} {}", vertexX, baseHeight, vertexY);
					}
				}
				continue;
			}
			if (tiles[tileX * y + tileY].mergeType == ALL)
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
							Tile* targetTile = &tiles
								[revelentTile.positions[targetVertexIndex * 2] * y 
								+ revelentTile.positions[targetVertexIndex * 2 + 1]];
							if (targetTile->mergeType == ALL)
							{
								sum += targetTile->height;
								total++;
							}
						}
						vertices[offset] = vertexX;
						vertices[offset + 1] = (float)sum / (float)total;
						vertices[offset + 2] = vertexY;
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
			if (i < 0 || i > (this->x - 1) || j < 0 || j > (this->y - 1) )
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
	glNamedBufferStorage(instanceVBO, sizeof(float) * 4 * 3 * x * y, vertices, GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribBinding(vao, 0, 0);
	glVertexArrayVertexBuffer(vao, 0, instanceVBO, 0, 12 * sizeof(float));
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glVertexArrayBindingDivisor(vao, 0, 1);

	glEnableVertexArrayAttrib(vao, 1);
	glVertexArrayAttribBinding(vao, 1, 1);
	glVertexArrayVertexBuffer(vao, 1, instanceVBO, 0, 12 * sizeof(float));
	glVertexArrayAttribFormat(vao, 1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

	glVertexArrayBindingDivisor(vao, 1, 1);

	glEnableVertexArrayAttrib(vao, 2);
	glVertexArrayAttribBinding(vao, 2, 2);
	glVertexArrayVertexBuffer(vao, 2, instanceVBO, 0, 12 * sizeof(float));
	glVertexArrayAttribFormat(vao, 2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float));

	glVertexArrayBindingDivisor(vao, 2, 1);

	glEnableVertexArrayAttrib(vao, 3);
	glVertexArrayAttribBinding(vao, 3, 3);
	glVertexArrayVertexBuffer(vao, 3, instanceVBO, 0, 12 * sizeof(float));
	glVertexArrayAttribFormat(vao, 3, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float));

	glVertexArrayBindingDivisor(vao, 3, 1);

}

void Terrain::_printResult()
{
	for (int i = 0; i < x * y * 4 * 3; i+=12)
	{
		spdlog::info("Tile[{}][{}]: {} {} {}, {} {} {}, {} {} {}, {} {} {}",
			(i / 12) / y, i / 12 % y,
			vertices[i], vertices[i + 1], vertices[i + 2],
			vertices[i + 3], vertices[i + 4], vertices[i + 5],
			vertices[i + 6], vertices[i + 7], vertices[i + 8],
			vertices[i + 9], vertices[i + 10], vertices[i + 11]);
	}
}

