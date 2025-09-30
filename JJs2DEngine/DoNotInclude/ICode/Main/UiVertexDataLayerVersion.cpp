#include "MainIpch.h"
#include "../../../Include/Main/UiVertexDataLayerVersion.h"

#include "../../DNIData/Main/UiVertexDataLayerVersionInternal.h"

namespace JJs2DEngine
{
	UiVertexDataLayerVersion::UiVertexDataLayerVersion(UiVertexDataLayerVersionInternal& ref) : _internal(ref)
	{
	}

	UiVertexDataLayerVersion::~UiVertexDataLayerVersion()
	{
	}

	std::optional<size_t> UiVertexDataLayerVersion::AddObject(const UiObjectData& newObjectData)
	{
		return _internal.AddObject(newObjectData);
	}

}