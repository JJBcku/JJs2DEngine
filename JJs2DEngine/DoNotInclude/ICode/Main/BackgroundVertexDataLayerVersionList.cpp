#include "MainIpch.h"
#include "../../../Include/Main/BackgroundVertexDataLayerVersionList.h"

#include "../../DNIData/Main/BackgroundVertexDataLayerVersionListInternal.h"

namespace JJs2DEngine
{
	BackgroundVertexDataLayerVersionList::BackgroundVertexDataLayerVersionList(BackgroundVertexDataLayerVersionListInternal& ref) : _internal(ref)
	{
	}

	BackgroundVertexDataLayerVersionList::~BackgroundVertexDataLayerVersionList()
	{
	}

	void BackgroundVertexDataLayerVersionList::SetActiveVersion(std::optional<size_t> newActiveVersion)
	{
		_internal.SetActiveVersion(newActiveVersion);
	}

	std::optional<size_t> BackgroundVertexDataLayerVersionList::GetActiveVersion() const
	{
		return _internal.GetActiveVersion();
	}

}