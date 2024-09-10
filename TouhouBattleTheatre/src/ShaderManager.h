#pragma once

#include "../TouhouBattleTheatre.h"
#include <map>
#include <string>
#include <vector>
#include <unordered_map>

struct ShaderInfo 
{
    bool hasGeom = false;
    std::string shaderCode;
    std::vector<std::string> uniformList;
};

struct ShaderProgram 
{
	std::unordered_map<std::string, GLint> uniformMap;
	GLuint id = 0;
};

class ShaderManager 
{
public:
    std::unordered_map<std::string, ShaderProgram> shaderMap;
    std::unordered_map<std::string, GLuint> uniformBindingMap;
    static ShaderManager& getInstance();
    bool compileShader(ShaderInfo shaderInfo);
private:
    void _checkCompileErrors(GLuint shaderID)
    {
        GLint success;
        GLchar infoLog[1024];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderID, 1024, NULL, infoLog);
            spdlog::warn("ERROR::SHADER_COMPILATION_ERROR:\n{}", infoLog);
        }
        glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shaderID, 1024, NULL, infoLog);
            spdlog::warn("ERROR::SHADER_LINKING_ERROR:\n{}", infoLog);
        }
    }
};




