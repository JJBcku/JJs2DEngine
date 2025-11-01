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
	std::array<std::pair<size_t, size_t>, 3> texturesID;
	size_t currentObjectTexture;

	bool is16Bit;
	bool isRBReversed;

	float cameraX;
	float cameraY;
	float rotation;
	float zoom;

	MainDataCollection();
	~MainDataCollection();
};