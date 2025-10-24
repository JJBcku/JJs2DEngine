#pragma once

#include "VertexTransferFrameData.h"

#include <Miscellaneous/Bool64Def.h>
#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSPrimaryIRCommandBufferDef.h>

#include <CustomLists/IDObject.h>

#include <vector>
#include <optional>
#include <stdint.h>

namespace JJs2DEngine
{
	struct UiObjectData;
	struct UiObjectDataInternal;
	typedef std::optional<UiObjectDataInternal> UIObjectDataOptional;

	class TextureDataMainInternal;

	struct VertexTransferFrameData;

	class UiVertexDataLayerVersionInternal
	{
	public:
		UiVertexDataLayerVersionInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList, size_t maxObjectAmount, size_t layersDepth, size_t transferFrameAmount);
		~UiVertexDataLayerVersionInternal();

		std::optional<size_t> AddObject(const UiObjectData& newObjectData);

		uint32_t GetBuffersMask() const;
		uint64_t GetSingleBuffersMemorySize() const;
		uint64_t GetTotalBuffersMemorySize() const;
		uint64_t GetMemoryAligment() const;

		IDObject<VS::AutoCleanupVertexBuffer> GetVertexBufferID(size_t transferFrameIndice);

		size_t WriteDataToBuffer(std::optional<IDObject<VS::AutoCleanupStagingBuffer>> stagingBufferID, size_t transferFrameIndice, bool noChangeOverride);
		size_t GetCurrentUsedObjectAmount() const;
		size_t GetAmountOfObjectsInVertexBuffer(size_t transferFrameIndice) const;

		Misc::Bool64 IsOwnedByTransferQueue(size_t transferFrameIndice) const;
		void SetOwnedByTransferQueue(size_t transferFrameIndice, Misc::Bool64Values newValue);

	private:
		TextureDataMainInternal& _textureDataList;
		VS::DataBufferLists _dataBufferList;

		std::vector<UIObjectDataOptional> _objectList;
		std::vector<size_t> _unusedIndexes;
		size_t _usedObjectAmount;
		size_t _nextDepthValueUNORM;
		size_t _layersDepth;

		std::vector<Misc::Bool64> _ownedByTransferQueue;
		std::vector<VertexTransferFrameData> _frameData;

		uint64_t _buffersMemoryMask;
		uint64_t _singleBuffersMemorySize;
		uint64_t _buffersMemoryAligment;

		uint64_t _totalBuffersMemorySize;
	};
}
