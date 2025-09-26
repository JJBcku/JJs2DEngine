#pragma once

#include "UiVertexDataLayerVersionListDef.h"

namespace JJs2DEngine
{
	class UiVertexDataLayerVersion;

	class UiVertexDataLayerVersionList
	{
	public:
		UiVertexDataLayerVersionList(UiVertexDataLayerVersionListInternal& ref);
		~UiVertexDataLayerVersionList();

		UiVertexDataLayerVersion GetLayersVersion(size_t versionIndex);

		const UiVertexDataLayerVersion GetLayersVersion(size_t versionIndex) const;

	private:
		UiVertexDataLayerVersionListInternal& _internal;
	};
}
