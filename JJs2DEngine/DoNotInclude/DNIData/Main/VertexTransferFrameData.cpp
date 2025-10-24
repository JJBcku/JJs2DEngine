#include "MainDNIpch.h"
#include "VertexTransferFrameData.h"

#include <Miscellaneous/Bool64.h>

namespace JJs2DEngine
{
	VertexTransferFrameData::VertexTransferFrameData()
	{
		changed = Misc::BOOL64_FALSE;
		lastAmountWrittenToVertexBuffer = 0;
	}

	JJs2DEngine::VertexTransferFrameData::~VertexTransferFrameData()
	{
	}
}