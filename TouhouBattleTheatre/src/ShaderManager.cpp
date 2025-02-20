#include "ShaderManager.h"

#include <fstream>
#include <filesystem>

ShaderManager& ShaderManager::GetInstance()
{
	static ShaderManager sm;
	return sm;
}

bool ShaderManager::CompileShader(ShaderInfo shaderInfo)
{
    
    if (_shaderMap.contains(shaderInfo.shaderCode)) 
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
        strcat_s(path, TestApplication::GetInstance().BasePath.c_str());
        strcat_s(path, "shader/");
        strcat_s(path, code);
        strcat_s(path, "/");
        strcat_s(path, shaderInfo.shaderCode.c_str());
        strcat_s(path, ".");
        strcat_s(path, code);
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
            spdlog::warn("Shader [{}] failed to compile. Cause is: {}",shaderInfo.shaderCode, path);
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
    /*for (auto uniformName : shaderInfo.uniformList) 
    {
        GLuint uniformID = glGetUniformLocation(shaderProgramID, uniformName.c_str());
        if (uniformID == -1)
        {
            spdlog::warn("Failed to acquire unifrom [{}] from shader [{}]", uniformName, shaderInfo.shaderCode);
            continue;
        }
        shaderProgram.uniformMap[uniformName] = uniformID;
    }*/
    

    GLint uniformCount = 0;
    GLint uniformBlockCount = 0;
    glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORMS, &uniformCount);
    glGetProgramInterfaceiv(shaderProgramID, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &uniformBlockCount);

    if (uniformCount != 0)
    {
        GLint 	nameMaxLength = 0;
        GLsizei nameLength = 0;
        GLsizei size = 0;
        GLenum 	type = GL_NONE;
        glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);

        auto uniformName = std::make_unique<char[]>(nameMaxLength);

        for (GLint i = 0; i < uniformCount; ++i)
        {
            glGetActiveUniform(shaderProgramID, i, nameMaxLength, &nameLength, &size, &type, uniformName.get());

            UniformInfo uniformInfo = {};
            uniformInfo.location = glGetUniformLocation(shaderProgramID, uniformName.get());
            if(uniformInfo.location == -1)
				continue; // -1 代表内建或者uniform块

            UniformType uniformType;
            switch (type) 
            {
			case GL_FLOAT:
				uniformType = UniformType::FLOAT;
				break;
			case GL_FLOAT_VEC2:
				uniformType = UniformType::VEC2;
				break;
			case GL_FLOAT_VEC3:
				uniformType = UniformType::VEC3;
				break;
			case GL_FLOAT_VEC4:
				uniformType = UniformType::VEC4;
				break;
			case GL_INT:
				uniformType = UniformType::INT;
				break;
			case GL_INT_VEC2:
				uniformType = UniformType::IVEC2;
				break;
			case GL_INT_VEC3:
				uniformType = UniformType::IVEC3;
				break;
			case GL_INT_VEC4:
				uniformType = UniformType::IVEC4;
				break;
			case GL_BOOL:
				uniformType = UniformType::BOOL;
				break;
			case GL_BOOL_VEC2:
				uniformType = UniformType::BVEC2;
				break;
			case GL_BOOL_VEC3:
				uniformType = UniformType::BVEC3;
				break;
			case GL_BOOL_VEC4:
				uniformType = UniformType::BVEC4;
				break;
			case GL_FLOAT_MAT2:
				uniformType = UniformType::MAT2;
				break;
			case GL_FLOAT_MAT3:
				uniformType = UniformType::MAT3;
				break;
			case GL_FLOAT_MAT4:
				uniformType = UniformType::MAT4;
				break;
			case GL_SAMPLER_2D:
				uniformType = UniformType::SAMPLER2D;
				break;
			case GL_SAMPLER_CUBE:
				uniformType = UniformType::SAMPLERCUBE;
				break;
			case GL_UNSIGNED_INT:
				uniformType = UniformType::UINT;
				break;
			case GL_UNSIGNED_INT_VEC2:
				uniformType = UniformType::UVEC2;
				break;
			case GL_UNSIGNED_INT_VEC3:
				uniformType = UniformType::UVEC3;
				break;
			case GL_UNSIGNED_INT_VEC4:
				uniformType = UniformType::UVEC4;
				break;
			case GL_SAMPLER_1D:
				uniformType = UniformType::SAMPLER1D;
				break;
			case GL_SAMPLER_3D:
				uniformType = UniformType::SAMPLER3D;
				break;
			case GL_SAMPLER_2D_ARRAY:
				uniformType = UniformType::SAMPLER2DARRAY;
				break;
			default:
				uniformType = UniformType::UNKNOWN;
				break;
            }


            uniformInfo.type = uniformType;
            shaderProgram.uniformMap.emplace(std::make_pair(
                std::string(uniformName.get(), nameLength), uniformInfo));
            spdlog::debug("Got a uniform. Name:[{}], Shader ID:[{}], Location:[{}], ArraySize: [{}], type: [{}] "
                , uniformName.get(), shaderProgramID, uniformInfo.location, size, (int)uniformType);
            
        }

        //spdlog::warn("Uniform Block Count = {}", uniformBlockCount);
        if (uniformBlockCount != 0)
        {
            for (GLuint i = 0; i < uniformBlockCount; i++)
            {
                glGetActiveUniformBlockName(shaderProgramID, i, nameMaxLength, &nameLength, uniformName.get());
                spdlog::debug("Got a uniform block. Name:[{}], Shader ID:[{}]", 
                    uniformName.get(), shaderProgramID);
                if (!_uniformBlockMap.contains(uniformName.get())) // 如果没有注册这个uniform block
                {
                    GLuint uniformBlockIndex = glGetUniformBlockIndex(shaderProgramID, uniformName.get());
                    GLuint bindingPoint = _uniformBlockMap.size();
                    glUniformBlockBinding(shaderProgramID, uniformBlockIndex, bindingPoint);

                    GLint blockSize;
                    glGetActiveUniformBlockiv(shaderProgramID, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
                    spdlog::debug("New uniform block: [{}], assigned to binding point [{}], block size is [{}]", 
                        uniformName.get(), bindingPoint, blockSize);

                    GLuint uniformBufferID;
                    glCreateBuffers(1, &uniformBufferID);
                    glNamedBufferStorage(uniformBufferID, blockSize, NULL, GL_DYNAMIC_STORAGE_BIT);
                    glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uniformBufferID);

                    UniformBlockInfo uniformBlockInfo = {};
					uniformBlockInfo.bufferID = uniformBufferID;
                    uniformBlockInfo.bindingPoint = bindingPoint;
                    uniformBlockInfo.size = blockSize;
                    _uniformBlockMap.emplace(std::make_pair(
                        std::string(uniformName.get(), nameLength), uniformBlockInfo));

                }//if ends
                else
                {
                    spdlog::debug("Uniform block [{}] already exists in uniform block map", uniformName.get());

                }

            }
        }
            

    }

    _shaderMap.emplace(std::make_pair(
        shaderInfo.shaderCode, shaderProgram));








    return true;
}

