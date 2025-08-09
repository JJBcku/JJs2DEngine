#pragma once

#include <stdint.h>

#if defined(JJ2DENGINE_8K_TEXTURES_ENABLED)
constexpr uint32_t skippedSizeLevels = 6;
#elseif defined(JJ2DENGINE_4K_TEXTURES_ENABLED)
constexpr uint32_t skippedSizeLevels = 5;
#else
constexpr uint32_t skippedSizeLevels = 4;
#endif


constexpr uint32_t imagesInTextureArray = 8;
constexpr uint32_t textureArraysAmount = 2;
constexpr uint32_t imagesInAllTextureArrays = imagesInTextureArray * textureArraysAmount;
