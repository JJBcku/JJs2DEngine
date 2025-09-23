#pragma once

#include <memory>

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionListInternal;
	class UiVertexDataLayerVersionList;

	typedef std::unique_ptr<UiVertexDataLayerVersionListInternal> UiVertexDataLayerVersionListPointer;
}