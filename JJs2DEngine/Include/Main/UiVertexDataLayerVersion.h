#pragma once

#include <optional>

namespace JJs2DEngine
{
	class UiVertexDataLayerVersionInternal;

	struct UiObjectData;

	class UiVertexDataLayerVersion
	{
	public:
		UiVertexDataLayerVersion(UiVertexDataLayerVersionInternal& ref);
		~UiVertexDataLayerVersion();

		std::optional<size_t> AddObject(const UiObjectData& newObjectData);

	private:
		UiVertexDataLayerVersionInternal& _internal;
	};
}
