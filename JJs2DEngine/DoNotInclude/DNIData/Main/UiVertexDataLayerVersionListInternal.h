#pragma once

#include <stdint.h>
#include <vector>
#include <memory>
#include <optional>

#include "../../../Include/Main/UiVertexDataLayerVersionListDef.h"

#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSMemoryObjectsList.h>

#include <CustomLists/IDObject.h>

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionInternal;
	class TextureDataMainInternal;

	class UiVertexDataLayerVersionListInternal
	{
	public:
		UiVertexDataLayerVersionListInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList, VS::MemoryObjectsList& memoryObjectsList,
			const std::vector<size_t>& versionsMaxVerticesList, size_t layersDepth);
		~UiVertexDataLayerVersionListInternal();

		UiVertexDataLayerVersionListInternal(const UiVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		UiVertexDataLayerVersionListInternal(UiVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

		UiVertexDataLayerVersionListInternal& operator=(const UiVertexDataLayerVersionListInternal& rhs) noexcept = delete;
		UiVertexDataLayerVersionListInternal& operator=(UiVertexDataLayerVersionListInternal&& rhs) noexcept = delete;

		void WriteDataToBuffer();

		UiVertexDataLayerVersionInternal& GetLayersVersion(size_t versionIndex);

		const UiVertexDataLayerVersionInternal& GetLayersVersion(size_t versionIndex) const;

	private:
		VS::DataBufferLists _dataBufferList;
		VS::MemoryObjectsList _memoryObjectsList;

		std::vector<std::unique_ptr<UiVertexDataLayerVersionInternal>> _versionList;
		size_t _activeLayer;

		VS::MemoryAllocationFullID _vertexMemoryID;

		std::optional<IDObject<VS::AutoCleanupStagingBuffer>> _stagingBufferID;
		std::optional<VS::MemoryAllocationFullID> _stagingMemoryID;
	};
}
