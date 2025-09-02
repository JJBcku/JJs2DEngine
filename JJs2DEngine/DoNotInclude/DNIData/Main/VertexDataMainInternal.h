#pragma once

#include <stdint.h>

namespace JJs2DEngine
{
	class VertexDataMainInternal
	{
	public:
		VertexDataMainInternal(uint64_t stub);
		~VertexDataMainInternal();

	private:
		uint64_t _stub;
	};
}