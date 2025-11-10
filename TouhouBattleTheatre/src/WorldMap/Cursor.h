#pragma once

#include "../../TouhouBattleTheatre.h"

enum CursorState
{
	CURSOR_DEFAULT,
	CURSOR_SELECT,
	CURSOR_TARGET,
	CURSOR_PATH
};

class Cursor
{
public:
	Cursor();
	~Cursor();
	void Render();
	void Update(const float delta);
	void HandleInput();
	void UpdateCursorPos(const float mouse_x, const float mouse_y, const float camera_x, const float camera_y, const float zoom, const float screen_width, const float screen_height, const float view_width, const float view_height);
	glm::vec2 GetCursorPos();
private:
	GLuint _cursorTexture;
	GLuint _selectTexture;
	GLuint _targetTexture;
	GLuint _pathTexture;
	GLuint _vao;
	GLuint _vbo;
	unsigned int _cursorTileX = 0;
	unsigned int _cursorTileY = 0;
	CursorState _state = CURSOR_DEFAULT;

};