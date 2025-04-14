#include "WorldUIItem.h"

WorldUIItem::WorldUIItem()
{
}

WorldUIItem::~WorldUIItem()
{
}

void WorldUIItem::Update(const float delta)
{
	if (HoverActive)
	{
		_hoverTime += delta;
		HoverActive = false;
	}
	else
	{
		_hoverTime = 0.0f;
	}

}

void WorldUIItem::Render()
{

}

bool WorldUIItem::isWithinUIElement(int x, int y)
{
	//TODO
	return false;
}
