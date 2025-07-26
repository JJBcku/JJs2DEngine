#pragma once

#include <Miscellaneous/Bool64Def.h>
#include <CustomLists/IDObject.h>

#include <VulkanSimplified/VSCommon/VSDataFormatFlags.h>

#include <VulkanSimplified/VSDevice/VSWindowListDef.h>

namespace JJs2DEngine
{
	struct WindowInitializationData;

	class WindowDataInternal
	{
	public:
		WindowDataInternal(const WindowInitializationData& initData, uint32_t framesInFlight, VS::DataFormatSetIndependentID format, VS::WindowList& windowList);
		~WindowDataInternal();

		void ChangeSwapchainFormat(VS::DataFormatSetIndependentID newFormat);

	private:
		VS::WindowList& _windowList;

		std::string _windowTitle;
		uint32_t _windowWidth;
		uint32_t _windowHeight;

		uint64_t _framesInFlight;
		VS::DataFormatSetIndependentID _format;

		Misc::Bool64 _fullscreen;

		IDObject<VS::WindowPointer> _windowID;
	};
}
