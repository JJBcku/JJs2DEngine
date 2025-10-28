#include "MainIpch.h"
#include "../../../Include/Main/VertexDataMain.h"

#include "../../DNIData/Main/VertexDataMainInternal.h"

#include "../../../Include/Main/BackgroundVertexDataLayerVersionList.h"
#include "../../../Include/Main/UiVertexDataLayerVersionList.h"
#include "../../../Include/Main/WorldLayerVertexDataLayerVersionList.h"

namespace JJs2DEngine
{
	VertexDataMain::VertexDataMain(VertexDataMainInternal& ref) : _internal(ref)
	{
	}

	VertexDataMain::~VertexDataMain()
	{
	}

	void VertexDataMain::SetCameraPosition(float X, float Y)
	{
		_internal.SetCameraPosition(X, Y);
	}

	void VertexDataMain::SetCameraRotation(float rotation)
	{
		_internal.SetCameraRotation(rotation);
	}

	void VertexDataMain::SetCameraZoom(float zoom)
	{
		_internal.SetCameraZoom(zoom);
	}

	void VertexDataMain::CreateBackgroundLayerVersionList(const std::vector<BackgroundObjectData>& versionDataList)
	{
		_internal.CreateBackgroundLayerVersionList(versionDataList);
	}

	IDObject<UiVertexDataLayerVersionListPointer> VertexDataMain::AddUiLayerVersionList(const std::vector<size_t>& versionsMaxVerticesList, size_t addOnReserving)
	{
		return _internal.AddUiLayerVersionList(versionsMaxVerticesList, addOnReserving);
	}

	IDObject<WorldLayerVertexDataLayerVersionListPointer> VertexDataMain::AddWorldLayerVersionList(const std::vector<size_t>& versionsMaxVerticesList, size_t addOnReserving)
	{
		return _internal.AddWorldLayerVersionList(versionsMaxVerticesList, addOnReserving);
	}

	BackgroundVertexDataLayerVersionList VertexDataMain::GetBackgroundVertexDataLayerVersionList()
	{
		return _internal.GetBackgroundVertexDataLayerVersionList();
	}

	UiVertexDataLayerVersionList VertexDataMain::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID)
	{
		return _internal.GetUiVertexDataLayerVersionList(ID);
	}

	WorldLayerVertexDataLayerVersionList VertexDataMain::GetWorldLayerVertexDataLayerVersionList(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID)
	{
		return _internal.GetWorldLayerVertexDataLayerVersionList(ID);
	}

	const BackgroundVertexDataLayerVersionList VertexDataMain::GetBackgroundVertexDataLayerVersionList() const
	{
		return _internal.GetBackgroundVertexDataLayerVersionList();
	}

	const UiVertexDataLayerVersionList VertexDataMain::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const
	{
		return _internal.GetUiVertexDataLayerVersionList(ID);
	}

	const WorldLayerVertexDataLayerVersionList VertexDataMain::GetWorldLayerVertexDataLayerVersionList(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID) const
	{
		return _internal.GetWorldLayerVertexDataLayerVersionList(ID);
	}
}