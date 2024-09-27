#include "CameraController.h"




class RTSCameraController : public CameraController
{
public:
	RTSCameraController(float screenWidth, float screenHeight, const glm::vec3 &position, const glm::vec3 &lookAt, float fov) :
		_position(position), _lookAt(lookAt), _fov(fov), _screenWidth(screenWidth), _screenHeight(screenHeight), _speed(10.0f), _rotateSpeed(0.1f)
	{
		_camera = std::make_unique<Camera>();
		_updateCamera();
	}
	void Update(float dt) override 
	{
		_updateCamera(); // 此类摄像机不需要随时间更新位置
	}
	void Movement(const glm::vec2 movement) 
	{
		_updateCamera();
	}
	void MoveToNewPosition(const glm::vec3& newLookAt) 
	{ 
		glm::vec3 direction = newLookAt - _lookAt;
		_position += direction;
		_lookAt += direction;
		_updateCamera();
	}
	

private:
	void _updateCamera() 
	{
		_camera->ProjParams(_fov, _screenWidth / _screenHeight, 10.0f, 1000.0f);
		_camera->ViewParams(_position, _lookAt);
		_camera->UploadProjViewToShaderManager();
	}
	void _updateDirection()
	{
		_forward = glm::normalize(_lookAt - _position);
		_right = glm::normalize(glm::cross(_forward, glm::vec3(0, 1, 0)));
	}
	glm::vec3 _position;
	glm::vec3 _lookAt;
	glm::vec3 _forward;
	glm::vec3 _right;
	float _fov;
	float _screenWidth;
	float _screenHeight;
	float _speed;
	float _rotateSpeed;
};