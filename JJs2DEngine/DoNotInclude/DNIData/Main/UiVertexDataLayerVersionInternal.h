#pragma once

#include <vector>
#include <optional>
#include <stdint.h>

#include <Miscellaneous/Bool64Def.h>
#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>
#include <VulkanSimplified/VSDevice/VSPrimaryIRCommandBufferDef.h>

#include <CustomLists/IDObject.h>

namespace JJs2DEngine
{
	struct UiObjectData;
	struct UiObjectDataInternal;
	typedef std::optional<UiObjectDataInternal> ObjectDataOptional;

	class TextureDataMainInternal;

	struct TransferFrameData
	{
		Misc::Bool64 changed;
		size_t lastAmountWrittenToVertexBuffer;
		IDObject<VS::AutoCleanupVertexBuffer> vertexBuffer;

		TransferFrameData();
		~TransferFrameData();
	};

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

		size_t WriteDataToBuffer(std::optional<IDObject<VS::AutoCleanupStagingBuffer>> stagingBufferID, size_t transferFrameIndice);
		size_t GetCurrentUsedObjectAmount() const;
		size_t GetAmountOfObjectsInVertexBuffer(size_t transferFrameIndice) const;

		Misc::Bool64 IsOwnedByTransferQueue(size_t transferFrameIndice) const;
		void SetOwnedByTransferQueue(size_t transferFrameIndice, Misc::Bool64Values newValue);

	private:
		TextureDataMainInternal& _textureDataList;
		VS::DataBufferLists _dataBufferList;

		std::vector<ObjectDataOptional> _objectList;
		std::vector<size_t> _unusedIndexes;
		size_t _usedObjectAmount;
		size_t _nextDepthValueUNORM;
		size_t _layersDepth;

		std::vector<Misc::Bool64> _ownedByTransferQueue;
		std::vector<TransferFrameData> _frameData;

		uint64_t _buffersMemoryMask;
		uint64_t _singleBuffersMemorySize;
		uint64_t _buffersMemoryAligment;

		uint64_t _totalBuffersMemorySize;
	};
}