bool ShaderManager::SetUniform(const std::string& shaderName, const std::string& uniformName, UniformType type, const void* value)
{
	if (!_shaderMap.contains(shaderName))
	{
		spdlog::warn("Shader [{}] not found in shader map", shaderName);
		return false;
	}
	auto& shaderProgram = _shaderMap[shaderName];
    if (!shaderProgram.uniformMap.contains(uniformName))
    {
        spdlog::warn("Uniform [{}] not found in shader [{}]", uniformName, shaderName);
        return false;
    }
	auto& uniformInfo = shaderProgram.uniformMap[uniformName];
	GLint location = uniformInfo.location;
	switch (type)
	{
	case UniformType::FLOAT:
		glProgramUniform1f(shaderProgram.id, location, *(GLfloat*)value);
		break;
	case UniformType::VEC2:
		glProgramUniform2fv(shaderProgram.id, location, 1, (GLfloat*)value);
		break;
	case UniformType::VEC3:
		glProgramUniform3fv(shaderProgram.id, location, 1, (GLfloat*)value);
		break;
	case UniformType::VEC4:
		glProgramUniform4fv(shaderProgram.id, location, 1, (GLfloat*)value);
		break;
	case UniformType::MAT2:
		glProgramUniformMatrix2fv(shaderProgram.id, location, 1, GL_FALSE, (GLfloat*)value);
		break;
	case UniformType::MAT3:
		glProgramUniformMatrix3fv(shaderProgram.id, location, 1, GL_FALSE, (GLfloat*)value);
		break;
	case UniformType::MAT4:
		glProgramUniformMatrix4fv(shaderProgram.id, location, 1, GL_FALSE, (GLfloat*)value);
		break;
	case UniformType::INT:
		glProgramUniform1i(shaderProgram.id, location, *(GLint*)value);
		break;
	case UniformType::IVEC2:
		glProgramUniform2iv(shaderProgram.id, location, 1, (GLint*)value);
		break;
	case UniformType::IVEC3:
		glProgramUniform3iv(shaderProgram.id, location, 1, (GLint*)value);
		break;
	case UniformType::IVEC4:
		glProgramUniform4iv(shaderProgram.id, location, 1, (GLint*)value);
		break;
	case UniformType::UINT:
		glProgramUniform1ui(shaderProgram.id, location, *(GLuint*)value);
		break;
	case UniformType::UVEC2:
		glProgramUniform2uiv(shaderProgram.id, location, 1, (GLuint*)value);
		break;
	case UniformType::UVEC3:
		glProgramUniform3uiv(shaderProgram.id, location, 1, (GLuint*)value);
		break;
	case UniformType::UVEC4:
		glProgramUniform4uiv(shaderProgram.id, location, 1, (GLuint*)value);
		break;
	case UniformType::BOOL:
		glProgramUniform1i(shaderProgram.id, location, *(GLint*)value);
		break;
	case UniformType::BVEC2:
		glProgramUniform2iv(shaderProgram.id, location, 1, (GLint*)value);
		break;
	case UniformType::BVEC3:
		glProgramUniform3iv(shaderProgram.id, location, 1, (GLint*)value);
		break;
	case UniformType::BVEC4:
		glProgramUniform4iv(shaderProgram.id, location, 1, (GLint*)value);
		break;
	case UniformType::SAMPLER1D:
	case UniformType::SAMPLER2D:
	case UniformType::SAMPLER3D:
	case UniformType::SAMPLERCUBE:
		glProgramUniform1i(shaderProgram.id, location, *(GLint*)value);
		break;
	default:
		spdlog::warn("Unknown uniform type");
		break;
	}
	return true;
}

