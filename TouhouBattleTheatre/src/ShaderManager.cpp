#include "ShaderManager.h"

#include <fstream>
#include <vector>

ShaderManager& ShaderManager::getInstance()
{
	static ShaderManager sm;
	return sm;
}

bool ShaderManager::compileShader(ShaderInfo shaderInfo)
{
    
    if (shaderMap.contains(shaderInfo.shaderCode)) 
    {
        spdlog::info("Shader {} is already inited", shaderInfo.shaderCode);
        return true;
    }


    GLint success;
    
    std::vector compileList = { "vert", "frag" };
    if (shaderInfo.hasGeom)
        compileList.push_back("geom");

    GLuint shaderProgramID = glCreateProgram();
    if (shaderProgramID == 0) 
    {
        spdlog::warn("Failed to create shader program from OpenGL.");
        return false;
    }
    std::vector<GLuint> shaderIDList;
    for (auto code : compileList) 
    {
        std::ifstream file;
        std::string glslCodeString;
        std::stringstream inputStream;
        char path[1024] = "";
        strcat(path, BasePath.c_str());
        strcat(path, "shader/");
        strcat(path, code);
        strcat(path, "/");
        strcat(path, shaderInfo.shaderCode.c_str());
        strcat(path, ".");
        strcat(path, code);
        spdlog::debug("Path is {}", path);
        file.open(path);
        if (!file.is_open())
        {
            spdlog::warn("File {} failed to load!", path);
            glDeleteProgram(shaderProgramID);
            return false;
        }
        inputStream << file.rdbuf();
        file.close();
        
        glslCodeString = inputStream.str();
        inputStream.flush();
        //spdlog::debug("Shader loaded is \n{}", glslCodeString);
        const GLchar* glslCode = glslCodeString.c_str();
        GLuint shaderID;
        //spdlog::debug(code == "vert" ? "GL_VERTEX_SHADER" : (code == "frag" ? "GL_FRAGMENT_SHADER" : "GL_GEOMETRY_SHADER"));

        shaderID = glCreateShader(code == "vert" ? GL_VERTEX_SHADER : 
                                    (code == "frag" ? GL_FRAGMENT_SHADER : GL_GEOMETRY_SHADER) );
        if (shaderID == 0) 
        {
            spdlog::warn("Failed to allocate resource for shade! Path is \n{}", path);
            glDeleteProgram(shaderProgramID);
            return false;
        }
        glShaderSource(shaderID, 1, &glslCode, NULL);
        glCompileShader(shaderID);
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) 
        {
            glGetShaderInfoLog(shaderID, 1024, NULL, path);
            spdlog::warn("Shader [{}] compiling failed. Cause is: {}",shaderInfo.shaderCode, path);
            glDeleteProgram(shaderProgramID);
            glDeleteShader(shaderID);
            return false;
        }
        glAttachShader(shaderProgramID, shaderID);
        shaderIDList.push_back(shaderID);
    }
    glLinkProgram(shaderProgramID);
    for (auto shaderID : shaderIDList)
        glDeleteShader(shaderID);
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[1024];
        glGetProgramInfoLog(shaderProgramID, 1024, NULL, infoLog);
        spdlog::warn("Shader program [{}] linking Failed. Log: {}", shaderInfo.shaderCode, infoLog);
    }

    ShaderProgram shaderProgram;
    shaderProgram.id = shaderProgramID;
    for (auto uniformName : shaderInfo.uniformList) 
    {
        GLuint uniformID = glGetUniformLocation(shaderProgramID, uniformName.c_str());
        if (uniformID == -1)
        {
            spdlog::warn("Failed to acquire unifrom [{}] from shader [{}]", uniformName, shaderInfo.shaderCode);
            continue;
        }
        shaderProgram.uniformMap[uniformName] = uniformID;
    }
    shaderMap[shaderInfo.shaderCode] = shaderProgram;

    //for (auto sp : shaderMap)
    //{
    //    spdlog::warn("sp:{}|{}", sp.first,sp.second.id);
    //    for (auto un : sp.second.uniformMap)
    //        spdlog::warn("un:{}|{}",un.first,un.second);
    //}

    return true;
}
