#include "UnitRenderer.h"

#include "../TextureLoader.h"
#include "../ShaderManager.h"
#include "UnitManager.h"
#include "WorldUI.h"

#include <glm/gtc/matrix_transform.hpp>

UnitRenderer::UnitRenderer()
{
    _textureAtlas = TextureLoader::Load("red_fairy.png");
    _setupBuffers();
}

UnitRenderer::~UnitRenderer()
{
    if (_vao != 0) glDeleteVertexArrays(1, &_vao);
    if (_vbo != 0) glDeleteBuffers(1, &_vbo);
	if (_textureAtlas != 0) glDeleteTextures(1, &_textureAtlas);
}

void UnitRenderer::_setupBuffers()
{
    glCreateBuffers(1, &_vbo);
    glNamedBufferStorage(_vbo, sizeof(VERTICES), VERTICES, 0); // Removed GL_DYNAMIC_STORAGE_BIT as it's not dynamic

    glCreateVertexArrays(1, &_vao);

    // Bind VBO to the VAO's binding point 0
    glVertexArrayVertexBuffer(_vao, 0, _vbo, 0, VERTEX_STRIDE);

    // Position attribute (location = 0)
    glEnableVertexArrayAttrib(_vao, 0);
    glVertexArrayAttribBinding(_vao, 0, 0);
    glVertexArrayAttribFormat(_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);

    // Texture coordinate attribute (location = 1)
    glEnableVertexArrayAttrib(_vao, 1);
    glVertexArrayAttribBinding(_vao, 1, 0);
    glVertexArrayAttribFormat(_vao, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));

}

void UnitRenderer::RenderUnits(const UnitManager& unitManager/*, const Camera& camera*/)
{
    auto& shaderManager = ShaderManager::GetInstance();
    auto it = shaderManager.ShaderMap().find("actor");
    if (it == shaderManager.ShaderMap().end()) {
        spdlog::error("Shader 'actor' not found in ShaderManager.");
        return;
    }

    glUseProgram(it->second.id);
    glBindVertexArray(_vao);

    // 遍历所有单位并为每个单位发出一个绘制调用
    // 注意：临时测试版，每个单位一次draw call。
    for (const auto& unit : unitManager.GetAllUnits()) 
    {
        glBindTextureUnit(0, _textureAtlas);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(unit.GetX() * TILE_SIZE, 0.0f, unit.GetY() * TILE_SIZE));
        shaderManager.SetUniform("actor", "model", UniformType::MAT4, (void*)&model);
        glDrawArrays(GL_TRIANGLE_FAN, 0, VERTEX_COUNT);
    }

    glBindVertexArray(0);
}