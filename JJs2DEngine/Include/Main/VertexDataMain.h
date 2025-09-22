#pragma once

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

		IDObject<UiVertexDataLayerVersionListInternal> AddUiLayerVersionList(size_t initialVersionCapacity, size_t addOnReserving);

	private:
		VertexDataMainInternal& _internal;
	};
}