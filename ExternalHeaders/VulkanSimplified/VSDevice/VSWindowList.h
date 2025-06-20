#pragma once

#include <CustomLists\IDObjectDef.h>

#include "VSWindowListDef.h"

namespace VulkanSimplified
{
	class WindowListInternal;
}

namespace VulkanSimplified
{
	struct WindowCreationData;
	class Window;

	class WindowList
	{
	public:
		WindowList(WindowListInternal& ref);
		~WindowList();

		IDObject<WindowPointer> CreateWindow(const WindowCreationData& creationData, size_t addOnReserving = 0);

		Window GetWindow(IDObject<WindowPointer> windowID);

		const Window GetWindow(IDObject<WindowPointer> windowID) const;

	private:
		WindowListInternal& _internal;
	};
}
