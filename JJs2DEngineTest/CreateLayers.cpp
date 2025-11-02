#include "pch.h"
#include "CreateLayers.h"

#include "MainDataCollection.h"

#include <Main.h>
#include <VertexDataMain.h>
#include <BackgroundObjectData.h>
#include <BackgroundVertexDataLayerVersionList.h>

#include <UiVertexDataLayerVersionList.h>
#include <UiVertexDataLayerVersion.h>
#include <UiObjectData.h>

#include <WorldLayerVertexDataLayerVersionList.h>
#include <WorldLayerVertexDataLayerVersion.h>
#include <WorldLayerObjectData.h>

void CreateLayers(MainDataCollection& data)
{
	auto vertexDataMain = data.main->GetVertexDataMainList();

	{
		JJ2DE::BackgroundObjectData backgroundData;
		backgroundData.textureWidthInPixels = 1920U;
		backgroundData.textureHeightInPixels = 1080U;
		backgroundData.textureIndex = data.backgroundtexturesID;
		backgroundData.inPreloadedTexturesList = false;

		vertexDataMain.CreateBackgroundLayerVersionList({ backgroundData });

		auto backgroundList = vertexDataMain.GetBackgroundVertexDataLayerVersionList();
		backgroundList.SetActiveVersion(0);
	}

	{
		data.UILayerID = vertexDataMain.AddUiLayerVersionList({ 1 }, 0x10);
		auto UIVertexLayerVersion = vertexDataMain.GetUiVertexDataLayerVersionList(data.UILayerID).GetLayersVersion(0);

		JJ2DE::UiObjectData UIObject;
		UIObject.screenWidth_UNORM = JJ2DE::onePointZeroUNORMValue;
		UIObject.screenHeight_UNORM = JJ2DE::onePointZeroUNORMValue / 8;
		UIObject.screenPositionX_SNORM = 0;
		UIObject.screenPositionY_SNORM = JJ2DE::onePointZeroSNORMValue - UIObject.screenHeight_UNORM;
		UIObject.textureWidthInPixels = 2048U;
		UIObject.textureHeightInPixels = 256U;
		UIObject.inPreloadedTexturesList = false;
		UIObject.textureIndex = data.uiTexturesIDs[0];

		auto optionalObjectID = UIVertexLayerVersion.AddObject(UIObject);

		if (!optionalObjectID.has_value())
			throw std::runtime_error("CreateLayers Error: Program failed to create the UI object!");

		data.UIObjectID = optionalObjectID.value();
		
	}

	{
		data.worldLayerID = vertexDataMain.AddWorldLayerVersionList({ 1 }, 0x10);
		auto worldVertexLayerVersion = vertexDataMain.GetWorldLayerVertexDataLayerVersionList(data.worldLayerID).GetLayersVersion(0);

		JJ2DE::WorldLayerObjectData object;
		object.screenWidth_UNORM = JJ2DE::onePointZeroUNORMValue / 2;
		object.screenHeight_UNORM = JJ2DE::onePointZeroUNORMValue / 2;
		object.screenPositionX_SNORM = 0;
		object.screenPositionY_SNORM = 0;
		object.textureWidthInPixels = 1024U;
		object.textureHeightInPixels = 1024U;
		object.inPreloadedTexturesList = false;
		object.textureIndex = data.worldTexturesIDs[0];

		auto optionalObjectID = worldVertexLayerVersion.AddObject(object);

		if (!optionalObjectID.has_value())
			throw std::runtime_error("CreateLayers Error: Program failed to create the world object!");

		data.worldObjectID = optionalObjectID.value();
	}

	vertexDataMain.SetCameraPosition(data.cameraX, data.cameraY);
	vertexDataMain.SetCameraZoom(data.zoom);
	vertexDataMain.SetPerspectiveRotation(data.rotation);
}
