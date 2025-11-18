#pragma once

#include "../TouhouBattleTheatre.h"
#include "TestApplication.h"

#include <map>
#include <string>
#include <vector>
#include <unordered_map>

enum UniformType 
{
    FLOAT, VEC2, VEC3, VEC4,
    MAT2, MAT3, MAT4,
	INT, IVEC2, IVEC3, IVEC4,
	UINT, UVEC2, UVEC3, UVEC4,
	BOOL, BVEC2, BVEC3, BVEC4,
    SAMPLER1D,SAMPLER2D,SAMPLER3D,SAMPLERCUBE,
	SAMPLER1DARRAY, SAMPLER2DARRAY,
    UNKNOWN
};


struct UniformInfo
{
    GLint location;
    UniformType type;
};

struct UniformBlockInfo
{
    GLint bindingPoint;
    GLsizei size;
    GLint bufferID;
};

struct ShaderInfo 
{
    bool hasGeom = false;
    std::string shaderCode;
};

struct ShaderProgram 
{
	std::unordered_map<std::string, UniformInfo> uniformMap;
	GLuint id = 0;
};

class ShaderManager 
{
public:
    const std::unordered_map<std::string, ShaderProgram>& ShaderMap() { return _shaderMap; };
    static ShaderManager& GetInstance();
    bool CompileShader(ShaderInfo shaderInfo);
    bool SetUniform(const std::string& shaderName, const std::string& uniformName, UniformType type, const void* value);
    bool SetUniformBlock(const std::string& uniformBlockName, const void* value);
    const ShaderProgram GetShaderProgram(const std::string& shaderName);
    
private:
    std::unordered_map<std::string, ShaderProgram> _shaderMap;
    std::vector<ShaderInfo> _shaderInfoList;
    std::unordered_map<std::string, UniformBlockInfo> _uniformBlockMap;
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
	void _initShaderInfoList(); 
    ShaderManager();
    ~ShaderManager() {};
	ShaderManager(ShaderManager const&) = delete;
	void operator=(ShaderManager const&) = delete;
};




