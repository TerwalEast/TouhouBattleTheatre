#include "ShaderManager.h"

#include <fstream>
#include <filesystem>
#include <sstream>
#include <vector>
#include <memory>
#include <string>

ShaderManager& ShaderManager::GetInstance()
{
	static ShaderManager sm;
	return sm;
}

static GLenum ShaderStageFromCode(const std::string& code)
{
	if (code == "vert") return GL_VERTEX_SHADER;
	if (code == "frag") return GL_FRAGMENT_SHADER;
	if (code == "geom") return GL_GEOMETRY_SHADER;
	return GL_NONE;
}

bool ShaderManager::CompileShader(ShaderInfo shaderInfo)
{
	if (_shaderMap.contains(shaderInfo.shaderCode))
	{
		spdlog::info("着色器 {} 已经初始化过了。请检查资源名。", shaderInfo.shaderCode);
		return true;
	}

	GLint success = GL_FALSE;

	std::vector<std::string> compileList = { "vert", "frag" };
	if (shaderInfo.hasGeom)
		compileList.push_back("geom");

	GLuint shaderProgramID = glCreateProgram();
	if (shaderProgramID == 0)
	{
		spdlog::warn("Failed to create shader program from OpenGL.");
		return false;
	}

	std::vector<GLuint> shaderIDList;
	shaderIDList.reserve(compileList.size());

	// Base path once
	std::filesystem::path shaderBase = std::filesystem::path("res/shader");

	for (const auto& code : compileList)
	{
		std::filesystem::path filePath = shaderBase / code / (shaderInfo.shaderCode + "." + code);
		//spdlog::debug("Path is {}", filePath.string());

		std::ifstream file(filePath, std::ios::binary);
		if (!file.is_open())
		{
			//spdlog::warn("File {} failed to load!", filePath.string());
			glDeleteProgram(shaderProgramID);
			return false;
		}

		std::stringstream inputStream;
		inputStream << file.rdbuf();
		std::string glslCodeString = inputStream.str();
		const GLchar* glslCode = glslCodeString.c_str();

		GLenum stage = ShaderStageFromCode(code);
		if (stage == GL_NONE)
		{
			spdlog::warn("Unknown shader stage for code [{}]", code);
			glDeleteProgram(shaderProgramID);
			return false;
		}

		GLuint shaderID = glCreateShader(stage);
		if (shaderID == 0)
		{
			spdlog::warn("Failed to allocate resource for shader! Path is {}", filePath.string());
			glDeleteProgram(shaderProgramID);
			return false;
		}

		glShaderSource(shaderID, 1, &glslCode, NULL);
		glCompileShader(shaderID);

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLint logLen = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLen);
			std::string infoLog;
			if (logLen > 0)
			{
				infoLog.resize(static_cast<size_t>(logLen));
				GLsizei received = 0;
				glGetShaderInfoLog(shaderID, logLen, &received, infoLog.data());
				if (received < logLen)
					infoLog.resize(static_cast<size_t>(received));
			}
			spdlog::warn("Shader [{}] failed to compile (stage {}). Log:\n{}", shaderInfo.shaderCode, code, infoLog);
			glDeleteShader(shaderID);
			glDeleteProgram(shaderProgramID);
			return false;
		}

		glAttachShader(shaderProgramID, shaderID);
		shaderIDList.push_back(shaderID);
	}

	glLinkProgram(shaderProgramID);

	// delete shaders after linking
	for (GLuint shaderID : shaderIDList)
		glDeleteShader(shaderID);

	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLint logLen = 0;
		glGetProgramiv(shaderProgramID, GL_INFO_LOG_LENGTH, &logLen);
		std::string infoLog;
		if (logLen > 0)
		{
			infoLog.resize(static_cast<size_t>(logLen));
			GLsizei received = 0;
			glGetProgramInfoLog(shaderProgramID, logLen, &received, infoLog.data());
			if (received < logLen)
				infoLog.resize(static_cast<size_t>(received));
		}
		spdlog::warn("Shader program [{}] linking Failed. Log:\n{}", shaderInfo.shaderCode, infoLog);
		glDeleteProgram(shaderProgramID);
		return false;
	}

	ShaderProgram shaderProgram;
	shaderProgram.id = shaderProgramID;

	// Active uniforms
	GLint uniformCount = 0;
	GLint uniformBlockCount = 0;
	glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORMS, &uniformCount);
	glGetProgramInterfaceiv(shaderProgramID, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &uniformBlockCount);

	if (uniformCount > 0)
	{
		GLint nameMaxLength = 0;
		glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameMaxLength);
		if (nameMaxLength <= 0) nameMaxLength = 256;
		std::vector<char> uniformNameBuf(static_cast<size_t>(nameMaxLength));

		for (GLint i = 0; i < uniformCount; ++i)
		{
			GLsizei nameLength = 0;
			GLint size = 0;
			GLenum type = GL_NONE;
			glGetActiveUniform(shaderProgramID, static_cast<GLuint>(i), nameMaxLength, &nameLength, &size, &type, uniformNameBuf.data());

			std::string uniformName(uniformNameBuf.data(), static_cast<size_t>(nameLength));
			UniformInfo uniformInfo = {};
			uniformInfo.location = glGetUniformLocation(shaderProgramID, uniformName.c_str());
			if (uniformInfo.location == -1)
				continue; // built-in or in uniform block

			UniformType uniformType;
			switch (type)
			{
			case GL_FLOAT:            uniformType = UniformType::FLOAT; break;
			case GL_FLOAT_VEC2:       uniformType = UniformType::VEC2; break;
			case GL_FLOAT_VEC3:       uniformType = UniformType::VEC3; break;
			case GL_FLOAT_VEC4:       uniformType = UniformType::VEC4; break;
			case GL_INT:              uniformType = UniformType::INT; break;
			case GL_INT_VEC2:         uniformType = UniformType::IVEC2; break;
			case GL_INT_VEC3:         uniformType = UniformType::IVEC3; break;
			case GL_INT_VEC4:         uniformType = UniformType::IVEC4; break;
			case GL_BOOL:             uniformType = UniformType::BOOL; break;
			case GL_BOOL_VEC2:        uniformType = UniformType::BVEC2; break;
			case GL_BOOL_VEC3:        uniformType = UniformType::BVEC3; break;
			case GL_BOOL_VEC4:        uniformType = UniformType::BVEC4; break;
			case GL_FLOAT_MAT2:       uniformType = UniformType::MAT2; break;
			case GL_FLOAT_MAT3:       uniformType = UniformType::MAT3; break;
			case GL_FLOAT_MAT4:       uniformType = UniformType::MAT4; break;
			case GL_SAMPLER_2D:       uniformType = UniformType::SAMPLER2D; break;
			case GL_SAMPLER_CUBE:     uniformType = UniformType::SAMPLERCUBE; break;
			case GL_UNSIGNED_INT:     uniformType = UniformType::UINT; break;
			case GL_UNSIGNED_INT_VEC2:uniformType = UniformType::UVEC2; break;
			case GL_UNSIGNED_INT_VEC3:uniformType = UniformType::UVEC3; break;
			case GL_UNSIGNED_INT_VEC4:uniformType = UniformType::UVEC4; break;
			case GL_SAMPLER_1D:       uniformType = UniformType::SAMPLER1D; break;
			case GL_SAMPLER_3D:       uniformType = UniformType::SAMPLER3D; break;
			case GL_SAMPLER_2D_ARRAY: uniformType = UniformType::SAMPLER2DARRAY; break;
			default:                  uniformType = UniformType::UNKNOWN; break;
			}

			uniformInfo.type = uniformType;
			shaderProgram.uniformMap.emplace(uniformName, uniformInfo);
			//spdlog::debug("Got a uniform. Name:[{}], Shader ID:[{}], Location:[{}], ArraySize: [{}], type: [{}]",uniformName, shaderProgramID, uniformInfo.location, size, (int)uniformType);
		}
	}

	// Active uniform blocks
	if (uniformBlockCount > 0)
	{
		GLint nameMaxLengthUB = 0;
		glGetProgramiv(shaderProgramID, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &nameMaxLengthUB);
		if (nameMaxLengthUB <= 0) nameMaxLengthUB = 256;
		std::vector<char> ubNameBuf(static_cast<size_t>(nameMaxLengthUB));

		for (GLuint i = 0; i < static_cast<GLuint>(uniformBlockCount); ++i)
		{
			GLsizei nameLength = 0;
			glGetActiveUniformBlockName(shaderProgramID, i, nameMaxLengthUB, &nameLength, ubNameBuf.data());
			std::string ubName(ubNameBuf.data(), static_cast<size_t>(nameLength));
			//spdlog::debug("Got a uniform block. Name:[{}], Shader ID:[{}]", ubName, shaderProgramID);

			if (!_uniformBlockMap.contains(ubName))
			{
				GLuint uniformBlockIndex = glGetUniformBlockIndex(shaderProgramID, ubName.c_str());
				GLuint bindingPoint = static_cast<GLuint>(_uniformBlockMap.size());
				glUniformBlockBinding(shaderProgramID, uniformBlockIndex, bindingPoint);

				GLint blockSize = 0;
				glGetActiveUniformBlockiv(shaderProgramID, i, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);
				//spdlog::debug("New uniform block: [{}], assigned to binding point [{}], block size is [{}]",ubName, bindingPoint, blockSize);

				GLuint uniformBufferID = 0;
				glCreateBuffers(1, &uniformBufferID);
				glNamedBufferStorage(uniformBufferID, static_cast<GLsizeiptr>(blockSize), NULL, GL_DYNAMIC_STORAGE_BIT);
				glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uniformBufferID);

				UniformBlockInfo uniformBlockInfo = {};
				uniformBlockInfo.bufferID = uniformBufferID;
				uniformBlockInfo.bindingPoint = bindingPoint;
				uniformBlockInfo.size = blockSize;
				_uniformBlockMap.emplace(ubName, uniformBlockInfo);
			}
			else
			{
				spdlog::debug("Uniform块 [{}] 已经存在。", ubName);
			}
		}
	}

	_shaderMap.emplace(std::make_pair(shaderInfo.shaderCode, std::move(shaderProgram)));
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

