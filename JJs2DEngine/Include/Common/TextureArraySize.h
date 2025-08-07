#pragma once

#include <stdint.h>

constexpr uint32_t skippedSizeLevels = 6;
constexpr uint32_t imagesInTextureArray = 8;
constexpr uint32_t textureArraysAmount = 2;
constexpr uint32_t imagesInAllTextureArrays = imagesInTextureArray * textureArraysAmount;
