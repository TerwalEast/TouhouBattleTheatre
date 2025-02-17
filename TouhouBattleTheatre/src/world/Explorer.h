#pragma once

#include "Actor.h"


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
	Actor_Movement _movement;
	Actor_UI _ui;
};