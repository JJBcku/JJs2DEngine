#include "MainIpch.h"
#include "../../../Include/Main/Main.h"

#include "../../DNIData/Main/MainInternal.h"

namespace JJ2DE
{
	Main::Main(const MainInitializationData& initData) : _internal(std::make_unique<MainInternal>(initData))
	{
	}

	Main::~Main()
	{
	}

}