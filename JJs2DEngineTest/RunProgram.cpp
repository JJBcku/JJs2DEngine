#include "pch.h"
#include "RunProgram.h"

#include "MainDataCollection.h"

#include "CreateMainClass.h"
#include "CreateTextures.h"
#include "CreateLayers.h"

#include <Main.h>
#include <InputDataList.h>
#include <InputEvent.h>
#include <VertexDataMain.h>
#include <WorldLayerVertexDataLayerVersionList.h>
#include <WorldLayerVertexDataLayerVersion.h>
#include <UiVertexDataLayerVersionList.h>
#include <UiVertexDataLayerVersion.h>

#include <chrono>
#include <thread>
#include <iostream>
#include <optional>
#include <algorithm>

constexpr float changeRatio = 0.25f;

float GetPreviousZoomValue(float currentZoom);
float GetNextZoomValue(float currentZoom);

struct KeyPressList
{
	uint64_t escKey = 0;
	uint64_t f10Key = 0;

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

struct MouseDataList
{
	bool leftClick = false;
	bool rightClick = false;

	uint32_t leftDoubleClicks = 0;
	uint32_t rightDoubleClicks = 0;

	std::optional<float> positionX;
	std::optional<float> positionY;

	float positionChangeX = 0.0f;
	float positionChangeY = 0.0f;

	std::optional<float> lastPositionX;
	std::optional<float> lastPositionY;

	MouseDataList() {};
	~MouseDataList() {};
};

void HandleKeyPress(KeyPressList& keyPressData, JJ2DE::KeyEventData keyPress);
void HandleMouseMovement(const JJ2DE::Main& main, MouseDataList& mouseDataList, JJ2DE::MouseMotionEvent mouseEvent);
void HandleMouseButtons(const JJ2DE::Main& main, MouseDataList& mouseDataList, JJ2DE::MouseButtonEvent mouseEvent);

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
	MouseDataList mouseDataList;

	auto UILayer = vertexDataMain.GetUiVertexDataLayerVersionList(data.UILayerID).GetLayersVersion(0);
	auto worldLayer = vertexDataMain.GetWorldLayerVertexDataLayerVersionList(data.worldLayerID).GetLayersVersion(0);

	bool quit = false;

	bool fullscreen = false;

	size_t framesThisSecond = 0;
	float lastSecond = 0.0;

	static const auto startTime = std::chrono::high_resolution_clock::now();
	auto lastFrameTime = startTime;

	while (!main.IsWindowClosed() && !quit)
	{
		main.UpdateCurrentTime();
		main.HandleEvents();

		const auto& eventDataList = inputData.GetEventList();

		if (inputData.FocusWasLost())
		{
			keyPressList = KeyPressList();
			mouseDataList = MouseDataList();
			inputData.ResetFocusLost();
		}
		else
		{
			for (size_t i = 0; i < eventDataList.size(); ++i)
			{
				if (eventDataList[i].type == JJ2DE::InputEventsType::KEY_EVENT)
					HandleKeyPress(keyPressList, eventDataList[i].keyEvent.data);
				else if (eventDataList[i].type == JJ2DE::InputEventsType::MOUSE_MOTION_EVENT)
					HandleMouseMovement(main, mouseDataList, eventDataList[i].mouseMotionEvent.data);
				else if (eventDataList[i].type == JJ2DE::InputEventsType::MOUSE_BUTTON_EVENT)
					HandleMouseButtons(main, mouseDataList, eventDataList[i].mouseButtonEvent.data);
			}
		}
		inputData.ClearEventList();

		quit = keyPressList.escKey > 0;

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

			bool cursorOnUIObject = mouseDataList.positionY <= data.UITextureLowerY && mouseDataList.positionY >= data.UITextureUpperY;

			if (cursorOnUIObject)
			{
				if (mouseDataList.leftClick && data.currentUIObjectTexture != 2)
				{
					UILayer.ChangeObjectsTexture(false, data.UITexturesIDs[2], data.UIObjectID);
					data.currentUIObjectTexture = 2;
				}
				else if (!mouseDataList.leftClick && data.currentUIObjectTexture != 1)
				{
					UILayer.ChangeObjectsTexture(false, data.UITexturesIDs[1], data.UIObjectID);
					data.currentUIObjectTexture = 1;
				}
			}
			else if (data.currentUIObjectTexture != 0)
			{
				UILayer.ChangeObjectsTexture(false, data.UITexturesIDs[0], data.UIObjectID);
				data.currentUIObjectTexture = 0;
			}

			keyPressList.zKey += mouseDataList.leftDoubleClicks;
			keyPressList.xKey += mouseDataList.rightDoubleClicks;

			if (keyPressList.zKey >= keyPressList.xKey)
			{
				keyPressList.zKey -= keyPressList.xKey;
				keyPressList.xKey = 0;
			}
			else
			{
				keyPressList.xKey -= keyPressList.zKey;
				keyPressList.zKey = 0;
			}

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

			if (!mouseDataList.leftClick && mouseDataList.lastPositionX.has_value())
			{
				data.cameraX -= mouseDataList.positionChangeX;
				data.cameraY -= mouseDataList.positionChangeY;

				mouseDataList.positionChangeX = 0.0f;
				mouseDataList.positionChangeY = 0.0f;

				mouseDataList.lastPositionX.reset();
				mouseDataList.lastPositionY.reset();
			}

			float maxCameraOffset = 0.5f;
			float minCameraOffset = -0.5f;

			if (keyPressList.sKey || keyPressList.downKey)
			{
				data.cameraY += changeRatio * frameDelta;
			}
			else if (keyPressList.wKey || keyPressList.upKey)
			{
				data.cameraY -= changeRatio * frameDelta;
			}

			if (keyPressList.dKey || keyPressList.rightKey)
			{
				data.cameraX += changeRatio * frameDelta;
			}
			else if (keyPressList.aKey || keyPressList.leftKey)
			{
				data.cameraX -= changeRatio * frameDelta;
			}

			if (data.cameraY > maxCameraOffset)
				data.cameraY = maxCameraOffset;
			if (data.cameraY < minCameraOffset)
				data.cameraY = minCameraOffset;

			if (data.cameraX > maxCameraOffset)
				data.cameraX = maxCameraOffset;
			if (data.cameraX < minCameraOffset)
				data.cameraX = minCameraOffset;

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

			vertexDataMain.SetCameraPosition(std::clamp(data.cameraX - mouseDataList.positionChangeX, minCameraOffset, maxCameraOffset),
				std::clamp(data.cameraY - mouseDataList.positionChangeY, minCameraOffset, maxCameraOffset));
			vertexDataMain.SetPerspectiveRotation(data.rotation);
			vertexDataMain.SetCameraZoom(data.zoom);

			lastFrameTime = currentTime;
			main.RenderSingleFrame();

			if (time - lastSecond >= 1.0f)
			{
				data.currentWorldObjectTexture += 1;
				if (data.currentWorldObjectTexture >= data.worldTexturesIDs.size())
					data.currentWorldObjectTexture = 0;

				worldLayer.ChangeObjectsTexture(false, data.worldTexturesIDs[data.currentWorldObjectTexture], data.worldObjectID);

				lastSecond = time;
				std::cout << "FPS: " << framesThisSecond << std::endl;
				framesThisSecond = 0;
			}
		}

