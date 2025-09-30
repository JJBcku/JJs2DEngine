#pragma once

#include "../../../Include/Main/VertexDataMainDef.h"

#include <CustomLists/UnsortedIDVector.h>
#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>

#include <stdint.h>
#include <vector>

namespace JJs2DEngine
{
	union VertexLayerOrderID;

	class TextureDataMainInternal;

	class VertexDataMainInternal
	{
	public:
		VertexDataMainInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists dataBufferList, VS::MemoryObjectsList memoryObjectsList);
		~VertexDataMainInternal();

		IDObject<UiVertexDataLayerVersionListPointer> AddUiLayerVersionList(const std::vector<size_t>& versionsMaxVerticesList, size_t addOnReserving);

		UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID);

		const UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const;

	private:
		TextureDataMainInternal& _textureDataList;
		VS::DataBufferLists _dataBufferList;
		VS::MemoryObjectsList _memoryObjectsList;

		UnsortedIDVector<UiVertexDataLayerVersionListPointer> _uiLayersList;

		std::vector<VertexLayerOrderID> _layerOrderList;
	};
}