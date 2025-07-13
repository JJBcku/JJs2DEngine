#include "MainDNIpch.h"
#include "PipelineCacheHeaders.h"

namespace JJs2DEngine
{
	PipelineCacheMainHeader::PipelineCacheMainHeader()
	{
		magicNumbers = pipelineHeaderCorrectMagicNumbers;
		elementCount = 0;
	}

	PipelineCacheElementHeader::PipelineCacheElementHeader()
	{
		elementSize = 0;
		elementCRC64WE = 0;
		deleted = VS::BOOL64_FALSE;
	}

}