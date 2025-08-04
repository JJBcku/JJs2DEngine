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

	bool quit = false;

	while (main.IsWindowClosed() != Misc::BOOL64_TRUE && !quit)
	{
		main.UpdateCurrentTime();
		main.HandleEvents();

		quit = !keyList.ESCkey.GetKeyPressList().empty();
		inputData.ResetKeyPresses();
	}

	main.WaitForIdleDevice();
}
