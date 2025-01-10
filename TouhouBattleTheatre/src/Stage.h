#pragma once

#include <vector>

class Stage 
{
public:
	Stage();
	~Stage();
	void Update();
	void Render();
protected:
};

class StageManager
{
public:
	StageManager();
	~StageManager();
	void Update();
	void Render();
	void ChangeStage(Stage* stage);
	void PushStage(Stage* stage);
	void PopStage();
	void ClearStages();
protected:
	std::vector<Stage*> m_stages;
};

class TitleStage : public Stage
{
public:
	TitleStage()
	{
	}
	~TitleStage()
	{
	}
private:



};

class MainStage : public Stage
{
public:
	MainStage()
	{
	}
	~MainStage()
	{
	};


class BattleStage : public Stage
{
public:
	BattleStage()
	{
		_loadBattleMap();
	}
	~BattleStage()
	{
	}
private:
	void _loadBattleMap();
};

