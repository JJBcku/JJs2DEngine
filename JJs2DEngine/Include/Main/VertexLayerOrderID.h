#pragma once

#include <CustomLists/IDObject.h>

#include "VertexDataMainDef.h"

namespace JJs2DEngine
{
	enum class VertexLayerOrderIDType : uint64_t
	{
		UNSET = 0,
		UI_LAYER = 1,
		WORLD_LAYER = 2,
	};

	union VertexLayerOrderID
	{
		VertexLayerOrderIDType type;
		struct
		{
			VertexLayerOrderIDType type;
			IDObject<UiVertexDataLayerVersionListPointer> ID;
		} uiLayerID;

		struct
		{
			VertexLayerOrderIDType type;
			IDObject<WorldLayerVertexDataLayerVersionListPointer> ID;
		} worldLayerID;

		VertexLayerOrderID() noexcept;
		VertexLayerOrderID(const VertexLayerOrderID& rhs) noexcept;
		VertexLayerOrderID(VertexLayerOrderID&& rhs) noexcept;
		VertexLayerOrderID(IDObject<UiVertexDataLayerVersionListPointer> ID) noexcept;
		VertexLayerOrderID(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID) noexcept;
		~VertexLayerOrderID();

		VertexLayerOrderID& operator=(const VertexLayerOrderID& rhs) noexcept;
		VertexLayerOrderID& operator=(VertexLayerOrderID&& rhs) noexcept;
	};
}