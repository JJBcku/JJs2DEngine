#pragma once

#include <optional>

namespace JJs2DEngine
{
	class BackgroundVertexDataLayerVersionListInternal;

	class BackgroundVertexDataLayerVersionList
	{
	public:
		BackgroundVertexDataLayerVersionList(BackgroundVertexDataLayerVersionListInternal& ref);
		~BackgroundVertexDataLayerVersionList();

		void SetActiveVersion(std::optional<size_t> newActiveVersion);
		std::optional<size_t> GetActiveVersion() const;

	private:
		BackgroundVertexDataLayerVersionListInternal& _internal;
	};
}
