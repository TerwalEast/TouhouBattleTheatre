#pragma once


#include "../../TouhouBattleTheatre.h"

#include "../Stage.h"

#include "../camera/OrthoCameraController.h"
#include "TileMap.h"
#include "WorldUI.h"
#include "UnitManager.h"

#include <stb_truetype.h>
#include <memory>
#include <vector>

/**
 * 战略地图类。继承Stage，是统领战略地图界面的场景主类。
 */


enum StageState { plot, command, animation, resolve, sleep };

class WorldMapBackGround
{
public:
	WorldMapBackGround();
	~WorldMapBackGround();
	void Render();
	void Update(const float delta);
private:
	GLuint _backgroundTexture;
	GLuint _vao;
	GLuint _vbo;
	float _scrollingTime = 0.0f;
};

class WorldMap : public Stage
{
public:
	WorldMap();
	~WorldMap();
	void Start(SDL_Window* window, Uint8* KeyStates);
	void Render();
	void Update(const float delta);
	void HandleInput(Uint8* KeyStates);

	UnitManager& GetUnitManager() { return _unitManager; }

private:
	bool _quit = false;

	//图块地图类
	TileMap _tileMap;

	//控制背后的滚动背景
	WorldMapBackGround _background;
	
	//当前回合数
	int _turnNumber = 1;
	
	//派系列表
	//std::vector<Faction> _factions;

	UnitManager _unitManager{ 100,100 };

	UnitRenderer _unitRenderer;

	WorldUI _ui;
	
	bool _click = false;
	float wheel = 0.0f;
	StageState _state = command;


};