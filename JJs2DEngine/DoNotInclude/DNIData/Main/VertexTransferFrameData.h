#pragma once

#include <Miscellaneous/Bool64Def.h>
#include <VulkanSimplified/VSDevice/VSDataBufferLists.h>

#include <CustomLists/IDObject.h>

namespace JJs2DEngine
{
	struct VertexTransferFrameData
	{
		Misc::Bool64 changed;
		size_t lastAmountWrittenToVertexBuffer;
		IDObject<VS::AutoCleanupVertexBuffer> vertexBuffer;

		VertexTransferFrameData();
		~VertexTransferFrameData();
	};
}