bool ShaderManager::SetUniformBlock(const std::string& uniformBlockName, const void* value)
{
	if (!_uniformBlockMap.contains(uniformBlockName))
	{
		spdlog::warn("Uniform block [{}] not found in uniform block map", uniformBlockName);
		return false;
	}
	auto& uniformBlockInfo = _uniformBlockMap[uniformBlockName];
	glNamedBufferSubData(uniformBlockInfo.bufferID, 0, uniformBlockInfo.size, value);
    return true;
}




void ShaderManager::_initShaderInfoList()
{
	std::string path = TestApplication::GetInstance().BasePath + "shader/vert";
    for (auto const& entry : std::filesystem::directory_iterator(path)) 
    {
        std::string code = entry.path().filename().stem().string();
        spdlog::debug("Scanned shader code: [{}]", code);
		ShaderInfo shaderInfo;
		shaderInfo.shaderCode = code;
		std::string geomPath = TestApplication::GetInstance().BasePath + "shader/geom/" + code + ".geom";
		if(std::filesystem::exists(geomPath))
			shaderInfo.hasGeom = true;
		_shaderInfoList.push_back(shaderInfo);
		spdlog::debug("Shader code [{}] added to list. {} geom", code, shaderInfo.hasGeom);
    }
    for (auto shaderInfo : _shaderInfoList) 
    {
		CompileShader(shaderInfo);
    }
    for (auto shaderInfo : _shaderMap) 
    {
        spdlog::debug("Code: [{}], id: [{}]", shaderInfo.first, shaderInfo.second.id);
        for (auto uniform : shaderInfo.second.uniformMap) 
        {
            spdlog::debug("uniform name: [{}], loc: [{}], type:[{}]", uniform.first, uniform.second.location, (int)uniform.second.type);
        }
    }
    for (auto block : _uniformBlockMap) 
    {
        spdlog::debug("Code: [{}], binding: [{}], size: [{}], bufferId: [{}]",
            block.first,
            block.second.bindingPoint,
            block.second.size,
            block.second.bufferID);
    }


}




ShaderManager::ShaderManager() 
{
	_initShaderInfoList();
}
