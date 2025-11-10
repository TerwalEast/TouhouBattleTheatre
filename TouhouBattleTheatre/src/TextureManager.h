#pragma once

#include "../TouhouBattleTheatre.h"
#include "TestApplication.h"
#include "TextureLoader.h" // 包含新的头文件

#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <filesystem>

class TextureManager
{
public:
	static TextureManager& GetInstance()
	{
		static TextureManager instance;
		return instance;
	}

	const std::map<std::string, GLuint, std::less<>>& TextureMap() const { return _textureMap; }

	GLuint GetTexture(std::string_view name) const
	{
		if (auto it = _textureMap.find(name); it != _textureMap.end())
		{
			return it->second;
		}
		
		spdlog::error("材质名 {} 未找到", name);
		return 0;
	}

	//Costly
	std::string GetTextureName(GLuint texture) const
	{
		for (const auto& [name, tex] : _textureMap)
		{
			if (tex == texture)
			{
				return name;
			}
		}
		return "";
	}

	void LoadTexture(std::string_view path, std::string_view name)
	{
		if (_textureMap.contains(name)) return;
		
		// 使用新的加载器
		GLuint texture = TextureLoader::Load(path);
		if (texture)
			_textureMap.emplace(name, texture);
		else
			spdlog::error("材质管理器加载材质失败，材质相对路径为 {}", path);
	}

	void LoadTileAtlas(std::string_view filePath, const int tileWidth, const int tileHeight, std::string_view name)
	{
		if (_textureMap.contains(name)) return;

		// 使用新的加载器
		GLuint texture = TextureLoader::LoadTileAtlas(filePath, tileWidth, tileHeight);
		if (texture)
			_textureMap.emplace(name, texture);
		else
			spdlog::error("材质管理器加载材质合集失败，材质合集名为 {}", filePath);
	}

private:
	TextureManager() = default;
	~TextureManager() 
	{
		// 管理器析构时，应释放所有它管理的纹理
		for (const auto& [name, texture] : _textureMap)
		{
			glDeleteTextures(1, &texture);
		}
		_textureMap.clear();
	}
	TextureManager(const TextureManager&) = delete;
	void operator=(const TextureManager&) = delete;

	std::map<std::string, GLuint, std::less<>> _textureMap;
};