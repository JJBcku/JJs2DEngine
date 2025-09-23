#include "pch.h"
#include "CreateLayers.h"

#include "MainDataCollection.h"

#include <Main.h>
#include <VertexDataMain.h>
#include <UiVertexDataLayerVersionList.h>

void CreateLayers(MainDataCollection& data)
{
	auto vertexDataMain = data.main->GetVertexDataMainList();

	data.layerID = vertexDataMain.AddUiLayerVersionList({ 1 }, 0x10);

	auto uiVertexLayerVersionList = vertexDataMain.GetUiVertexDataLayerVersionList(data.layerID);
}
