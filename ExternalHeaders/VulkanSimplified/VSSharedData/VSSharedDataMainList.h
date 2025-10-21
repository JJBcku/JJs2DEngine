#pragma once

namespace VulkanSimplified
{
	class SharedDataMainListInternal;
}

namespace VulkanSimplified
{
	class SharedDescriptorDataList;
	class SharedRenderPassDataLists;
	class SharedPipelineDataList;

	class SharedDataMainList
	{
	public:
		SharedDataMainList(SharedDataMainListInternal& ref);
		~SharedDataMainList();

		SharedDescriptorDataList GetSharedDescriptorDataList();
		SharedRenderPassDataLists GetSharedRenderPassDataLists();
		SharedPipelineDataList GetSharedPipelineDataLists();

		const SharedDescriptorDataList GetSharedDescriptorDataList() const;
		const SharedRenderPassDataLists GetSharedRenderPassDataLists() const;
		const SharedPipelineDataList GetSharedPipelineDataLists() const;

	private:
		SharedDataMainListInternal& _internal;
	};
}