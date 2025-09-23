#pragma once

#include <stdint.h>
#include <vector>

#include <CustomLists/UnsortedIDVector.h>

#include "../../../Include/Main/VertexDataMainDef.h"

namespace JJs2DEngine
{
	union VertexLayerOrderID;

	class VertexDataMainInternal
	{
	public:
		VertexDataMainInternal();
		~VertexDataMainInternal();

		IDObject<UiVertexDataLayerVersionListPointer> AddUiLayerVersionList(const std::vector<uint64_t>& versionsMaxVerticesList, size_t addOnReserving);

		UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID);

		const UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const;

	private:
		UnsortedIDVector<UiVertexDataLayerVersionListPointer> _uiLayersList;

		std::vector<VertexLayerOrderID> _layerOrderList;
	};
}