#pragma once


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

	bool _consumeClick = false;
	bool _consumeHover = false;

	void Update(const float delta);
	void Render();
	bool isWithinUIElement(int x, int y);

private:
	WorldUIState _state = inactive;
	WorldUIShape _shape = rectangle;

	float _hoverTime = 0.0f;

};

