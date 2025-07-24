#pragma once

#include <array>
#include <Miscellaneous/Bool64Def.h>

namespace JJs2DEngine
{
	constexpr std::array<unsigned char, 8> pipelineHeaderCorrectMagicNumbers = { 'J', 'J', 'E', 'P', 'L', 'C', 'H', 0 };

	struct PipelineCacheMainHeader
	{
		std::array<unsigned char, 8> magicNumbers;
		uint64_t elementCount;

		PipelineCacheMainHeader();
	};

	struct PipelineCacheElementHeader
	{
		uint64_t elementSize;
		uint64_t elementCRC64WE;
		Misc::Bool64 deleted;

		PipelineCacheElementHeader();
	};
}