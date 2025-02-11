#pragma once

#include "WorldMap.h"

#include <vector>

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

	//-1 means out of map
	unsigned int _locationX;
	unsigned int _locationY;

	//unsigned int _remainingActionPoints;
	//unsigned int _maxActionPoints;

	std::string _actorName;
	std::string _actorType;
	std::string _actorDescription;
	std::string _actorPortraitCode;
};