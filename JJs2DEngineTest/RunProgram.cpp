#include "pch.h"
#include "RunProgram.h"

#include "MainDataCollection.h"

#include "CreateMainClass.h"
#include "CreateLayers.h"

#include <Main.h>
#include <InputDataList.h>
#include <SpecialKeysDataList.h>

#include <Miscellaneous/Bool64.h>

#include <chrono>
#include <thread>

void RunProgram()
{
	MainDataCollection data;

	CreateMainClass(data);
	CreateLayers(data);

	auto& main = *data.main;

	auto inputData = main.GetInputDataList();
	const auto& keyList = inputData.GetSpecialKeyList();
	const auto& fullscreenKey = keyList.f10Key.GetKeyPressList();

	bool quit = false;

	Misc::Bool64Values fullscreen = Misc::BOOL64_FALSE;

	while (main.IsWindowClosed() != Misc::BOOL64_TRUE && !quit)
	{
		main.UpdateCurrentTime();
		main.HandleEvents();

		quit = !keyList.ESCkey.GetKeyPressList().empty();

		if (main.RenderingShouldBePaused())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else
		{
			main.RunSingleFrame();
		}

		if (!fullscreenKey.empty())
		{
			if (fullscreen == Misc::BOOL64_FALSE)
				fullscreen = Misc::BOOL64_TRUE;
			else
				fullscreen = Misc::BOOL64_FALSE;

			main.ChangeFullscreen(fullscreen);
		}

		inputData.ClearKeyPressesLists();
	}

	main.WaitForIdleDevice();
}
