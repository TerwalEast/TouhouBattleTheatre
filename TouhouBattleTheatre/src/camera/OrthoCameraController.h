#pragma once

#include "Camera.h"
#include <glm/gtx/norm.hpp> // For glm::distance2

// 使用常量定义方向向量
static const glm::vec3 RIGHT(1.0f, 0.0f, 0.0f);
static const glm::vec3 UP(0.0f, 0.0f, 1.0f);

class OrthoCameraController
{
public:

	OrthoCameraController(float screenWidth, float screenHeight, const glm::vec3& position, const glm::vec3& lookAt, float zoom = 1.0f) :
		_position(position), 
		_lookAt(lookAt), 
		_viewWidth(screenWidth), 
		_viewHeight(screenHeight), 
		_speed(1.0f), 
		_zoom(zoom),
		_up(0.0f, 0.0f, 1.0f),
		_camera(std::make_unique<Camera>()),
		// 初始化目标状态
		_targetPosition(position),
		_targetLookAt(lookAt),
		_targetZoom(zoom),
		// 设置插值速度
		_moveLerpFactor(5.5f),
		_zoomLerpFactor(5.5f)
	{
		_updateCamera();
	}

	void Update(float dt)
	{
		bool needsUpdate = false;

		// 平滑移动位置
		if (glm::distance2(_position, _targetPosition) > 0.00001f)
		{
			// Lerp
			_position += (_targetPosition - _position) * _moveLerpFactor * dt;
			needsUpdate = true;
		}
		else
		{
			_position = _targetPosition; // 吸附到目标位置，避免抖动
		}

		// 平滑移动视点
		if (glm::distance2(_lookAt, _targetLookAt) > 0.00001f)
		{
			_lookAt += (_targetLookAt - _lookAt) * _moveLerpFactor * dt;
			needsUpdate = true;
		}
		else
		{
			_lookAt = _targetLookAt;
		}

		// 平滑缩放
		if (std::abs(_zoom - _targetZoom) > 0.001f)
		{
			_zoom += (_targetZoom - _zoom) * _zoomLerpFactor * dt;
			needsUpdate = true;
		}
		else
		{
			_zoom = _targetZoom;
		}

		if (needsUpdate)
		{
			_updateCamera();
		}
	}

	void Movement(const glm::vec3& movement)
	{		
		const glm::vec3 direction = RIGHT * movement.x + UP * movement.y;
		const glm::vec3 offset = direction * _speed;
		
		// 更新目标位置
		_targetPosition += offset;
		_targetLookAt += offset;
		
	}

	// Getters
	const glm::vec3& GetPosition() const { return _position; }
	const glm::vec3& GetLookAt() const { return _lookAt; }
	float GetZoom() const { return _zoom; }
	float GetViewWidth() const { return _viewWidth; }
	float GetViewHeight() const { return _viewHeight; }
	float GetSpeed() const { return _speed; }
	const Camera* GetCamera() const { return _camera.get(); }

	// Setters
	void SetViewPort(float width, float height) 
	{
		if (_viewWidth != width || _viewHeight != height)
		{
			_viewWidth = width;
			_viewHeight = height;
			_updateCamera();
		}
	}

	void SetZoom(float zoom)
	{
		// 更新目标缩放值
		_targetZoom = glm::max(0.1f, zoom); // 防止缩放值过小
	}

	void SetSpeed(float speed) { _speed = speed; }

	void SetPosition(const glm::vec3& position)
	{
		const glm::vec3 offset = position - _targetPosition;
		// 更新目标位置和视点
		_targetPosition = position;
		_targetLookAt += offset; // 保持相对视角
	}

private:
	static constexpr float NEAR_PLANE = -10.0f;
	static constexpr float FAR_PLANE = 105.0f;

	void _updateCamera()
	{
		_camera->ProjOrthoParams(_viewWidth, _viewHeight, NEAR_PLANE, FAR_PLANE, _zoom);
		_camera->ViewParams(_position, _lookAt, _up);
		_camera->UploadProjViewToShaderManager();
	}

	// 成员变量
	std::unique_ptr<Camera> _camera;
	
	glm::vec3 _position;
	glm::vec3 _lookAt;
	glm::vec3 _up;

	float _viewWidth;
	float _viewHeight;
	float _speed;
	float _zoom;

	// 用于平滑移动和缩放的目标状态
	glm::vec3 _targetPosition;
	glm::vec3 _targetLookAt;
	float _targetZoom;

	// 插值因子
	float _moveLerpFactor;
	float _zoomLerpFactor;
};
