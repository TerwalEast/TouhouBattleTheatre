#pragma once

#include <string>

enum WorldUIState
{
	hot, active, inactive
};

enum WorldUIShape
{
	rectangle, circle
};


class WorldUIItem
{
public:
	WorldUIItem();
	~WorldUIItem();

	bool ConsumeClick = false;
	bool ConsumeHover = false;
	bool HoverActive = false;

	void Update(const float delta);
	void Render();
	bool isWithinUIElement(int x, int y);


private:

	std::string _name = "Unamed WorldUI";

	WorldUIState _state = inactive;
	WorldUIShape _shape = rectangle;

	float _hoverTime = 0.0f;

	

};

