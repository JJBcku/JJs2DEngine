#pragma once

#include <stdint.h>

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionListInternal
	{
	public:
		UiVertexDataLayerVersionListInternal(uint64_t stump);
		~UiVertexDataLayerVersionListInternal();

	private:
		uint64_t _stump;
	};
}
