#include "pch.h"
#include "RunProgram.h"

#include "MainDataCollection.h"

#include "CreateMainClass.h"
#include "CreateTexture.h"
#include "CreateLayers.h"

#include <Main.h>
#include <InputDataList.h>
#include <SpecialKeysDataList.h>

#include <Miscellaneous/Bool64.h>

#include <chrono>
#include <thread>
#include <iostream>

void RunProgram()
{
	MainDataCollection data;

	CreateMainClass(data);
	CreateTexture(data);
	CreateLayers(data);

	auto& main = *data.main;

	auto inputData = main.GetInputDataList();
	const auto& keyList = inputData.GetSpecialKeyList();
	const auto& fullscreenKey = keyList.f10Key.GetKeyPressList();

	bool quit = false;

	Misc::Bool64Values fullscreen = Misc::BOOL64_FALSE;

	size_t framesThisSecond = 0;
	float lastSecond = 0.0;

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
			main.RenderSingleFrame();

			framesThisSecond++;

			static const auto startTime = std::chrono::high_resolution_clock::now();

			auto currentTime = std::chrono::high_resolution_clock::now();

			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

			if (time - lastSecond >= 1.0f)
			{
				lastSecond = time;
				std::cout << "FPS: " << framesThisSecond << std::endl;
				framesThisSecond = 0;
			}
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
