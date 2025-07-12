#pragma once

namespace JJs2DEngine
{
	struct PipelineListInitializationData;

	class PipelineListInternal
	{
	public:
		PipelineListInternal(const PipelineListInitializationData& initData);
		~PipelineListInternal();

	private:
		uint64_t _stump;
	};
}