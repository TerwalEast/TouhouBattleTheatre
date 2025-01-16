#pragma once


#include "Stage.h"
#include "TileMap.h"
#include "../TouhouBattleTheatre.h"



class WorldMap : public Stage
{
public:
	WorldMap();
	~WorldMap();
protected:
	
private:
	TileMap* m_tileMap;
	int turnNumber;
	


};