#include "MainDNIpch.h"
#include "PipelineCacheHeaders.h"

#include <Miscellaneous/Bool64.h>

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
		deleted = Misc::BOOL64_FALSE;
	}

}