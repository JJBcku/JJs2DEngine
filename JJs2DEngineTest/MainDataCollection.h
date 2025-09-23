#pragma once

#include <CustomLists/IDObject.h>

#include <MainDef.h>
#include <VertexDataMainDef.h>

struct MainDataCollection
{
	std::unique_ptr<JJ2DE::Main> main;

	IDObject<JJ2DE::UiVertexDataLayerVersionListPointer> layerID;

	MainDataCollection();
	~MainDataCollection();
};