#include "MainDNIpch.h"
#include "InputDataListInternal.h"

namespace JJs2DEngine
{
	InputDataListInternal::InputDataListInternal(std::chrono::high_resolution_clock::time_point currentTime, VS::SdlEventHandler eventHandler) : _eventHandler(eventHandler),
		_currentTime(currentTime)
	{
	}

	InputDataListInternal::~InputDataListInternal()
	{
	}

	void InputDataListInternal::UpdateCurrentTime(std::chrono::high_resolution_clock::time_point currentTime)
	{
		_currentTime = currentTime;
	}

}