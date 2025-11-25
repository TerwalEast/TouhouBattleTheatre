#pragma once

#include <glad/glad.h>

#include <vector>
#include <memory>

class UnitManager;

class UnitRenderer
{
public:
    UnitRenderer();
    ~UnitRenderer();

    // 禁止拷贝和移动，因为渲染器持有GPU资源
    UnitRenderer(const UnitRenderer&) = delete;
    UnitRenderer& operator=(const UnitRenderer&) = delete;
    UnitRenderer(UnitRenderer&&) = delete;
    UnitRenderer& operator=(UnitRenderer&&) = delete;

    void RenderUnits(const UnitManager& unitManager);

private:

    void _setupBuffers();

    GLuint _textureAtlas = 0;
    GLuint _vao = 0;
    GLuint _vbo = 0;
};