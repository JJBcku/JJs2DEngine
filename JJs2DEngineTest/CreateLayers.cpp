#include "pch.h"
#include "CreateLayers.h"

#include "MainDataCollection.h"

#include <Main.h>
#include <VertexDataMain.h>
#include <UiVertexDataLayerVersionList.h>
#include <UiVertexDataLayerVersion.h>
#include <UiObjectData.h>

void CreateLayers(MainDataCollection& data)
{
	auto vertexDataMain = data.main->GetVertexDataMainList();

	data.layerID = vertexDataMain.AddUiLayerVersionList({ 1 }, 0x10);

	auto uiVertexLayerVersionList = vertexDataMain.GetUiVertexDataLayerVersionList(data.layerID);

	auto uiVertexLayer = uiVertexLayerVersionList.GetLayersVersion(0);

	JJ2DE::UiObjectData object;
	object.screenWidth_UNORM = JJ2DE::onePointZeroUNORMValue / 2;
	object.screenHeight_UNORM = JJ2DE::onePointZeroUNORMValue / 2;
	object.screenPositionX_SNORM = 0;
	object.screenPositionY_SNORM = 0;
	object.textureWidth_UNORM = JJ2DE::onePointZeroUNORMValue;
	object.textureHeight_UNORM = JJ2DE::onePointZeroUNORMValue;
	object.textureIndex = {7, 0};

	auto optionalObjectID = uiVertexLayer.AddObject(object);

	if (!optionalObjectID.has_value())
		throw std::runtime_error("CreateLayers Error: Program failed to create the object!");

	data.objectID = optionalObjectID.value();

}
