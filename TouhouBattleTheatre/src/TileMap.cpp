#include "TileMap.h"
#include "stb_image.h"

GLuint _loadTex(const char* filePath, const int tileWidth, const int tileHeight)
{
	GLsizei tilesheetWidth, tilesheetHeight, nrChannel;
	stbi_uc* pixels = stbi_load(filePath, &tilesheetWidth, &tilesheetHeight, &nrChannel, STBI_rgb_alpha);
	GLuint tileset;
	GLsizei
		tiles_x = tilesheetWidth / tileWidth,
		tiles_y = tilesheetHeight / tileHeight,
		tile_count = tiles_x * tiles_y;

	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &tileset);

	glTextureParameteri(tileset, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(tileset, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(tileset, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(tileset, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureStorage3D(tileset, 1, GL_RGBA8, tileWidth, tileHeight, tile_count);

	{
		GLuint temp_tex = 0;
		glCreateTextures(GL_TEXTURE_2D, 1, &temp_tex);
		glTextureStorage2D(temp_tex, 1, GL_RGBA8, tilesheetWidth, tilesheetHeight);
		glTextureSubImage2D(temp_tex, 0, 0, 0, tilesheetWidth, tilesheetHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

		for (GLsizei i = 0; i < tile_count; ++i)
		{
			GLint x = (i % tiles_x) * tileWidth, y = (i / tiles_x) * tileHeight;
			glCopyImageSubData(temp_tex, GL_TEXTURE_2D, 0, x, y, 0, tileset, GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, tileWidth, tileHeight, 1);
		}
		glDeleteTextures(1, &temp_tex);
	}

	stbi_image_free(pixels);

	return tileset;
}



TileMap_Test::TileMap_Test()
{
	_walkableMapSizeX = 15;
	_walkableMapSizeY = 10;
	_tiles = new TerrainTile[(_walkableMapSizeX + 2) * (_walkableMapSizeY + 2)];

	//Load Texture Atlas as texture array
	const char path[25] = { "res/tile/grass_atlas.png" };

	_tilesheetTexture = _loadTex(path, 32, 32);

}

TerrainTile::TerrainTile()
{
	_tileType = 0;
}

TerrainTile::~TerrainTile()
{
}
