#include "pch.h"
#include "CreateTextures.h"

#include "MainDataCollection.h"

#include <Main.h>

#include <stb/stb_image.h>

#include <vector>

void CreateBackgroundTexture(MainDataCollection& data);
void CreateObjectTexture(MainDataCollection& data);
void CreateObjectTextureSecondFrame(MainDataCollection& data);
void CreateObjectTextureThirdFrame(MainDataCollection& data);

void CreateTextures(MainDataCollection& data)
{
	CreateBackgroundTexture(data);
	CreateObjectTexture(data);
	CreateObjectTextureSecondFrame(data);
	CreateObjectTextureThirdFrame(data);
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

		stbi_image_free(pixels);
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

		stbi_image_free(pixels);
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

		stbi_image_free(pixels);
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

		stbi_image_free(pixels);
	}

	auto resultData = main.TryToAddTextureToStreamedTexturesTransferList(filedata, width, height);

	if (!resultData.has_value())
		throw std::runtime_error("CreateObjectTexture Error: Program failed to add the texture to the transfer order list!");

	data.texturesIDs[0] = resultData.value();
}

void CreateObjectTextureSecondFrame(MainDataCollection& data)
{
	auto& main = *data.main;

	std::vector<unsigned char> filedata;

	std::string filename;

	if (data.is16Bit)
	{
		filename = "TextureSecondFrame16BitRGB.png";
	}
	else if (data.isRBReversed)
	{
		filename = "TextureSecondFrame8BitBGR.png";
	}
	else
	{
		filename = "TextureSecondFrame8BitRGB.png";
	}

	std::string filepath = "Textures\\" + filename;

	uint32_t width = 0;
	uint32_t height = 0;
	int channels = 0;

	if (data.is16Bit)
	{
		stbi_us* pixels = stbi_load_16(filepath.c_str(), reinterpret_cast<int32_t*>(&width), reinterpret_cast<int32_t*>(&height), &channels, STBI_rgb_alpha);
		if (pixels == nullptr)
			throw std::runtime_error("CreateObjectTextureSecondFrame Error: Program failed to load a 16 bit texture!");

		size_t dataSize = static_cast<size_t>(width) * height;
		dataSize *= 8;
		filedata.resize(dataSize);

		std::memcpy(filedata.data(), pixels, dataSize);

		stbi_image_free(pixels);
	}
	else
	{
		stbi_uc* pixels = stbi_load(filepath.c_str(), reinterpret_cast<int32_t*>(&width), reinterpret_cast<int32_t*>(&height), &channels, STBI_rgb_alpha);
		if (pixels == nullptr)
			throw std::runtime_error("CreateObjectTextureSecondFrame Error: Program failed to load a 8 bit texture!");

		size_t dataSize = static_cast<size_t>(width) * height;
		dataSize *= 4;
		filedata.resize(dataSize);

		std::memcpy(filedata.data(), pixels, dataSize);

		stbi_image_free(pixels);
	}

	auto resultData = main.TryToAddTextureToStreamedTexturesTransferList(filedata, width, height);

	if (!resultData.has_value())
		throw std::runtime_error("CreateObjectTextureSecondFrame Error: Program failed to add the texture to the transfer order list!");

	data.texturesIDs[1] = resultData.value();
}

void CreateObjectTextureThirdFrame(MainDataCollection& data)
{
	auto& main = *data.main;

	std::vector<unsigned char> filedata;

	std::string filename;

	if (data.is16Bit)
	{
		filename = "TextureThirdFrame16BitRGB.png";
	}
	else if (data.isRBReversed)
	{
		filename = "TextureThirdFrame8BitBGR.png";
	}
	else
	{
		filename = "TextureThirdFrame8BitRGB.png";
	}

	std::string filepath = "Textures\\" + filename;

	uint32_t width = 0;
	uint32_t height = 0;
	int channels = 0;

	if (data.is16Bit)
	{
		stbi_us* pixels = stbi_load_16(filepath.c_str(), reinterpret_cast<int32_t*>(&width), reinterpret_cast<int32_t*>(&height), &channels, STBI_rgb_alpha);
		if (pixels == nullptr)
			throw std::runtime_error("CreateObjectTextureThirdFrame Error: Program failed to load a 16 bit texture!");

		size_t dataSize = static_cast<size_t>(width) * height;
		dataSize *= 8;
		filedata.resize(dataSize);

		std::memcpy(filedata.data(), pixels, dataSize);

		stbi_image_free(pixels);
	}
	else
	{
		stbi_uc* pixels = stbi_load(filepath.c_str(), reinterpret_cast<int32_t*>(&width), reinterpret_cast<int32_t*>(&height), &channels, STBI_rgb_alpha);
		if (pixels == nullptr)
			throw std::runtime_error("CreateObjectTextureThirdFrame Error: Program failed to load a 8 bit texture!");

		size_t dataSize = static_cast<size_t>(width) * height;
		dataSize *= 4;
		filedata.resize(dataSize);

		std::memcpy(filedata.data(), pixels, dataSize);

		stbi_image_free(pixels);
	}

	auto resultData = main.TryToAddTextureToStreamedTexturesTransferList(filedata, width, height);

	if (!resultData.has_value())
		throw std::runtime_error("CreateObjectTextureThirdFrame Error: Program failed to add the texture to the transfer order list!");

	data.texturesIDs[2] = resultData.value();
}
