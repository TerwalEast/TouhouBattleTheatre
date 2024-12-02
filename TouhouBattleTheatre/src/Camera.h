#pragma once

#include "../TouhouBattleTheatre.h"
#include "TestApplication.h"
#include "ShaderManager.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera 
{
public:
    Camera(const std::string& uniformBlockName = "Matrices") : _uniformBlockName(uniformBlockName) {}
    void ViewParams(glm::vec3 const& eye_pos, glm::vec3 const& lookat) 
    {
		this->ViewParams(eye_pos, lookat, glm::vec3(0, 1, 0));
    }
    void ViewParams(glm::vec3 const& eye_pos, glm::vec3 const& lookat, glm::vec3 const& up)
    {
		_viewMat = glm::lookAt(eye_pos, lookat, up);
		_inversedViewMat = glm::inverse(_viewMat);
		//_lookAtDist = glm::distance(eye_pos, lookat);
    }
    void ViewParams(float pitch, float yaw, float roll, glm::vec3 const& eye_pos, glm::vec3 const& up)
    {
        glm::vec3 diff = glm::normalize(glm::vec3(
            cos(glm::radians(yaw) * cos(glm::radians(pitch))),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw) * cos(glm::radians(pitch)))
        )
        );
        _viewMat = glm::lookAt(eye_pos, eye_pos + diff, up);
		_inversedViewMat = glm::inverse(_viewMat);
    }
    void ViewParamsQuat(float pitch, float yaw, float roll, glm::vec3 const& eye_pos)
    {
        glm::quat qPitch = glm::angleAxis(glm::radians(pitch), glm::vec3(1, 0, 0));
        glm::quat qYaw = glm::angleAxis(glm::radians(yaw), glm::vec3(0, 1, 0));
        glm::quat qRoll = glm::angleAxis(glm::radians(roll), glm::vec3(0, 0, 1));

        glm::quat orientation = qPitch * qYaw * qRoll;
        orientation = glm::normalize(orientation);
        glm::mat4 rotate = glm::mat4_cast(orientation);

        glm::mat4 translate = glm::mat4(1.0f);
        translate = glm::translate(translate, -eye_pos);

        _viewMat = rotate * translate;
        _inversedViewMat = glm::inverse(_viewMat);
    }
    void ProjParams(float fov, float aspect, float near_plane, float far_plane)
    {
		_fov = fov;
		_aspect = aspect;
		_nearPlane = near_plane;
		_farPlane = far_plane;
		_projMat = glm::perspective(glm::radians(fov), aspect, near_plane, far_plane);
		_inversedProjMat = glm::inverse(_projMat);

    }
    void ProjOrthoParams(float w, float h, float near_plane, float far_plane)
    {
		_fov = 0;
		_aspect = w / h;
		_nearPlane = near_plane;
		_farPlane = far_plane;
		_projMat = glm::ortho(0.0f, w, 0.0f, h, near_plane, far_plane);
		_inversedProjMat = glm::inverse(_projMat);
    }
    void UploadProjViewToShaderManager()
    {
		ShaderManager::GetInstance().SetUniformBlock(_uniformBlockName, (void*)&_projMat);
    }

    const glm::mat4& ViewMatrix() const { return _viewMat; }
    const glm::mat4& InverseViewMatrix() const { return _inversedViewMat; }
    const glm::mat4& ProjMatrix() const { return _projMat; }
    const glm::mat4& InverseProjMatrix() const { return _inversedProjMat; }

    const glm::vec3& EyePos() const { return *reinterpret_cast<const glm::vec3*>(&_inversedViewMat[3]); }
    const glm::vec3& RightVec() const { return *reinterpret_cast<const glm::vec3*>(&_inversedViewMat[0]); }
    const glm::vec3& UpVec() const { return *reinterpret_cast<const glm::vec3*>(&_inversedViewMat[1]); }
    glm::vec3 ForwardVec() const { return -(*reinterpret_cast<const glm::vec3*>(&_inversedViewMat[2])); }
    //glm::vec3 LookAt() const { return this->EyePos() + this->ForwardVec() * this->LookAtDist(); }

    //float LookAtDist() const { return _lookAtDist; }
    float FOV() const { return _fov; }
    float Aspect() const { return _aspect; }
    float NearPlane() const { return _nearPlane; }
    float FarPlane() const { return _farPlane; }

private:
    glm::mat4    _projMat;
    glm::mat4    _viewMat;

    glm::mat4    _inversedViewMat;
    glm::mat4    _inversedProjMat;
     
    //float        _lookAtDist;
    float        _fov;
    float        _aspect;
    float        _nearPlane, _farPlane;
    
    const std::string _uniformBlockName;
};


