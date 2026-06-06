#pragma once

#include <optional>
#include <utility>

namespace JJs2DEngine
{
	class BackgroundVertexDataLayerVersionListInternal;

	class BackgroundVertexDataLayerVersionList
	{
	public:
		BackgroundVertexDataLayerVersionList(BackgroundVertexDataLayerVersionListInternal& ref);
		~BackgroundVertexDataLayerVersionList();

		BackgroundVertexDataLayerVersionList& operator=(const BackgroundVertexDataLayerVersionList&) noexcept = delete;

		void SetActiveVersion(std::optional<size_t> newActiveVersion);
		std::optional<size_t> GetActiveVersion() const;

		void ChangeObjectsTexture(size_t layerIndex, bool inPreloadedTexturesList, std::pair<size_t, size_t> newTextureID);

	private:
		BackgroundVertexDataLayerVersionListInternal& _internal;
	};
}
