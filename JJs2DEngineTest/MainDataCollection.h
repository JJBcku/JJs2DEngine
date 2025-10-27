#pragma once

#include <CustomLists/IDObject.h>

#include <MainDef.h>
#include <VertexDataMainDef.h>

struct MainDataCollection
{
	std::unique_ptr<JJ2DE::Main> main;

	IDObject<JJ2DE::WorldLayerVertexDataLayerVersionListPointer> layerID;
	size_t objectID;

	std::pair<size_t, size_t> backgroundtexturesID;
	std::pair<size_t, size_t> texturesID;

	bool is16Bit;
	bool isRBReversed;

	MainDataCollection();
	~MainDataCollection();
};