const ShaderProgram ShaderManager::GetShaderProgram(const std::string& shaderName)
{
	if (_shaderMap.find(shaderName) != _shaderMap.end())
	{
		return _shaderMap[shaderName];
	}
	else
	{
		spdlog::warn("Shader program [{}] not found!", shaderName);
		return ShaderProgram{};
	}
}

void ShaderManager::_initShaderInfoList()
{
	std::filesystem::path path = std::filesystem::path("res/shader/vert");

	for (auto const& entry : std::filesystem::directory_iterator(path))
	{
		std::string code = entry.path().filename().stem().string();
		spdlog::debug("扫描到了着色器名: [{}]", code);
		ShaderInfo shaderInfo;
		shaderInfo.shaderCode = code;
		std::filesystem::path geomPath = std::filesystem::path("res/shader/geom");
		geomPath /= code + ".geom";
		if (std::filesystem::exists(geomPath))
			shaderInfo.hasGeom = true;
		_shaderInfoList.push_back(shaderInfo);
		spdlog::debug("Shader code [{}] added to list. {} geom", code, shaderInfo.hasGeom);
	}
	for (auto& shaderInfo : _shaderInfoList)
	{
		CompileShader(shaderInfo);
	}
	for (auto const& shaderInfo : _shaderMap)
	{
		spdlog::debug("Code: [{}], id: [{}]", shaderInfo.first, shaderInfo.second.id);
		for (auto const& uniform : shaderInfo.second.uniformMap)
		{
			spdlog::debug("uniform name: [{}], loc: [{}], type:[{}]", uniform.first, uniform.second.location, (int)uniform.second.type);
		}
	}
	for (auto const& block : _uniformBlockMap)
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