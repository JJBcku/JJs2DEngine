#include "pch.h"
#include "MainDataCollection.h"

#include <Main.h>

#include <limits>

MainDataCollection::MainDataCollection()
{
	UIObjectID = std::numeric_limits<size_t>::max();
	worldObjectID = std::numeric_limits<size_t>::max();

	backgroundtexturesID = { std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max() };
	worldTexturesIDs[0] = {std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()};
	worldTexturesIDs[1] = {std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()};
	worldTexturesIDs[2] = {std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max()};
	currentWorldObjectTexture = 0;

	UITexturesIDs[0] = { std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max() };
	UITexturesIDs[1] = { std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max() };
	UITexturesIDs[2] = { std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::max() };
	currentUIObjectTexture = 0;

	UITextureUpperY = 0.0f;
	UITextureLowerY = 0.0f;

	is16Bit = false;
	isRBReversed = false;

	cameraX = 0.0f;
	cameraY = 0.0f;
	rotation = 0.0f;
	zoom = 1.0f;
}

MainDataCollection::~MainDataCollection()
{
}
