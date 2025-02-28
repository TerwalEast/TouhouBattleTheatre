#include "Cursor.h"
#include "../ShaderManager.h"
#include "../TextureManager.h"

#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>

Cursor::Cursor()
{
	_cursorTexture = Texture_Load(GET_TEXTURE_PATH(Cursor.png));
	glCreateBuffers(1, &_vbo);

	float _vertices[20] = {
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		8.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		8.0f, 0.0f, 8.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 8.0f, 0.0f, 1.0f
	};

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

Cursor::~Cursor()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteVertexArrays(1, &_vao);
	glDeleteTextures(1, &_cursorTexture);
}

void Cursor::Render()
{
	glUseProgram(ShaderManager::GetInstance().ShaderMap().at("actor").id);
	glBindVertexArray(_vao);
	glBindTextureUnit(0, _cursorTexture);

	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, glm::vec3(_cursorTileX * 8.0f, 0.0f, _cursorTileY * 8.0f));

	ShaderManager::GetInstance().SetUniform("actor", "model", UniformType::MAT4, (void*)&model);

	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glBindVertexArray(0);
}

void Cursor::Update(const float delta)
{
	float x, y;
	SDL_GetMouseState(&x, &y);
	//_updateCursorPos(x, y);
}

void Cursor::HandleInput()
{

}

void Cursor::UpdateCursorPos(const float mouse_x, const float mouse_y, const float camera_x, const float camera_y, const float zoom, const float screen_width, const float screen_height, const float view_width, const float view_height)
{

	//map -0.5 to 0.5
	float normalized_x = mouse_x / screen_width - 0.5f;
	float normalized_y = 0.5f - mouse_y / screen_height;

	//map to view size
	float offset_x = normalized_x * view_width;
	float offset_y = normalized_y * view_height;

	//map to world coord
	float world_x = camera_x + offset_x;
	float world_y = camera_y + offset_y;

	//spdlog::info("World Pos: {} {}", world_x, world_y);

	//map to tile
	int tile_x = world_x >= 0 ? (int)world_x / 8 : 0;
	int tile_y = world_y >= 0 ? (int)world_y / 8 : 0;

	//spdlog::info("Tile Pos: {} {}", tile_x, tile_y);

	_cursorTileX = tile_x;
	_cursorTileY = tile_y;

}

glm::vec2 Cursor::GetCursorPos()
{
	return glm::vec2(_cursorTileX, _cursorTileY);
}
