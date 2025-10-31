#include "Cursor.h"
#include "../ShaderManager.h"
#include "../TextureManager.h"

#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>

namespace {
    constexpr float TILE_SIZE = 8.0f;
    constexpr float VERTICES[] = {
        // Positions        // Texture Coords
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        TILE_SIZE, 0.0f, 0.0f,   1.0f, 0.0f,
        TILE_SIZE, 0.0f, TILE_SIZE,   1.0f, 1.0f,
        0.0f, 0.0f, TILE_SIZE,   0.0f, 1.0f
    };
    constexpr size_t VERTEX_COUNT = 4;
    constexpr size_t COMPONENTS_PER_VERTEX = 5;
    constexpr size_t VERTEX_STRIDE = COMPONENTS_PER_VERTEX * sizeof(float);
}

Cursor::Cursor()
{
	_cursorTexture = Texture_Load(GET_TEXTURE_PATH(Cursor.png));
	glCreateBuffers(1, &_vbo);
	glNamedBufferStorage(_vbo, sizeof(VERTICES), VERTICES, 0); // Removed GL_DYNAMIC_STORAGE_BIT as it's not dynamic

	glCreateVertexArrays(1, &_vao);

    // Bind VBO to the VAO's binding point 0
	glVertexArrayVertexBuffer(_vao, 0, _vbo, 0, VERTEX_STRIDE);

    // Position attribute (location = 0)
	glEnableVertexArrayAttrib(_vao, 0);
	glVertexArrayAttribBinding(_vao, 0, 0);
	glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

    // Texture coordinate attribute (location = 1)
	glEnableVertexArrayAttrib(_vao, 1);
	glVertexArrayAttribBinding(_vao, 1, 0);
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
    auto& shaderManager = ShaderManager::GetInstance();
    auto it = shaderManager.ShaderMap().find("actor");
    if (it == shaderManager.ShaderMap().end()) {
        // Optionally log an error if the shader is not found
		spdlog::error("Shader 'actor' not found in ShaderManager.");
        return;
    }

	glUseProgram(it->second.id);
	glBindVertexArray(_vao);
	glBindTextureUnit(0, _cursorTexture);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(_cursorTileX * TILE_SIZE, 0.0f, _cursorTileY * TILE_SIZE));

	shaderManager.SetUniform("actor", "model", UniformType::MAT4, (void*)&model);

	glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_COUNT);
	glBindVertexArray(0);
}

void Cursor::Update(const float delta)
{
	//float x, y;
	//SDL_GetMouseState(&x, &y);
	//_updateCursorPos(x, y);
}

void Cursor::HandleInput()
{

}

void Cursor::UpdateCursorPos(const float mouse_x, const float mouse_y, const float camera_x, const float camera_y, const float zoom, const float screen_width, const float screen_height, const float view_width, const float view_height)
{
	// Convert mouse coordinates to world coordinates
	float world_x = camera_x + (mouse_x / screen_width - 0.5f) * view_width;
	float world_y = camera_y + (0.5f - mouse_y / screen_height) * view_height;

	// Convert world coordinates to tile coordinates
	if (world_x >= 0 && world_y >= 0) {
		_cursorTileX = static_cast<int>(world_x / TILE_SIZE);
		_cursorTileY = static_cast<int>(world_y / TILE_SIZE);
	} else {
        // Keep the cursor at a non-negative position if world coordinates are negative
        _cursorTileX = 0;
        _cursorTileY = 0;
    }
}

glm::vec2 Cursor::GetCursorPos()
{
	return glm::vec2(_cursorTileX, _cursorTileY);
}
