#include "Cursor.h"
#include "WorldUI.h"
#include "../ShaderManager.h"
#include "../TextureLoader.h"
#include "UnitManager.h"

#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>



Cursor::Cursor(WorldUI &worldUI) : _worldUI(worldUI)
{
	_cursorTexture = TextureLoader::Load("cursor/cursor.png");
	_selectTexture = TextureLoader::Load("cursor/select.png");
	_pathTexture = TextureLoader::Load("cursor/path.png");
	_targetTexture = TextureLoader::Load("cursor/target.png");

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

	_cursorUIItem = std::make_shared<WorldUIItem>(glm::vec2(0.0f, 0.0f),
		glm::vec2(TestApplication::GetInstance().GetScreenWidth(),
			TestApplication::GetInstance().GetScreenHeight()));
	_cursorUIItem->OnClick = [this]() {
		this->Click();
		};
	_cursorUIItem->ConsumeClick = true;

	_worldUI.RegisterUIItem(_cursorUIItem);

};

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
		spdlog::error("Shader 'actor' not found in ShaderManager.");
        return;
    }

	glUseProgram(it->second.id);
	glBindVertexArray(_vao);

	if (_state == CURSOR_DEFAULT)
	{
		glBindTextureUnit(0, _cursorTexture);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(_cursorTileX * TILE_SIZE, 0.0f, _cursorTileY * TILE_SIZE));
		shaderManager.SetUniform("actor", "model", UniformType::MAT4, (void*)&model);
		glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_COUNT);
		glBindVertexArray(0);
	}
	else if (_state == CURSOR_SELECT)
	{
		glBindTextureUnit(0, _selectTexture);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(_targetTileX * TILE_SIZE, 0.0f, _targetTileY * TILE_SIZE));
		shaderManager.SetUniform("actor", "model", UniformType::MAT4, (void*)&model);
		glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_COUNT);

		glBindTextureUnit(0, _targetTexture);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(_cursorTileX * TILE_SIZE, 0.0f, _cursorTileY * TILE_SIZE));
		shaderManager.SetUniform("actor", "model", UniformType::MAT4, (void*)&model);
		glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_COUNT);
		glBindVertexArray(0);

	}
}

void Cursor::Update(const float delta)
{
	//float x, y;
	//SDL_GetMouseState(&x, &y);
	//_updateCursorPos(x, y);
	if (_state == CursorState::CURSOR_DEFAULT)
	{
		// 什么也不做，指针处于默认状态
	}
	else if (_state == CursorState::CURSOR_SELECT)
	{
		// 指针处于选择状态，选中了一个单位
	}

}

void Cursor::UpdateCursorPos(const int tile_x, const int tile_y)
{
	_cursorTileX = tile_x;
	_cursorTileY = tile_y;
}


void Cursor::SetState(CursorState state)
{
	_state = state;
}

void Cursor::SetTargetTile(int tile_x, int tile_y)
{
	_targetTileX = tile_x;
	_targetTileY = tile_y;
}

glm::vec2 Cursor::GetCursorPos()
{
	return glm::vec2(_cursorTileX, _cursorTileY);
}

void Cursor::Click()
{
	spdlog::info("Cursor clicked at tile ({}, {})", _cursorTileX, _cursorTileY);
	if (_state == CursorState::CURSOR_DEFAULT)
	{
		Select(_cursorTileX, _cursorTileY);
	}
	else if (_state == CursorState::CURSOR_SELECT)
	{
		_state = CursorState::CURSOR_DEFAULT;
	}
}

void Cursor::Select(int tile_x, int tile_y)
{
	// TODO 先检查是否处于战争迷雾中 等战争迷雾实装
	//CheckVisible();

	// 检查逻辑：友军-》敌人-》地表特征-》地形
	//UnitManager& unitManager = 

	

}
