#pragma once

#include <stdint.h>
#include <vector>

#include <CustomLists/UnsortedIDVector.h>

#include "../../../Include/Main/VertexDataMainDef.h"

namespace JJs2DEngine
{
	union VertexLayerOrderID;

	class TextureDataMainInternal;

	class VertexDataMainInternal
	{
	public:
		VertexDataMainInternal(TextureDataMainInternal& textureDataList);
		~VertexDataMainInternal();

		IDObject<UiVertexDataLayerVersionListPointer> AddUiLayerVersionList(const std::vector<size_t>& versionsMaxVerticesList, size_t addOnReserving);

		UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID);

		const UiVertexDataLayerVersionListInternal& GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const;

	private:
		TextureDataMainInternal& _textureDataList;

		UnsortedIDVector<UiVertexDataLayerVersionListPointer> _uiLayersList;

		std::vector<VertexLayerOrderID> _layerOrderList;
	};
}