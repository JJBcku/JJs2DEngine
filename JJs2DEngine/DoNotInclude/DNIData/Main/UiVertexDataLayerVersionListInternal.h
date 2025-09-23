#pragma once

#include <stdint.h>
#include <vector>

#include "../../../Include/Main/UiVertexDataLayerVersionListDef.h"

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionListInternal
	{
	public:
		UiVertexDataLayerVersionListInternal(const std::vector<uint64_t>& versionsMaxVerticesList);
		~UiVertexDataLayerVersionListInternal();

		UiVertexDataLayerVersionListInternal(const UiVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		UiVertexDataLayerVersionListInternal(UiVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

		UiVertexDataLayerVersionListInternal& operator=(const UiVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		UiVertexDataLayerVersionListInternal& operator=(UiVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

	private:
		std::vector<UiVertexDataLayerVersionInternal> _versionList;
		size_t _activeLayer;
	};
}
