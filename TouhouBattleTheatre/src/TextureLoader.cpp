#include "TextureLoader.h"

#include <stb_image.h>
#include <vector>

namespace TextureLoader
{

GLuint Load(std::string_view filePath)
{
	spdlog::info("读取材质文件中，相对路径为 {} ", filePath.data());
	stbi_set_flip_vertically_on_load(true);

	std::filesystem::path path = std::filesystem::path("res") / "texture" / filePath;

	GLuint texture{};
	GLsizei width, height, nrChannels;
	stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		GLenum internalFormat = 0, dataFormat = 0;
		if (nrChannels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (nrChannels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}

		if (internalFormat != 0)
		{
			glTextureStorage2D(texture, 1, internalFormat, width, height);
			glTextureSubImage2D(texture, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
		}
		else
		{
			spdlog::error("读取材质文件失败，相对路径为 {}, 格式为 {} channels", path.string(), nrChannels);
			glDeleteTextures(1, &texture);
			texture = 0;
		}
	}
	else
	{
		spdlog::error("读取材质文件失败，相对路径为 {}", path.string());
		texture = 0;

	}
	stbi_image_free(data);
	return texture;
}

GLuint LoadTileAtlas(std::string_view filePath, const int tileWidth, const int tileHeight)
{
	spdlog::info("读取材质文件合集中，相对路径为 {} ", filePath.data());
	stbi_set_flip_vertically_on_load(true);
	GLsizei tilesheetWidth, tilesheetHeight, nrChannel;
	std::filesystem::path path = std::filesystem::path("res") / "texture" / filePath;
	stbi_uc* pixels = stbi_load(path.string().c_str(), &tilesheetWidth, &tilesheetHeight, &nrChannel, STBI_rgb_alpha);
	if (!pixels)
	{
		spdlog::error("读取材质文件合集失败，路径为 {}", path.string()); return 0;
	}

	const GLsizei
		tiles_x = tilesheetWidth / tileWidth,
		tiles_y = tilesheetHeight / tileHeight,
		tile_count = tiles_x * tiles_y;

	GLuint tileset;
	glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &tileset);

	glTextureParameteri(tileset, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(tileset, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(tileset, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(tileset, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTextureStorage3D(tileset, 1, GL_RGBA8, tileWidth, tileHeight, tile_count);

	std::vector<stbi_uc> tile_buffer(static_cast<size_t>(tileWidth) * tileHeight * 4);
	for (GLsizei i = 0; i < tile_count; ++i)
	{
		const GLint src_x = (i % tiles_x) * tileWidth;
		const GLint src_y = (i / tiles_x) * tileHeight;

		for (int row = 0; row < tileHeight; ++row)
		{
			const stbi_uc* src_ptr = pixels + (static_cast<size_t>(src_y) + row) * tilesheetWidth * 4 + (static_cast<size_t>(src_x) * 4);
			stbi_uc* dst_ptr = tile_buffer.data() + (static_cast<size_t>(row) * tileWidth * 4);
			memcpy(dst_ptr, src_ptr, static_cast<size_t>(tileWidth) * 4);
		}
		glTextureSubImage3D(tileset, 0, 0, 0, i, tileWidth, tileHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, tile_buffer.data());
	}

	stbi_image_free(pixels);
	return tileset;
}

} // namespace TextureLoader