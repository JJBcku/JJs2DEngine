#include "pch.h"
#include "RunProgram.h"

#include "MainDataCollection.h"

#include "CreateMainClass.h"
#include "CreateTextures.h"
#include "CreateLayers.h"

#include <Main.h>
#include <InputDataList.h>
#include <KeyPressData.h>
#include <SdlScancode.h>
#include <VertexDataMain.h>

#include <Miscellaneous/Bool64.h>

#include <chrono>
#include <thread>
#include <iostream>

float GetRotation();

void RunProgram()
{
	MainDataCollection data;

	CreateMainClass(data);
	CreateTextures(data);
	CreateLayers(data);

	auto& main = *data.main;
	auto vertexDataMain = main.GetVertexDataMainList();

	auto inputData = main.GetInputDataList();
	const auto& escKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_ESCAPE);
	const auto& fullscreenKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_F10);

	bool quit = false;

	Misc::Bool64Values fullscreen = Misc::BOOL64_FALSE;

	size_t framesThisSecond = 0;
	float lastSecond = 0.0;

	while (main.IsWindowClosed() != Misc::BOOL64_TRUE && !quit)
	{
		main.UpdateCurrentTime();
		main.HandleEvents();

		quit = !escKey.GetKeyPressList().empty();

		if (main.RenderingShouldBePaused())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
		else
		{
			vertexDataMain.SetCameraRotation(GetRotation());
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

		if (!fullscreenKey.GetKeyPressList().empty())
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

float GetRotation()
{
	float ret = 0.0f;

	static const auto startTime = std::chrono::high_resolution_clock::now();

	auto currentTime = std::chrono::high_resolution_clock::now();

	double rotationTime = std::chrono::duration<double, std::chrono::seconds::period>(currentTime - startTime).count() * 30;

	double fullRotationCountDouble = rotationTime / 360.0;
	uint64_t fullRotationCountUint = static_cast<uint64_t>(fullRotationCountDouble);

	double rotationDouble = rotationTime - (360.0 * static_cast<double>(fullRotationCountUint));
	ret = static_cast<float>(rotationDouble);

	return ret;
}
