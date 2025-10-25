#pragma once

#include <memory>

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionListInternal;
	class UiVertexDataLayerVersionList;

	class WorldLayerVertexDataLayerVersionListInternal;
	class WorldLayerVertexDataLayerVersionList;

	typedef std::unique_ptr<UiVertexDataLayerVersionListInternal> UiVertexDataLayerVersionListPointer;
	typedef std::unique_ptr<WorldLayerVertexDataLayerVersionListInternal> WorldLayerVertexDataLayerVersionListPointer;
}