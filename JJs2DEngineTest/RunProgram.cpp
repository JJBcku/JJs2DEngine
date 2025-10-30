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

constexpr float changeRatio = 0.25f;

float GetPreviousZoomValue(float currentZoom);
float GetNextZoomValue(float currentZoom);

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

	const auto& wKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_W);
	const auto& aKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_A);
	const auto& sKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_S);
	const auto& dKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_D);

	const auto& upKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_UP);
	const auto& rightKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_RIGHT);
	const auto& downKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_DOWN);
	const auto& leftKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_LEFT);

	const auto& eKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_E);
	const auto& qKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_Q);
	const auto& zKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_Z);
	const auto& xKey = inputData.GetKeyPressData(JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_X);

	bool quit = false;

	Misc::Bool64Values fullscreen = Misc::BOOL64_FALSE;

	size_t framesThisSecond = 0;
	float lastSecond = 0.0;

	static const auto startTime = std::chrono::high_resolution_clock::now();
	auto lastFrameTime = startTime;

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
			framesThisSecond++;

			auto currentTime = std::chrono::high_resolution_clock::now();

			float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
			float frameDelta = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastFrameTime).count();

			for (size_t i = 0; i < zKey.GetKeyPressList().size(); ++i)
			{
				float newZoom = GetNextZoomValue(data.zoom);
				float change = newZoom / data.zoom;

				data.cameraX *= change;
				data.cameraY *= change;

				data.zoom = newZoom;
			}

			for (size_t i = 0; i < xKey.GetKeyPressList().size(); ++i)
			{
				float newZoom = GetPreviousZoomValue(data.zoom);
				float change = newZoom / data.zoom;

				data.cameraX *= change;
				data.cameraY *= change;

				data.zoom = newZoom;
			}

			float maxCameraOffset = 0.25f * data.zoom;
			float minCameraOffset = -0.25f * data.zoom;

			if (sKey.GetCurrentKeyPressBegginingTime().has_value() || downKey.GetCurrentKeyPressBegginingTime().has_value())
			{
				data.cameraY += changeRatio * frameDelta;
				if (data.cameraY > maxCameraOffset)
					data.cameraY = maxCameraOffset;
			}
			else if (wKey.GetCurrentKeyPressBegginingTime().has_value() || upKey.GetCurrentKeyPressBegginingTime().has_value())
			{
				data.cameraY -= changeRatio * frameDelta;
				if (data.cameraY < minCameraOffset)
					data.cameraY = minCameraOffset;
			}

			if (dKey.GetCurrentKeyPressBegginingTime().has_value() || rightKey.GetCurrentKeyPressBegginingTime().has_value())
			{
				data.cameraX += changeRatio * frameDelta;
				if (data.cameraX > maxCameraOffset)
					data.cameraX = maxCameraOffset;
			}
			else if (aKey.GetCurrentKeyPressBegginingTime().has_value() || leftKey.GetCurrentKeyPressBegginingTime().has_value())
			{
				data.cameraX -= changeRatio * frameDelta;
				if (data.cameraX < minCameraOffset)
					data.cameraX = minCameraOffset;
			}

			if (qKey.GetCurrentKeyPressBegginingTime().has_value())
			{
				data.rotation += changeRatio * frameDelta * 360.0f;
				while (data.rotation >= 360.0f)
					data.rotation -= 360.0f;
			}
			else if (eKey.GetCurrentKeyPressBegginingTime().has_value())
			{
				data.rotation -= changeRatio * frameDelta * 360.0f;
				while (data.rotation <= 0.0f)
					data.rotation += 360.0f;
			}

			vertexDataMain.SetCameraPosition(data.cameraX, data.cameraY);
			vertexDataMain.SetCameraRotation(data.rotation);
			vertexDataMain.SetCameraZoom(data.zoom);

			lastFrameTime = currentTime;
			main.RenderSingleFrame();

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

float GetPreviousZoomValue(float currentZoom)
{
	float ret = 1.0f;

	if (currentZoom <= 1.0f)
		ret = 0.5f;

	return ret;
}

float GetNextZoomValue(float currentZoom)
{
	float ret = 1.0f;

	if (currentZoom >= 1.0f)
		ret = 2.0f;

	return ret;
}
