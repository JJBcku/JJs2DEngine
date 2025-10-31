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

struct KeyPressList
{
	bool escKey = false;
	bool f10Key = false;

	bool wKey = false;
	bool aKey = false;
	bool sKey = false;
	bool dKey = false;

	bool upKey = false;
	bool rightKey = false;
	bool downKey = false;
	bool leftKey = false;

	bool eKey = false;
	bool qKey = false;
	uint64_t zKey = 0;
	uint64_t xKey = 0;

	KeyPressList() {};
	~KeyPressList() {};
};

void HandleKeyPress(KeyPressList& keyPressData, JJ2DE::KeyPressData keyPress);

void RunProgram()
{
	MainDataCollection data;

	CreateMainClass(data);
	CreateTextures(data);
	CreateLayers(data);

	auto& main = *data.main;
	auto vertexDataMain = main.GetVertexDataMainList();

	auto inputData = main.GetInputDataList();
	KeyPressList keyPressList;

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

		quit = keyPressList.escKey;

		const auto& eventDataList = inputData.GetEventList();

		for (size_t i = 0; i < eventDataList.size(); ++i)
		{
			HandleKeyPress(keyPressList, eventDataList[i]);
		}
		inputData.ClearEventList();

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

			for (uint64_t i = 0; i < keyPressList.zKey; ++i)
			{
				float newZoom = GetNextZoomValue(data.zoom);

				data.zoom = newZoom;
			}

			for (uint64_t i = 0; i < keyPressList.xKey; ++i)
			{
				float newZoom = GetPreviousZoomValue(data.zoom);

				data.zoom = newZoom;
			}

			float maxCameraOffset = 0.5f;
			float minCameraOffset = -0.5f;

			if (keyPressList.sKey || keyPressList.downKey)
			{
				data.cameraY += changeRatio * frameDelta;
				if (data.cameraY > maxCameraOffset)
					data.cameraY = maxCameraOffset;
			}
			else if (keyPressList.wKey || keyPressList.upKey)
			{
				data.cameraY -= changeRatio * frameDelta;
				if (data.cameraY < minCameraOffset)
					data.cameraY = minCameraOffset;
			}

			if (keyPressList.dKey || keyPressList.rightKey)
			{
				data.cameraX += changeRatio * frameDelta;
				if (data.cameraX > maxCameraOffset)
					data.cameraX = maxCameraOffset;
			}
			else if (keyPressList.aKey || keyPressList.leftKey)
			{
				data.cameraX -= changeRatio * frameDelta;
				if (data.cameraX < minCameraOffset)
					data.cameraX = minCameraOffset;
			}

			if (keyPressList.eKey)
			{
				data.rotation += changeRatio * frameDelta * 360.0f;
				while (data.rotation >= 360.0f)
					data.rotation -= 360.0f;
			}
			else if (keyPressList.qKey)
			{
				data.rotation -= changeRatio * frameDelta * 360.0f;
				while (data.rotation <= 0.0f)
					data.rotation += 360.0f;
			}

			vertexDataMain.SetCameraPosition(data.cameraX, data.cameraY);
			vertexDataMain.SetPerspectiveRotation(data.rotation);
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

		if (keyPressList.f10Key)
		{
			if (fullscreen == Misc::BOOL64_FALSE)
				fullscreen = Misc::BOOL64_TRUE;
			else
				fullscreen = Misc::BOOL64_FALSE;

			main.ChangeFullscreen(fullscreen);
		}

		keyPressList.zKey = 0;
		keyPressList.xKey = 0;
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

void HandleKeyPress(KeyPressList& keyPressData, JJ2DE::KeyPressData keyPress)
{
	switch (keyPress.scanCode)
	{
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_ESCAPE:
		keyPressData.escKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_F10:
		keyPressData.f10Key = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_W:
		keyPressData.wKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_A:
		keyPressData.aKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_S:
		keyPressData.sKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_D:
		keyPressData.dKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_UP:
		keyPressData.upKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_RIGHT:
		keyPressData.rightKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_DOWN:
		keyPressData.downKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_LEFT:
		keyPressData.leftKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_E:
		keyPressData.eKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_Q:
		keyPressData.qKey = keyPress.keyPressed;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_Z:
		if (!keyPress.keyPressed || keyPress.keyRepeat)
			break;
		if (keyPressData.xKey > 0)
			keyPressData.xKey -= 1;
		else
			keyPressData.zKey += 1;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_X:
		if (!keyPress.keyPressed || keyPress.keyRepeat)
			break;
		if (keyPressData.zKey > 0)
			keyPressData.zKey -= 1;
		else
			keyPressData.xKey += 1;
		break;
	default:
		break;
	}
}
