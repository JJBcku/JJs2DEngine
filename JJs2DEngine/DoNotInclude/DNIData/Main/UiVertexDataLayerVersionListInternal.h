#pragma once

#include <stdint.h>
#include <vector>
#include <memory>

#include "../../../Include/Main/UiVertexDataLayerVersionListDef.h"

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionInternal;
	class TextureDataMainInternal;

	class UiVertexDataLayerVersionListInternal
	{
	public:
		UiVertexDataLayerVersionListInternal(TextureDataMainInternal& textureDataList, const std::vector<size_t>& versionsMaxVerticesList, size_t layersDepth);
		~UiVertexDataLayerVersionListInternal();

		UiVertexDataLayerVersionListInternal(const UiVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		UiVertexDataLayerVersionListInternal(UiVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

		UiVertexDataLayerVersionListInternal& operator=(const UiVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		UiVertexDataLayerVersionListInternal& operator=(UiVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

		UiVertexDataLayerVersionInternal& GetLayersVersion(size_t versionIndex);

		const UiVertexDataLayerVersionInternal& GetLayersVersion(size_t versionIndex) const;

	private:
		std::vector<std::unique_ptr<UiVertexDataLayerVersionInternal>> _versionList;
		size_t _activeLayer;
	};
}
