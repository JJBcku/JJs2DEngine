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

		IDObject<UiVertexDataLayerVersionListInternal> AddUiLayerVersionList(size_t initialVersionCapacity, size_t addOnReserving);

	private:
		UnsortedIDVector<UiVertexDataLayerVersionListInternal> _uiLayersList;

		std::vector<VertexLayerOrderID> _layerOrderList;
	};
}