		if (keyPressList.f10Key > 0)
		{
			if (fullscreen)
				fullscreen = false;
			else
				fullscreen = true;

			main.ChangeFullscreen(fullscreen);
		}

		keyPressList.escKey = 0;
		keyPressList.f10Key = 0;

		keyPressList.zKey = 0;
		keyPressList.xKey = 0;

		mouseDataList.leftDoubleClicks = 0;
		mouseDataList.rightDoubleClicks = 0;
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

void HandleKeyPress(KeyPressList& keyPressData, JJ2DE::KeyEventData keyPress)
{
	switch (keyPress.scanCode)
	{
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_ESCAPE:
		if (!keyPress.keyPressed)
			keyPressData.escKey += 1;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_F10:
		if (!keyPress.keyPressed)
			keyPressData.f10Key += 1;
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
		keyPressData.zKey += 1;
		break;
	case JJ2DE::SdlScancode::SDL_SCANCODE_MODULE_X:
		if (!keyPress.keyPressed || keyPress.keyRepeat)
			break;
		keyPressData.xKey += 1;
		break;
	default:
		break;
	}
}

void HandleMouseMovement(const JJ2DE::Main& main, MouseDataList& mouseDataList, JJ2DE::MouseMotionEvent mouseEvent)
{
	mouseDataList.positionX = main.TranslatePositionXInPixelsToWindowSizeRatio(mouseEvent.positionX);
	mouseDataList.positionY = main.TranslatePositionYInPixelsToWindowSizeRatio(mouseEvent.positionY);

	mouseDataList.positionX = mouseDataList.positionX.value() * 2.0f - 1.0f;
	mouseDataList.positionY = mouseDataList.positionY.value() * 2.0f - 1.0f;

	if (!mouseDataList.leftClick)
	{
		return;
	}

	if (mouseDataList.lastPositionX.has_value())
	{
		mouseDataList.positionChangeX = mouseDataList.positionX.value() - mouseDataList.lastPositionX.value();
		mouseDataList.positionChangeY = mouseDataList.positionY.value() - mouseDataList.lastPositionY.value();
	}
}

void HandleMouseButtons(const JJ2DE::Main& main, MouseDataList& mouseDataList, JJ2DE::MouseButtonEvent mouseEvent)
{
	float positionRatioX = main.TranslatePositionXInPixelsToWindowSizeRatio(mouseEvent.mousePositionX);
	float positionRatioY = main.TranslatePositionYInPixelsToWindowSizeRatio(mouseEvent.mousePositionY);

	positionRatioX = positionRatioX * 2.0f - 1.0f;
	positionRatioY = positionRatioY * 2.0f - 1.0f;

	switch (mouseEvent.buttonIndex)
	{
	case JJ2DE::SdlMouseButtonIndex::SDL_DATA_BUTTON_INDEX_LEFT:
		if (mouseEvent.buttonPressed && !mouseDataList.leftClick)
		{
			mouseDataList.lastPositionX = positionRatioX;
			mouseDataList.lastPositionY = positionRatioY;
		}
		if (mouseDataList.leftClick && mouseEvent.doubleClick)
			mouseDataList.leftDoubleClicks += 1;
		mouseDataList.leftClick = mouseEvent.buttonPressed;
		break;
	case JJ2DE::SdlMouseButtonIndex::SDL_DATA_BUTTON_INDEX_RIGHT:
		if (mouseDataList.rightClick && mouseEvent.doubleClick)
			mouseDataList.rightDoubleClicks += 1;
		mouseDataList.rightClick = mouseEvent.buttonPressed;
		break;
	default:
		break;
	}
}
