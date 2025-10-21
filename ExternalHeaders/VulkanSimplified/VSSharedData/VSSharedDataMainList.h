#pragma once

namespace VulkanSimplified
{
	class SharedDataMainListInternal;
}

namespace VulkanSimplified
{
	class SharedDescriptorDataList;
	class SharedRenderPassDataLists;
	class SharedPipelineDataLists;

	class SharedDataMainList
	{
	public:
		SharedDataMainList(SharedDataMainListInternal& ref);
		~SharedDataMainList();

		SharedDescriptorDataList GetSharedDescriptorDataList();
		SharedRenderPassDataLists GetSharedRenderPassDataLists();
		SharedPipelineDataLists GetSharedPipelineDataLists();

		const SharedDescriptorDataList GetSharedDescriptorDataList() const;
		const SharedRenderPassDataLists GetSharedRenderPassDataLists() const;
		const SharedPipelineDataLists GetSharedPipelineDataLists() const;

	private:
		SharedDataMainListInternal& _internal;
	};
}