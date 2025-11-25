#include "WorldUIItem.h"
#include <spdlog/spdlog.h>

WorldUIItem::WorldUIItem(const glm::vec2& position, const glm::vec2& size)
	: _position(position), _size(size){}

WorldUIItem::~WorldUIItem(){}

void WorldUIItem::SetClickAction(std::function<void()> action, MouseButton mouseButton)
{
	switch (mouseButton)
	{
	case MouseButton::LEFT:
		OnLeftClick = action;
		break;
	case MouseButton::RIGHT:
		OnRightClick = action;
		break;
	case MouseButton::MIDDLE:
		OnMiddleClick = action;
		break;
	default:
		break;
	}
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

void WorldUIItem::Render() {
	if (!Visible) return;

	// 渲染子节点
	for (const auto& child : _children) 
	{
		child->Render();
	}
}


bool WorldUIItem::IsWithinUIElement(int x, int y) 
{
	if (!Visible) return false;

	glm::vec2 absPos = GetAbsolutePosition();
	return (x >= absPos.x && x <= absPos.x + _size.x &&
		y >= absPos.y && y <= absPos.y + _size.y);

	//TODO 数据驱动的圆形判定
}

void WorldUIItem::AddChild(std::shared_ptr<WorldUIItem> child) 
{
	child->_parent = this;
	_children.push_back(child);
}

void WorldUIItem::RemoveChild(std::shared_ptr<WorldUIItem> child) 
{
	auto it = std::remove_if(_children.begin(), _children.end(),
		[&](const std::shared_ptr<WorldUIItem>& p) {
			return p == child;
		});
	if (it != _children.end()) {
		(*it)->_parent = nullptr;
		_children.erase(it, _children.end());
	}
}

glm::vec2 WorldUIItem::GetAbsolutePosition() const 
{
	if (_parent) 
	{
		return _parent->GetAbsolutePosition() + _position;
	}
	return _position;
}

void WorldUIItem::HandleClick(int x, int y, MouseButton mouseButton)
{
	switch (mouseButton)
	{
	case MouseButton::LEFT:
		if (OnLeftClick)
			OnLeftClick();
		else
			spdlog::debug("WorldUIItem [{}] received left click, but no action assigned.", Name);
		break;
	case MouseButton::RIGHT:
		if (OnRightClick)
			OnRightClick();
		else
			spdlog::debug("WorldUIItem [{}] received right click, but no action assigned.", Name);
		break;
	case MouseButton::MIDDLE:
		if (OnMiddleClick)
			OnMiddleClick();
		else
			spdlog::debug("WorldUIItem [{}] received middle click, but no action assigned.", Name);
		break;
	default:
		break;
	}
}
