#include "pch.h"
#include "CreateLayers.h"

#include "MainDataCollection.h"

#include <Main.h>
#include <VertexDataMain.h>
#include <UiVertexDataLayerVersionList.h>
#include <UiVertexDataLayerVersion.h>
#include <ObjectData.h>

void CreateLayers(MainDataCollection& data)
{
	auto vertexDataMain = data.main->GetVertexDataMainList();

	data.layerID = vertexDataMain.AddUiLayerVersionList({ 1 }, 0x10);

	auto uiVertexLayerVersionList = vertexDataMain.GetUiVertexDataLayerVersionList(data.layerID);

	auto uiVertexLayer = uiVertexLayerVersionList.GetLayersVersion(0);

	JJ2DE::ObjectData object;
	object.screenWidthUNORM = JJ2DE::onePointZeroUNORMValue / 2;
	object.screenHeightUNORM = JJ2DE::onePointZeroUNORMValue / 2;
	object.screenPositionXSNORM = 0;
	object.screenPositionYSNORM = 0;
	object.textureWidthUNORM = JJ2DE::onePointZeroUNORMValue;
	object.textureHeightUNORM = JJ2DE::onePointZeroUNORMValue;

	auto optionalObjectID = uiVertexLayer.AddObject(object);

	if (!optionalObjectID.has_value())
		throw std::runtime_error("CreateLayers Error: Program failed to create the object!");

	data.objectID = optionalObjectID.value();

}
