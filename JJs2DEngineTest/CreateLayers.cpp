#include "pch.h"
#include "CreateLayers.h"

#include "MainDataCollection.h"

#include <Main.h>
#include <VertexDataMain.h>
#include <WorldLayerVertexDataLayerVersionList.h>
#include <WorldLayerVertexDataLayerVersion.h>
#include <WorldLayerObjectData.h>

void CreateLayers(MainDataCollection& data)
{
	auto vertexDataMain = data.main->GetVertexDataMainList();

	data.layerID = vertexDataMain.AddWorldLayerVersionList({ 1 }, 0x10);

	auto worldVertexLayerVersionList = vertexDataMain.GetWorldLayerVertexDataLayerVersionList(data.layerID);

	auto worldVertexLayerVersion = worldVertexLayerVersionList.GetLayersVersion(0);

	JJ2DE::WorldLayerObjectData object;
	object.screenWidth_UNORM = JJ2DE::onePointZeroUNORMValue / 2;
	object.screenHeight_UNORM = JJ2DE::onePointZeroUNORMValue / 2;
	object.screenPositionX_SNORM = 0;
	object.screenPositionY_SNORM = 0;
	object.textureWidth_UNORM = JJ2DE::onePointZeroUNORMValue;
	object.textureHeight_UNORM = JJ2DE::onePointZeroUNORMValue;
	object.inPreloadedTexturesList = false;
	object.textureIndex = data.texturesID;

	auto optionalObjectID = worldVertexLayerVersion.AddObject(object);

	if (!optionalObjectID.has_value())
		throw std::runtime_error("CreateLayers Error: Program failed to create the object!");

	data.objectID = optionalObjectID.value();

	vertexDataMain.SetCameraPosition(0.0f, 0.0f);
	vertexDataMain.SetCameraZoom(0.5f);

}
