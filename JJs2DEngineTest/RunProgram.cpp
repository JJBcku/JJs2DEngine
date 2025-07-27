#include "pch.h"
#include "RunProgram.h"

#include <Miscellaneous/Bool64.h>

#include <Main.h>

#include "MainDataCollection.h"

#include "CreateMainClass.h"

void RunProgram()
{
	MainDataCollection data;

	CreateMainClass(data);

	auto& main = *data.main;

	while (main.IsWindowClosed() != Misc::BOOL64_TRUE)
	{
		main.HandleEvents();
	}

	main.WaitForIdleDevice();
}
