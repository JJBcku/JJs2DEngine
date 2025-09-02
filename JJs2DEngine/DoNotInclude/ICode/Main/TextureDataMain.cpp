#include "MainIpch.h"
#include "../../../Include/Main/TextureDataMain.h"

#include "../../../DoNotInclude/DNIData/Main/TextureDataMainInternal.h"

namespace JJs2DEngine
{
	TextureDataMain::TextureDataMain(TextureDataMainInternal& ref) : _internal(ref)
	{
	}

	TextureDataMain::~TextureDataMain()
	{
	}

}