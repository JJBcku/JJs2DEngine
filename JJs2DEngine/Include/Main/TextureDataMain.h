#pragma once

namespace JJs2DEngine
{
	class TextureDataMainInternal;

	class TextureDataMain
	{
	public:
		TextureDataMain(TextureDataMainInternal& ref);
		~TextureDataMain();

	private:
		TextureDataMainInternal& _internal;
	};
}
