#pragma once

#include <optional>

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionInternal;

	struct ObjectData;

	class UiVertexDataLayerVersion
	{
	public:
		UiVertexDataLayerVersion(UiVertexDataLayerVersionInternal& ref);
		~UiVertexDataLayerVersion();

		std::optional<size_t> AddObject(const ObjectData& newObjectData);

	private:
		UiVertexDataLayerVersionInternal& _internal;
	};
}
