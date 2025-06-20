#pragma once

namespace VulkanSimplified
{
	class SharedDataMainListInternal;
}

namespace VulkanSimplified
{
	class SharedDescriptorDataList;
	class SharedRenderPassDataList;
	class SharedPipelineDataList;

	class SharedDataMainList
	{
	public:
		SharedDataMainList(SharedDataMainListInternal& ref);
		~SharedDataMainList();

		SharedDescriptorDataList GetSharedDescriptorDataList();
		SharedRenderPassDataList GetSharedRenderPassDataList();
		SharedPipelineDataList GetSharedPipelineDataListss();

		const SharedDescriptorDataList GetSharedDescriptorDataList() const;
		const SharedRenderPassDataList GetSharedRenderPassDataList() const;
		const SharedPipelineDataList GetSharedPipelineDataListss() const;

	private:
		SharedDataMainListInternal& _internal;
	};
}