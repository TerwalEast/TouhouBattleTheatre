#include "CameraController.h"

enum Movement
{
	
};


class RTSCameraController : public CameraController
{
public:
	RTSCameraController(float screenWidth, float screenHeight, const glm::vec3 &position, const glm::vec3 &lookAt, float fov) :
		_position(position), _lookAt(lookAt), _fov(fov), _screenWidth(screenWidth), _screenHeight(screenHeight), _speed(10.0f), _rotateSpeed(0.1f)
	{
		_camera = std::make_unique<Camera>();
		_updateCamera();
	}
	void Update(float dt) override;
	

private:
	void _updateCamera() 
	{
		_camera->ProjParams(_fov, _screenWidth / _screenHeight, 0.1f, 1000.0f);
		_camera->ViewParams(_position, _lookAt);
		_camera->UploadProjViewToShaderManager();
	}
	glm::vec3 _position;
	glm::vec3 _lookAt;
	float _fov;
	float _screenWidth;
	float _screenHeight;
	float _speed;
	float _rotateSpeed;
};