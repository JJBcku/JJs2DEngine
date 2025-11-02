#pragma once

#include <CustomLists/IDObject.h>

#include <MainDef.h>
#include <VertexDataMainDef.h>

#include <array>

struct MainDataCollection
{
	std::unique_ptr<JJ2DE::Main> main;

	IDObject<JJ2DE::UiVertexDataLayerVersionListPointer> UILayerID;
	size_t UIObjectID;

	IDObject<JJ2DE::WorldLayerVertexDataLayerVersionListPointer> worldLayerID;
	size_t worldObjectID;

	std::pair<size_t, size_t> backgroundtexturesID;
	std::array<std::pair<size_t, size_t>, 3> worldTexturesIDs;
	size_t currentWorldObjectTexture;

	std::array<std::pair<size_t, size_t>, 3> UITexturesIDs;
	size_t currentUIObjectTexture;

	float UITextureUpperY;
	float UITextureLowerY;

	bool is16Bit;
	bool isRBReversed;

	float cameraX;
	float cameraY;
	float rotation;
	float zoom;

	MainDataCollection();
	~MainDataCollection();
};