
#include "Camera.h"

class RTSCameraController
{
public:
	RTSCameraController(float screenWidth, float screenHeight, const glm::vec3 &position, const glm::vec3 &lookAt, float fov) :
		_position(position), _lookAt(lookAt), _fov(fov), _viewWidth(screenWidth), _viewHeight(screenHeight), _speed(1.0f), _rotateSpeed(0.1f)
	{
		_camera = std::make_unique<Camera>();
		_up = glm::vec3(0, 1, 0);
		_updateCamera();
		_updateDirection();
	}
	void Update(float dt)
	{
		_updateCamera(); // 此类摄像机不需要随时间更新位置
	}
	void Movement(const glm::vec3& movement) 
	{

		glm::vec3 direction = _right * movement.z + _forward * movement.x + _up * movement.y;
		glm::vec3 offset = direction * _speed;
		_position += offset;
		_lookAt += glm::vec3(offset.x, 0, offset.z);

	}
	void ArcballRotate(float movement) 
	{
		glm::vec3 target = _position - _lookAt;
		glm::mat4 rotateMat = glm::rotate(glm::mat4(1.0f), glm::radians(-0.1f * movement), glm::vec3(0.0f, 1.0f, 0.0f));
		target = rotateMat * glm::vec4(target, 1.0f);
		//target = glm::normalize(target);
		_position = _lookAt + target;
		_updateDirection();
	}
	void MoveToNewPosition(const glm::vec3& newLookAt) 
	{ 
		glm::vec3 direction = newLookAt - _lookAt;
		_position += direction;
		_lookAt += direction;
	}
	
private:
	void _updateCamera() 
	{
		//glm::vec3 diff = glm::normalize(_lookAt - _position);
		_camera->ProjParams(_fov, _viewWidth / _viewHeight, 10.0f, 1000.0f);
		_camera->ViewParams(_position, _lookAt);
		_camera->UploadProjViewToShaderManager();
	}
	void _updateDirection()
	{
		glm::vec3 diff = glm::normalize(_lookAt - _position);
		_right = glm::normalize(glm::cross(diff, glm::vec3(0, 1, 0)));
		_forward = glm::normalize(glm::vec3(diff.x, 0, diff.z));
	}
	std::unique_ptr<Camera> _camera;
	glm::vec3 _position;
	glm::vec3 _lookAt;
	glm::vec3 _forward;
	glm::vec3 _right;
	glm::vec3 _up;
	float _fov;
	float _viewWidth;
	float _viewHeight;
	float _speed;
	float _rotateSpeed;
};