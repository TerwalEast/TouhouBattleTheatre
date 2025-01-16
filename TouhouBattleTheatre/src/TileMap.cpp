#include "TileMap.h"
#include <SOIL2/SOIL2.h>

TileMap_Test::TileMap_Test()
{
	_walkableMapSizeX = 15;
	_walkableMapSizeY = 10;
	_tiles = new Tile[(_walkableMapSizeX + 2) * (_walkableMapSizeY + 2)];

	//Load Texture Atlas as texture array

	GLuint texture = 0;

	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &texture);

	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureStorage3D(texture, 1, GL_RGBA8, 32, 32, 9);

	int _width, _height;
	unsigned char* pixels = SOIL_load_image("res/TileMap/TileMap.png", &_width, &_height, 0, SOIL_LOAD_AUTO);

	glTextureSubImage3D(texture, 0, 0, 0, 0, 32, 32, 9, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

}
