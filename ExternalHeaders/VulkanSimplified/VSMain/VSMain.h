#pragma once

#include <memory>

namespace VulkanSimplified
{
	class MainInternal;
}

namespace VulkanSimplified
{
	struct MainInitData;
	struct InstanceExtensionPacksList;
	struct InstanceLayerPacksList;

	struct InstanceCreationData;

	class SharedDataMainList;
	class SdlEventHandler;
	class Instance;

	class VersionData;

	class Main
	{
	public:
		Main(const MainInitData& initData);
		~Main();

		Main(const Main&) noexcept = delete;
		Main(Main&& rhs) noexcept;

		Main& operator=(const Main&) noexcept = delete;
		Main& operator=(Main&& rhs) noexcept;

		void CreateInstance(const InstanceCreationData& instanceInit);

		SdlEventHandler GetSdlEventHandler();
		SharedDataMainList GetSharedDataMainList();
		Instance GetInstance();

		const SdlEventHandler GetSdlEventHandler() const;
		const SharedDataMainList GetSharedDataMainList() const;
		const Instance GetInstance() const;

		VersionData GetMaxAvailableVulkanVersion() const;

		InstanceExtensionPacksList GetAvailableInstanceExtensionPacks() const;
		InstanceLayerPacksList GetAvailableInstanceLayerPacks() const;

	private:
		std::unique_ptr<MainInternal> _internal;
	};
}
