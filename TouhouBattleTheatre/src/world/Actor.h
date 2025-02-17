#pragma once

#include "WorldMap.h"

#include <vector>



struct Actor_Movement
{
	//-1 means out of map
	unsigned int _locationX;
	unsigned int _locationY;

	unsigned int _remainingActionPoints = 0;
	unsigned int _maxActionPoints = 0; //0 means the actor is passive, will neither move nor act

	bool _moveable = false;
	bool _isFloating = false;
	bool _canFly = false;
	bool _canSwim = false;
	bool _canClimb = false;
	bool _canDig = false;
	bool _canTeleport = false;


};

struct Actor_UI
{
	bool _showFactionColor = false;
	bool _showDescriptionOnHover = false;

};

struct Actor_Interactions_Landmark
{
	bool _canEnter = false;
	bool _hasGarrison = false;
	bool _canOccupy = false;
	bool _canSiege = false;

};



/**
* @brief Base class for all actors in the worldmap
*/
class Actor
{
public:
	Actor() {};
	~Actor() {};
	virtual void Update(const float delta) = 0;
	virtual void Render() = 0;
	virtual void HandleInput() = 0;
private:
	unsigned int _actorID;

	std::string _actorName;
	std::string _actorType;
	std::string _actorDescription;
	std::string _actorPortraitCode;
};