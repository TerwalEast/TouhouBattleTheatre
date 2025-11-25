#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "WorldUIItem.h"
#include <memory>

class WorldUI;

struct CameraParams 
{
	float camera_x, camera_y;
	float view_width, view_height;
	float screen_width, screen_height;
};

enum CursorState
{
	CURSOR_DEFAULT,
	CURSOR_SELECT,
	CURSOR_TARGET,
	CURSOR_PATH
};


 /**
  * 渲染光标和选择框的类，不负责游戏逻辑。
  */

class Cursor{
public:
	Cursor(WorldUI& worldUI);
	~Cursor();

	void Render();
	void Update(const float delta);
	void UpdateCursorPos(const int tile_x, const int tile_y);

	void SetState(CursorState state);
	void SetTargetTile(int tile_x, int tile_y);

	glm::vec2 GetCursorPos();

private:
	GLuint _vbo = 0;
	GLuint _vao = 0;
	GLuint _cursorTexture = 0;
	GLuint _selectTexture = 0;
	GLuint _pathTexture = 0;
	GLuint _targetTexture = 0;

	int _cursorTileX = 0;
	int _cursorTileY = 0;

	int _targetTileX = 0;
	int _targetTileY = 0;

	CursorState _state = CURSOR_DEFAULT;
	WorldUI& _worldUI;
	
};