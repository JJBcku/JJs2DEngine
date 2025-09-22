#include "MainDNIpch.h"
#include "VertexDataMainInternal.h"

#include "UiVertexDataLayerVersionListInternal.h"
#include "../../../Include/Main/VertexLayerOrderID.h"

namespace JJs2DEngine
{
	constexpr size_t maxActiveLayersCount = 126;

	VertexDataMainInternal::VertexDataMainInternal() : _uiLayersList(maxActiveLayersCount)
	{
		_layerOrderList.reserve(maxActiveLayersCount);
	}

	VertexDataMainInternal::~VertexDataMainInternal()
	{
	}

	IDObject<UiVertexDataLayerVersionListInternal> VertexDataMainInternal::AddUiLayerVersionList(size_t initialVersionCapacity, size_t addOnReserving)
	{
		if (_layerOrderList.size() == _layerOrderList.capacity())
			throw std::runtime_error("VertexDataMainInternal::AddUiLayerVersionList Error: Program tried to add more layers than the program supports!");

		auto ret = _uiLayersList.AddObject(UiVertexDataLayerVersionListInternal(initialVersionCapacity), addOnReserving);

		_layerOrderList.emplace_back(ret);

		return ret;
	}

}