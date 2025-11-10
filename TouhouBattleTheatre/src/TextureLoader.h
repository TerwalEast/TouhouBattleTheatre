#pragma once

#include "../TouhouBattleTheatre.h"
#include <string_view>
#include <filesystem>

namespace TextureLoader
{
	GLuint Load(std::string_view filePath);
	GLuint LoadTileAtlas(std::string_view filePath, int tileWidth, int tileHeight);
}