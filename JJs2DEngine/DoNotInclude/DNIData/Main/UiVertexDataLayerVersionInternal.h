#pragma once

#include <vector>
#include <optional>
#include <stdint.h>

#include <Miscellaneous/Bool64Def.h>
#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>

#include <CustomLists/IDObject.h>

namespace JJs2DEngine
{
	struct UiObjectData;
	struct UiObjectDataInternal;
	typedef std::optional<UiObjectDataInternal> ObjectDataOptional;

	class TextureDataMainInternal;

	class UiVertexDataLayerVersionInternal
	{
	public:
		UiVertexDataLayerVersionInternal(TextureDataMainInternal& textureDataList, VS::DataBufferLists& dataBufferList, size_t maxVertexAmount, size_t layersDepth);
		~UiVertexDataLayerVersionInternal();

		std::optional<size_t> AddObject(const UiObjectData& newObjectData);

		uint32_t GetBuffersMask() const;
		uint64_t GetMemorySize() const;
		uint64_t GetMemoryAligment() const;

		IDObject<VS::AutoCleanupVertexBuffer> GetVertexBufferID();

	private:
		TextureDataMainInternal& _textureDataList;
		VS::DataBufferLists _dataBufferList;

		std::vector<ObjectDataOptional> _objectList;
		std::vector<size_t> _unusedIndexes;
		size_t _usedVertexAmount;
		size_t _nextDepthValueUNORM;
		size_t _layersDepth;

		uint64_t _buffersMemoryMask;
		uint64_t _buffersMemorySize;
		uint64_t _buffersMemoryAligment;

		Misc::Bool64 _changed;

		IDObject<VS::AutoCleanupVertexBuffer> _vertexBuffer;
	};
}
