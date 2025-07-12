#include "MainDNIpch.h"
#include "PipelineListInternal.h"

#include "../../../Include/Main/PipelineListInitializationData.h"

namespace JJs2DEngine
{
	PipelineListInternal::PipelineListInternal(const PipelineListInitializationData& initData) : _stump(initData.stump)
	{
	}

	PipelineListInternal::~PipelineListInternal()
	{
	}

}