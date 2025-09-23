#pragma once

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionInternal
	{
	public:
		UiVertexDataLayerVersionInternal(uint64_t maxVertexAmount);
		~UiVertexDataLayerVersionInternal();

	private:
		uint64_t _maxVertexAmount;
	};
}
