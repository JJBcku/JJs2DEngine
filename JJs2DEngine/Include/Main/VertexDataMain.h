#pragma once

namespace JJs2DEngine
{
	class VertexDataMainInternal;

	class VertexDataMain
	{
	public:
		VertexDataMain(VertexDataMainInternal& ref);
		~VertexDataMain();

	private:
		VertexDataMainInternal& _internal;
	};
}