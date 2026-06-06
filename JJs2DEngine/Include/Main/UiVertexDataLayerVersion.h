#pragma once

#include <optional>
#include <utility>

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionInternal;

	struct UiObjectData;

	class UiVertexDataLayerVersion
	{
	public:
		UiVertexDataLayerVersion(UiVertexDataLayerVersionInternal& ref);
		~UiVertexDataLayerVersion();

		UiVertexDataLayerVersion& operator=(const UiVertexDataLayerVersion&) noexcept = delete;

		std::optional<size_t> AddObject(const UiObjectData& newObjectData);

		void ChangeObjectsTexture(bool inPreloadedTexturesList, std::pair<size_t, size_t> newTextureID, size_t objectsIndex);

	private:
		UiVertexDataLayerVersionInternal& _internal;
	};
}
