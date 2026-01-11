#include "MainIpch.h"
#include "../../../Include/Main/VertexLayerOrderID.h"

namespace JJs2DEngine
{
	VertexLayerOrderID::VertexLayerOrderID() noexcept
	{
		type = VertexLayerOrderIDType::UNSET;
	}

	VertexLayerOrderID::VertexLayerOrderID(const VertexLayerOrderID& rhs) noexcept
	{
		std::memcpy(this, &rhs, sizeof(VertexLayerOrderID));
	}

	VertexLayerOrderID::VertexLayerOrderID(VertexLayerOrderID&& rhs) noexcept
	{
		std::memcpy(this, &rhs, sizeof(VertexLayerOrderID));
		rhs.type = VertexLayerOrderIDType::UNSET;
	}

	VertexLayerOrderID::VertexLayerOrderID(IDObject<UiVertexDataLayerVersionListPointer> ID) noexcept
	{
		uiLayerID.type = VertexLayerOrderIDType::UI_LAYER;
		uiLayerID.ID = ID;
	}

	VertexLayerOrderID::VertexLayerOrderID(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID) noexcept
	{
		worldLayerID.type = VertexLayerOrderIDType::WORLD_LAYER;
		worldLayerID.ID = ID;
	}

	VertexLayerOrderID::~VertexLayerOrderID()
	{
	}

	VertexLayerOrderID& VertexLayerOrderID::operator=(const VertexLayerOrderID& rhs) noexcept
	{
		std::memcpy(this, &rhs, sizeof(VertexLayerOrderID));
		return *this;
	}

	VertexLayerOrderID& VertexLayerOrderID::operator=(VertexLayerOrderID&& rhs) noexcept
	{
		std::memcpy(this, &rhs, sizeof(VertexLayerOrderID));
		rhs.type = VertexLayerOrderIDType::UNSET;
		return *this;
	}

}