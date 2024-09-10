#include "Terrain.h"



Terrain::Terrain(int x, int y, int generateType = 0)
{
	this->x = x;
	this->y = y;

	vertices = (float*)malloc(sizeof(float) * x * y * 4 * 3);

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
				tiles[i * y + j].height = i % 2;
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

	shaderInfo.hasGeom = false;
	shaderInfo.shaderCode = "terrain";
	shaderInfo.uniformList = {"",""};

	_bakeMesh();


}

Terrain::~Terrain()
{
	free(tiles);
	free(vertices);
}

void Terrain::render()
{

	glBindVertexArray(vao);

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
						spdlog::info("Point:{} {} {}", vertexX, baseHeight, vertexY);
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
	_printResult();
}

void Terrain::_getRevelentTile(int x, int y, RevelentTile& revelentTile)
{
	revelentTile.num = 0;
	for(int i = x - 1; i < x + 1; i++)
	{
		for (int j = y - 1; j < y + 1; j++)
		{
			if (i < 0 || i > this->x + 1 || j < 0 || j > this->y + 1)
			{
				continue;
			}
			revelentTile.num ++;
			revelentTile.positions[revelentTile.num * 2 - 2] = i;
			revelentTile.positions[revelentTile.num * 2 - 1] = j;
		}
	}
}

void Terrain::_upLoadMeshToGPU()
{
	spdlog::debug("Mesh Buffer Creating");
	glCreateBuffers(1, &_vbo);
	glNamedBufferStorage(_vbo, sizeof(float) * x * y * 4 * 3, vertices, GL_DYNAMIC_STORAGE_BIT);
	glCreateVertexArrays(1, &vao);
	glVertexArrayVertexBuffer(vao, 0, _vbo, 0, 3 * sizeof(float));
	glEnableVertexArrayAttrib(vao, 0);
	glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
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

