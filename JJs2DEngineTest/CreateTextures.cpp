#include "pch.h"
#include "CreateTextures.h"

#include "MainDataCollection.h"

#include <Main.h>

#include <stb/stb_image.h>

#include <vector>

void CreateBackgroundTexture(MainDataCollection& data);
void CreateObjectTexture(MainDataCollection& data);

void CreateTextures(MainDataCollection& data)
{
	CreateBackgroundTexture(data);
	CreateObjectTexture(data);
}

void CreateBackgroundTexture(MainDataCollection& data)
{
	auto& main = *data.main;

	std::vector<unsigned char> filedata;

	std::string filename;

	if (data.is16Bit)
	{
		filename = "BackgroundTexture16BitRGB.png";
	}
	else if (data.isRBReversed)
	{
		filename = "BackgroundTexture8BitBGR.png";
	}
	else
	{
		filename = "BackgroundTexture8BitRGB.png";
	}

	std::string filepath = "Textures\\" + filename;

	uint32_t width = 0;
	uint32_t height = 0;
	int channels = 0;

	if (data.is16Bit)
	{
		stbi_us* pixels = stbi_load_16(filepath.c_str(), reinterpret_cast<int32_t*>(&width), reinterpret_cast<int32_t*>(&height), &channels, STBI_rgb_alpha);
		if (pixels == nullptr)
			throw std::runtime_error("CreateBackgroundTexture Error: Program failed to load a 16 bit texture!");

		size_t dataSize = static_cast<size_t>(width) * height;
		dataSize *= 8;
		filedata.resize(dataSize);

		std::memcpy(filedata.data(), pixels, dataSize);
	}
	else
	{
		stbi_uc* pixels = stbi_load(filepath.c_str(), reinterpret_cast<int32_t*>(&width), reinterpret_cast<int32_t*>(&height), &channels, STBI_rgb_alpha);
		if (pixels == nullptr)
			throw std::runtime_error("CreateBackgroundTexture Error: Program failed to load a 8 bit texture!");

		size_t dataSize = static_cast<size_t>(width) * height;
		dataSize *= 4;
		filedata.resize(dataSize);

		std::memcpy(filedata.data(), pixels, dataSize);
	}

	auto resultData = main.TryToAddTextureToStreamedTexturesTransferList(filedata, width, height);

	if (!resultData.has_value())
		throw std::runtime_error("CreateBackgroundTexture Error: Program failed to add the texture to the transfer order list!");

	data.backgroundtexturesID = resultData.value();
}

void CreateObjectTexture(MainDataCollection& data)
{
	auto& main = *data.main;

	std::vector<unsigned char> filedata;

	std::string filename;

	if (data.is16Bit)
	{
		filename = "Texture16BitRGB.png";
	}
	else if (data.isRBReversed)
	{
		filename = "Texture8BitBGR.png";
	}
	else
	{
		filename = "Texture8BitRGB.png";
	}

	std::string filepath = "Textures\\" + filename;

	uint32_t width = 0;
	uint32_t height = 0;
	int channels = 0;

	if (data.is16Bit)
	{
		stbi_us* pixels = stbi_load_16(filepath.c_str(), reinterpret_cast<int32_t*>(&width), reinterpret_cast<int32_t*>(&height), &channels, STBI_rgb_alpha);
		if (pixels == nullptr)
			throw std::runtime_error("CreateObjectTexture Error: Program failed to load a 16 bit texture!");

		size_t dataSize = static_cast<size_t>(width) * height;
		dataSize *= 8;
		filedata.resize(dataSize);

		std::memcpy(filedata.data(), pixels, dataSize);
	}
	else
	{
		stbi_uc* pixels = stbi_load(filepath.c_str(), reinterpret_cast<int32_t*>(&width), reinterpret_cast<int32_t*>(&height), &channels, STBI_rgb_alpha);
		if (pixels == nullptr)
			throw std::runtime_error("CreateObjectTexture Error: Program failed to load a 8 bit texture!");

		size_t dataSize = static_cast<size_t>(width) * height;
		dataSize *= 4;
		filedata.resize(dataSize);

		std::memcpy(filedata.data(), pixels, dataSize);
	}

	auto resultData = main.TryToAddTextureToStreamedTexturesTransferList(filedata, width, height);

	if (!resultData.has_value())
		throw std::runtime_error("CreateObjectTexture Error: Program failed to add the texture to the transfer order list!");

	data.texturesID = resultData.value();
}
