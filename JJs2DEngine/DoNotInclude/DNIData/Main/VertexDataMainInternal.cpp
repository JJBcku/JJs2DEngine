#include "MainDNIpch.h"
#include "VertexDataMainInternal.h"

#include "UiVertexDataLayerVersionListInternal.h"
#include "../../../Include/Main/VertexLayerOrderID.h"

namespace JJs2DEngine
{
	constexpr size_t maxActiveLayersCount = 126;

	VertexDataMainInternal::VertexDataMainInternal(TextureDataMainInternal& textureDataList) : _textureDataList(textureDataList), _uiLayersList(maxActiveLayersCount)
	{
		_layerOrderList.reserve(maxActiveLayersCount);
	}

	VertexDataMainInternal::~VertexDataMainInternal()
	{
	}

	IDObject<UiVertexDataLayerVersionListPointer> VertexDataMainInternal::AddUiLayerVersionList(const std::vector<size_t>& versionsMaxVerticesList, size_t addOnReserving)
	{
		if (_layerOrderList.size() == _layerOrderList.capacity())
			throw std::runtime_error("VertexDataMainInternal::AddUiLayerVersionList Error: Program tried to add more layers than the program supports!");

		if (versionsMaxVerticesList.empty())
			throw std::runtime_error("VertexDataMainInternal::AddUiLayerVersionList Error: Program tried to create an empty version list!");

		auto ret = _uiLayersList.AddObject(std::make_unique<UiVertexDataLayerVersionListInternal>(_textureDataList, versionsMaxVerticesList, _layerOrderList.size()), addOnReserving);

		_layerOrderList.emplace_back(ret);

		return ret;
	}

	UiVertexDataLayerVersionListInternal& VertexDataMainInternal::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID)
	{
		return *_uiLayersList.GetObject(ID);
	}

	const UiVertexDataLayerVersionListInternal& VertexDataMainInternal::GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const
	{
		return *_uiLayersList.GetConstObject(ID);
	}

}