#include "Camera.h"

class CameraController
{
public:
    CameraController();
    virtual void Update(float dt);
protected:
	std::unique_ptr<Camera> _camera;
};
