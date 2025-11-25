#pragma once

#include <glm/glm.hpp>
#include <functional>
#include <vector>
#include <memory>
#include <string>


enum class WorldUIState {
	INACTIVE,
	ACTIVE,
	HOVER
};

enum class WorldUIShape {
	RECT,
	CIRCLE
};

enum class MouseButton {
	LEFT,
	RIGHT,
	MIDDLE
};

class WorldUIItem : public std::enable_shared_from_this<WorldUIItem> {
public:
	WorldUIItem(const glm::vec2& position, const glm::vec2& size);
	virtual ~WorldUIItem();

	bool ConsumeClick = false;
	bool ConsumeHover = false;
	bool HoverActive = false;
	bool Visible = true;

	std::function<void()> OnLeftClick;
	std::function<void()> OnRightClick;
	std::function<void()> OnMiddleClick;

	void SetClickAction(std::function<void()> action, MouseButton mouseButton);

	virtual void Update(const float delta);
	virtual void Render();

	bool IsWithinUIElement(int x, int y);

	// 层级管理
	void AddChild(std::shared_ptr<WorldUIItem> child);
	void RemoveChild(std::shared_ptr<WorldUIItem> child);
	glm::vec2 GetAbsolutePosition() const;

	// 事件处理
	virtual void HandleClick(int x, int y, MouseButton mouseButton);

	std::string Name = "Unamed WorldUI";

protected:
	WorldUIState _state = WorldUIState::INACTIVE;
	WorldUIShape _shape = WorldUIShape::RECT;
	float _hoverTime = 0.0f;
	glm::vec2 _position; // 相对于父节点的坐标
	glm::vec2 _size;

	// 层级结构
	WorldUIItem* _parent = nullptr;
	std::vector<std::shared_ptr<WorldUIItem>> _children;
};

