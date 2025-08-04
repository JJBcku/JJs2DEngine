#include "pch.h"
#include "RunProgram.h"

#include <Miscellaneous/Bool64.h>

#include <Main.h>
#include <InputDataList.h>
#include <SpecialKeysDataList.h>

#include "MainDataCollection.h"

#include "CreateMainClass.h"

void RunProgram()
{
	MainDataCollection data;

	CreateMainClass(data);

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
