#pragma once

#include <stdint.h>
#include <vector>

#include <CustomLists/IDObjectDef.h>

#include "VertexDataMainDef.h"

namespace JJs2DEngine
{
	class VertexDataMainInternal;

	class VertexDataMain
	{
	public:
		VertexDataMain(VertexDataMainInternal& ref);
		~VertexDataMain();

		IDObject<UiVertexDataLayerVersionListPointer> AddUiLayerVersionList(const std::vector<uint64_t>& versionsMaxVerticesList, size_t addOnReserving);

		UiVertexDataLayerVersionList GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID);

		const UiVertexDataLayerVersionList GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const;

	private:
		VertexDataMainInternal& _internal;
	};
}