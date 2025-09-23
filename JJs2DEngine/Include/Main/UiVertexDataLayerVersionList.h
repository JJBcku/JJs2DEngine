#pragma once

#include "UiVertexDataLayerVersionListDef.h"

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionList
	{
	public:
		UiVertexDataLayerVersionList(UiVertexDataLayerVersionListInternal& ref);
		~UiVertexDataLayerVersionList();

	private:
		UiVertexDataLayerVersionListInternal& _internal;
	};
}
