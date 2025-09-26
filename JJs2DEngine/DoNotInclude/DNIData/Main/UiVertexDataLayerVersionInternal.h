#pragma once

#include <vector>
#include <optional>

namespace JJs2DEngine
{
	struct ObjectData;
	struct ObjectDataInternal;
	typedef std::optional<ObjectDataInternal> ObjectDataOptional;

	class TextureDataMainInternal;

	class UiVertexDataLayerVersionInternal
	{
	public:
		UiVertexDataLayerVersionInternal(TextureDataMainInternal& textureDataList, size_t maxVertexAmount, size_t layersDepth);
		~UiVertexDataLayerVersionInternal();

		std::optional<size_t> AddObject(const ObjectData& newObjectData);

	private:
		TextureDataMainInternal& _textureDataList;

		std::vector<ObjectDataOptional> _objectList;
		std::vector<size_t> _unusedIndexes;
		size_t _usedVertexAmount;
		size_t _nextDepthValueUNORM;
		size_t _layersDepth;
	};
}
