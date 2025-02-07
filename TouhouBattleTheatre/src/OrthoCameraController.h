#pragma once

#include "Camera.h"

class OrthoCameraController
{

public:
	OrthoCameraController(float screenWidth, float screenHeight, const glm::vec3& position, const glm::vec3& lookAt, float zoom = 1.0f) :
		_position(position), _lookAt(lookAt), _viewWidth(screenWidth), _viewHeight(screenHeight), _speed(1.0f), _rotateSpeed(0.1f), _zoom(zoom)
	{
		_camera = std::make_unique<Camera>();
		_up = glm::vec3(0, 0, 1);
		glm::vec3 diff = _lookAt - _position;
		_updateCamera();
		_updateDirection();
	}
	void Update(float dt)
	{
		_updateCamera(); // 此类摄像机不需要随时间更新位置
	}
	void Movement(const glm::vec3& movement)
	{
		glm::vec3 direction = glm::vec3(1.0f, 0.0f, 0.0f) * movement.x + glm::vec3(0.0f, 0.0f, 1.0f) * movement.y;
		glm::vec3 offset = direction * _speed;
		_position += offset;
		_lookAt += offset;
		spdlog::info("Position: {} {} {}", _position.x, _position.y, _position.z);
	}
	glm::vec3 GetPosition() const
	{
		return _position;
	}
	float GetZoom() const {	return _zoom; }
	float GetViewWidth() const { return _viewWidth; }
	float GetViewHeight() const { return _viewHeight; }

private:

	void _updateCamera()
	{
		//glm::vec3 diff = glm::normalize(_lookAt - _position);
		_camera->ProjOrthoParams(_viewWidth, _viewHeight, 10.0f, 1000.0f, _zoom);
		_camera->ViewParamsMirrored(_position, _lookAt, _up);
		_camera->UploadProjViewToShaderManager();
	}
	void _updateDirection()
	{
		//glm::vec3 diff = glm::normalize(glm::vec3
		//(
		//	cos(glm::radians(_pitch)) * sin(glm::radians(_yaw)),
		//	sin(glm::radians(_pitch)),
		//	cos(glm::radians(_pitch)) * cos(glm::radians(_yaw))
		//)
		//);
		//_right = glm::normalize(glm::cross(diff, glm::vec3(0, 1, 0)));
		//_forward = diff;
		//_lookAt = _position + _forward;
	}


	std::unique_ptr<Camera> _camera;
	glm::vec3 _position;
	glm::vec3 _lookAt;
	glm::vec3 _up;

	float _viewWidth;
	float _viewHeight;
	float _speed;
	float _rotateSpeed;
	float _zoom;

};
