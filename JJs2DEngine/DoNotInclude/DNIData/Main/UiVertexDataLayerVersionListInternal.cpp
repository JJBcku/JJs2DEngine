#include "MainDNIpch.h"
#include "UiVertexDataLayerVersionListInternal.h"

#include "UiVertexDataLayerVersionInternal.h"

namespace JJs2DEngine
{
	UiVertexDataLayerVersionListInternal::UiVertexDataLayerVersionListInternal(TextureDataMainInternal& textureDataList, const std::vector<size_t>& versionsMaxVerticesList, size_t layersDepth)
	{
		_versionList.reserve(versionsMaxVerticesList.size());
		for (uint64_t i = 0; i < versionsMaxVerticesList.size(); ++i)
		{
			_versionList.push_back(std::make_unique<UiVertexDataLayerVersionInternal>(textureDataList, versionsMaxVerticesList[i], layersDepth));
		}

		_activeLayer = 0;
	}

	UiVertexDataLayerVersionListInternal::~UiVertexDataLayerVersionListInternal()
	{
	}

	UiVertexDataLayerVersionInternal& UiVertexDataLayerVersionListInternal::GetLayersVersion(size_t versionIndex)
	{
		if (versionIndex >= _versionList.size())
			throw std::runtime_error("UiVertexDataLayerVersionListInternal::GetLayersVersion Error: Program tried to get a non-existent version of the layer!");

		return *_versionList[versionIndex];
	}

	const UiVertexDataLayerVersionInternal& UiVertexDataLayerVersionListInternal::GetLayersVersion(size_t versionIndex) const
	{
		if (versionIndex >= _versionList.size())
			throw std::runtime_error("UiVertexDataLayerVersionListInternal::GetLayersVersion Const Error: Program tried to get a non-existent version of the layer!");

		return *_versionList[versionIndex];
	}

}