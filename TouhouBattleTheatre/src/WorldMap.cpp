#include "WorldMap.h"
#include "ShaderManager.h"


WorldMap::WorldMap()
{

}

WorldMap::~WorldMap()
{
}

void WorldMap::Render()
{
	//Render background
	
	//Render tilemap
	_tileMap.Render();
	//Render Actors
	
	//Render Effects
	
	//Render UI
}

GLuint _loadBackgroundTexture(const char* path)
{
	stbi_set_flip_vertically_on_load(true);

	GLuint texture;
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
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else if (nrChannels == 4)
		{
			glTextureStorage2D(texture, 1, GL_RGBA8, width, height);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			spdlog::error("Failed to load texture {}, strange file format", path);
		}
	}
	else
	{
		spdlog::error("Failed to load texture {}, cannot read data", path);
	}
	stbi_image_free(data);
	return texture;
}

WorldMapBackGround::WorldMapBackGround()
{
	_backgroundTexture = _loadBackgroundTexture("res/texture/background.jpg");
	glCreateBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glNamedBufferStorage(_vbo, (sizeof(float) * 5)
		* 4, _vertices, GL_DYNAMIC_STORAGE_BIT);

	glCreateVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glEnableVertexArrayAttrib(_vao, 0);
	glVertexArrayAttribBinding(_vao, 0, 0);
	glVertexArrayVertexBuffer(_vao, 0, _vbo, 0, 5 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

	glEnableVertexArrayAttrib(_vao, 1);
	glVertexArrayAttribBinding(_vao, 1, 0);
	glVertexArrayVertexBuffer(_vao, 1, _vbo, 0, 5 * sizeof(float));
	glVertexArrayAttribFormat(_vao, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));


}

WorldMapBackGround::~WorldMapBackGround()
{
}

void WorldMapBackGround::Render()
{
	glUseProgram(ShaderManager::GetInstance().ShaderMap().at("background").id);
	glBindVertexArray(_vao);
	glBindTextureUnit(0, _backgroundTexture);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

