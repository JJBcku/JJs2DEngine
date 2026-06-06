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
		struct UILayerID
		{
			VertexLayerOrderIDType type;
			IDObject<UiVertexDataLayerVersionListPointer> ID;

			UILayerID(const IDObject<UiVertexDataLayerVersionListPointer>& ID) noexcept;
			~UILayerID() = default;
		} uiLayerID;

		struct WorldLayerID
		{
			VertexLayerOrderIDType type;
			IDObject<WorldLayerVertexDataLayerVersionListPointer> ID;

			WorldLayerID(const IDObject<WorldLayerVertexDataLayerVersionListPointer>& ID) noexcept;
			~WorldLayerID() = default;
		} worldLayerID;

		VertexLayerOrderID() noexcept;
		VertexLayerOrderID(const VertexLayerOrderID& rhs) noexcept;
		VertexLayerOrderID(VertexLayerOrderID&& rhs) noexcept;
		VertexLayerOrderID(const IDObject<UiVertexDataLayerVersionListPointer>& ID) noexcept;
		VertexLayerOrderID(const IDObject<WorldLayerVertexDataLayerVersionListPointer>& ID) noexcept;
		~VertexLayerOrderID();

		VertexLayerOrderID& operator=(const VertexLayerOrderID& rhs) noexcept;
		VertexLayerOrderID& operator=(VertexLayerOrderID&& rhs) noexcept;
	};
}