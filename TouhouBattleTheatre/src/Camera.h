#pragma once

#include "../TouhouBattleTheatre.h"
#include "TestApplication.h"

class Camera 
{
public:
    Camera();
    void ViewParams(glm::vec3 const& eye_pos, glm::vec3 const& lookat);
    void ViewParams(glm::vec3 const& eye_pos, glm::vec3 const& lookat, glm::vec3 const& up);
    void ProjParams(float fov, float aspect, float near_plane, float far_plane);
    void ProjOrthoParams(float w, float h, float near_plane, float far_plane);

    const glm::mat4& ViewMatrix() const { return _viewMat; }
    const glm::mat4& InverseViewMatrix() const { return _inversedViewMat; }
    const glm::mat4& ProjMatrix() const { return _projMat; }
    const glm::mat4& InverseProjMatrix() const { return _inversedProjMat; }

    const glm::vec3& EyePos() const { return *reinterpret_cast<const glm::vec3*>(&_inversedViewMat[3]); }
    const glm::vec3& RightVec() const { return *reinterpret_cast<const glm::vec3*>(&_inversedViewMat[0]); }
    const glm::vec3& UpVec() const { return *reinterpret_cast<const glm::vec3*>(&_inversedViewMat[1]); }
    glm::vec3 ForwardVec() const { return -(*reinterpret_cast<const glm::vec3*>(&_inversedViewMat[2])); }
    glm::vec3 LookAt() const { return this->EyePos() + this->ForwardVec() * this->LookAtDist(); }

    float LookAtDist() const { return _lookAtDist; }
    float FOV() const { return _fov; }
    float Aspect() const { return _aspect; }
    float NearPlane() const { return _nearPlane; }
    float FarPlane() const { return _farPlane; }

private:
    float        _lookAtDist;
    glm::mat4    _viewMat;
    glm::mat4    _inversedViewMat;   

    float        _fov;
    float        _aspect;
    float        _nearPlane, _farPlane;
    glm::mat4    _projMat;
    glm::mat4    _inversedProjMat;
};