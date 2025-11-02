#pragma once

#include <CustomLists/IDObject.h>

#include <MainDef.h>
#include <VertexDataMainDef.h>

#include <array>

struct MainDataCollection
{
	std::unique_ptr<JJ2DE::Main> main;

	IDObject<JJ2DE::WorldLayerVertexDataLayerVersionListPointer> layerID;
	size_t objectID;

	std::pair<size_t, size_t> backgroundtexturesID;
	std::array<std::pair<size_t, size_t>, 3> worldTexturesIDs;
	size_t currentWorldObjectTexture;

	std::array<std::pair<size_t, size_t>, 3> uiTexturesIDs;
	size_t currentUIObjectTexture;

	bool is16Bit;
	bool isRBReversed;

	float cameraX;
	float cameraY;
	float rotation;
	float zoom;

	MainDataCollection();
	~MainDataCollection();
};