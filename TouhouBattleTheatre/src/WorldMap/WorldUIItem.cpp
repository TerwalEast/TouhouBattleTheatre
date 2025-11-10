#include "WorldUIItem.h"

WorldUIItem::WorldUIItem(const glm::vec2& position, const glm::vec2& size)
	: _position(position), _size(size)
{
	//TODO
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
	// TODO
}

bool WorldUIItem::isWithinUIElement(int x, int y)
{
	switch (_shape)
	{
	case rectangle:
		return (x >= _position.x && x <= (_position.x + _size.x) &&
			y >= _position.y && y <= (_position.y + _size.y));
	case circle:
	{
		float radius = _size.x / 2.0f;
		glm::vec2 center = _position + glm::vec2(radius, radius);
		return glm::distance(glm::vec2(x, y), center) <= radius;
	}
	default:
		return false;
	}
}

void WorldUIItem::Click()
{
	if (OnClick)
	{
		OnClick();
	}
}
