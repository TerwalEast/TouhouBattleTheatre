
#pragma once

#include "../TouhouBattleTheatre.h"
#include "TestApplication.h"

#include <stb_image.h>

#include <map>

//����C�ð���

#define TEXTURE_PATH_PREFIX "res/texture/"
#define TO_STRING(x) #x

#define GET_TEXTURE_PATH(x) TEXTURE_PATH_PREFIX TO_STRING(x)  //GET_TEXTURE_PATH(x) -> "res/texture/x"



static GLuint Texture_Load(const char* path)
{
	spdlog::info("Loading texture {}", path);
	stbi_set_flip_vertically_on_load(true);

	GLuint texture{};
	GLsizei width, height, nrChannels;
	stbi_uc* data = stbi_load(path, &width, &height, &nrChannels, 0);

	if (data)
	{

		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		if (nrChannels == 3)
		{
			glTextureStorage2D(texture, 1, GL_RGB8, width, height);
			glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4)
		{
			glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
			glTextureSubImage2D(texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			spdlog::error("Failed to load texture {}, strange file format", path);
			texture = 0;
		}
	}
	else
	{
		spdlog::error("Failed to load texture {}, cannot read data", path);
		texture = 0;
	}
	stbi_image_free(data);
	return texture;
}

static GLuint Texture_LoadTileAtlas(const char* filePath, const int tileWidth, const int tileHeight)
{
	stbi_set_flip_vertically_on_load(true);
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

class TextureManager
{
public:
	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}
	//GLuint LoadTexture(const char* path);
	//GLuint LoadTileAtlas(const char* filePath, const int tileWidth, const int tileHeight);
	std::map<std::string, GLuint> TextureMap() { return _textureMap; }

	GLuint GetTexture(const char* name)
	{
		if (_textureMap.find(name) == _textureMap.end())
		{
			spdlog::error("Texture {} not found", name);
			return 0;
		}
		return _textureMap[name];
	}

	//Costly
	std::string GetTextureName(GLuint texture) 
	{
		for (auto& [name, tex] : _textureMap)
		{
			if (tex == texture)
			{
				return name;
			}
		}
		return "";
	}

	void LoadTexture(const char* path, const char* name)
	{
		int texture = Texture_Load(path);
		if (texture)
			_textureMap[name] = Texture_Load(path);
		else
			spdlog::error("Failed to load texture {}", path);
	}

	void LoadTileAtlas(const char* filePath, const int tileWidth, const int tileHeight, const char* name)
	{
		int texture = Texture_LoadTileAtlas(filePath, tileWidth, tileHeight);
		if (texture)
			_textureMap[name] = texture;
		else
			spdlog::error("Failed to load texture {}", filePath);
	}

private:
	TextureManager() {};
	~TextureManager() {};
	TextureManager(TextureManager const&) = delete;
	void operator=(TextureManager const&) = delete;
	std::map<std::string, GLuint> _textureMap;

};