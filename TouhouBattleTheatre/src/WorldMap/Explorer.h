#pragma once

#include "Actor.h"



/***
* Explorer
* Ì½Ë÷Õß¡£
*/
class Explorer : public Actor
{
public:
	Explorer();
	~Explorer();
	void Update(const float delta);
	void Render();
	void HandleInput();
	
private:
	unsigned int _remainingActionPoints;
	unsigned int _maxActionPoints;
	bool _showDetail = false;
	Actor_Movement _movement;
	Actor_UI _ui;
};


