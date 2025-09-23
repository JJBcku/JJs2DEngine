#include "MainDNIpch.h"
#include "UiVertexDataLayerVersionListInternal.h"

#include "UiVertexDataLayerVersionInternal.h"

namespace JJs2DEngine
{
	UiVertexDataLayerVersionListInternal::UiVertexDataLayerVersionListInternal(const std::vector<uint64_t>& versionsMaxVerticesList)
	{
		_versionList.reserve(versionsMaxVerticesList.size());
		for (uint64_t i = 0; i < versionsMaxVerticesList.size(); ++i)
		{
			_versionList.emplace_back(versionsMaxVerticesList[i]);
		}

		_activeLayer = 0;
	}

	UiVertexDataLayerVersionListInternal::~UiVertexDataLayerVersionListInternal()
	{
	}
}