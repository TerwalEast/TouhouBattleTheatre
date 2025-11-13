#pragma once

#include <string>
#include <glm/glm.hpp>
#include <functional>

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
	WorldUIItem(const glm::vec2& position, const glm::vec2& size);
	virtual ~WorldUIItem();

	bool ConsumeClick = false;
	bool ConsumeHover = false;
	bool HoverActive = false;

	// 事件回调
	std::function<void()> OnClick;
	void SetClickAction(std::function<void()> action) { OnClick = action; }

	virtual void Update(const float delta);
	virtual void Render() {};
	bool isWithinUIElement(int x, int y);

	// 触发点击事件
	void Click();

protected:
	std::string _name = "Unamed WorldUI";
	WorldUIState _state = inactive;
	WorldUIShape _shape = rectangle;
	float _hoverTime = 0.0f;

	glm::vec2 _position; // UI元素左上角位置
	glm::vec2 _size;     // UI元素尺寸
};

