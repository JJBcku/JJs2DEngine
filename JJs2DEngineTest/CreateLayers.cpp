#include "pch.h"
#include "CreateLayers.h"

#include "MainDataCollection.h"

#include <Main.h>
#include <VertexDataMain.h>
#include <BackgroundObjectData.h>
#include <BackgroundVertexDataLayerVersionList.h>
#include <WorldLayerVertexDataLayerVersionList.h>
#include <WorldLayerVertexDataLayerVersion.h>
#include <WorldLayerObjectData.h>

void CreateLayers(MainDataCollection& data)
{
	auto vertexDataMain = data.main->GetVertexDataMainList();

	JJ2DE::BackgroundObjectData backgroundData;
	backgroundData.textureWidthInPixels = 1920U;
	backgroundData.textureHeightInPixels = 1080U;
	backgroundData.textureIndex = data.backgroundtexturesID;
	backgroundData.inPreloadedTexturesList = false;

	vertexDataMain.CreateBackgroundLayerVersionList({ backgroundData });

	auto backgroundList = vertexDataMain.GetBackgroundVertexDataLayerVersionList();
	backgroundList.SetActiveVersion(0);

	data.layerID = vertexDataMain.AddWorldLayerVersionList({ 1 }, 0x10);

	auto worldVertexLayerVersionList = vertexDataMain.GetWorldLayerVertexDataLayerVersionList(data.layerID);

	auto worldVertexLayerVersion = worldVertexLayerVersionList.GetLayersVersion(0);

	JJ2DE::WorldLayerObjectData object;
	object.screenWidth_UNORM = JJ2DE::onePointZeroUNORMValue / 2;
	object.screenHeight_UNORM = JJ2DE::onePointZeroUNORMValue / 2;
	object.screenPositionX_SNORM = 0;
	object.screenPositionY_SNORM = 0;
	object.textureWidthInPixels = 1024U;
	object.textureHeightInPixels = 1024U;
	object.inPreloadedTexturesList = false;
	object.textureIndex = data.texturesID;

	auto optionalObjectID = worldVertexLayerVersion.AddObject(object);

	if (!optionalObjectID.has_value())
		throw std::runtime_error("CreateLayers Error: Program failed to create the object!");

	data.objectID = optionalObjectID.value();

	vertexDataMain.SetCameraPosition(data.cameraX, data.cameraY);
	vertexDataMain.SetCameraZoom(data.zoom);
	vertexDataMain.SetPerspectiveRotation(data.rotation);

}
