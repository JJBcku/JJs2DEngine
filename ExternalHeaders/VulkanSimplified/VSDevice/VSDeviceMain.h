#pragma once

namespace VulkanSimplified
{
	class DeviceMainInternal;
}

namespace VulkanSimplified
{
	class DeviceCore;
	class WindowList;
	class ShaderLists;

	class DescriptorDataLists;
	class RenderPassList;
	class PipelineDataLists;

	class MemoryObjectsList;
	class DataBufferLists;
	class ImageDataLists;

	class SynchronizationDataLists;
	class CommandPoolMainList;

	class DeviceMain
	{
	public:
		DeviceMain(DeviceMainInternal& ref);
		~DeviceMain();

		DeviceCore GetDeviceCore();
		WindowList GetWindowList();
		ShaderLists GetShaderLists();

		MemoryObjectsList GetMemoryObjectsList();
		DataBufferLists GetDataBufferLists();
		ImageDataLists GetImageDataLists();

		DescriptorDataLists GetDescriptorDataLists();
		RenderPassList GetRenderPassList();
		PipelineDataLists GetPipelineDataLists();

		SynchronizationDataLists GetSynchronizationDataLists();
		CommandPoolMainList GetCommandPoolMainList();

		const DeviceCore GetDeviceCore() const;
		const WindowList GetWindowList() const;
		const ShaderLists GetShaderLists() const;

		const MemoryObjectsList GetMemoryObjectsList() const;
		const DataBufferLists GetDataBufferLists() const;
		const ImageDataLists GetImageDataLists() const;

		const DescriptorDataLists GetDescriptorDataLists() const;
		const RenderPassList GetRenderPassList() const;
		const PipelineDataLists GetPipelineDataLists() const;

		const SynchronizationDataLists GetSynchronizationDataLists() const;
		const CommandPoolMainList GetCommandPoolMainList() const;

	private:
		DeviceMainInternal& _internal;
	};
}