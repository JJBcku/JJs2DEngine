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

	VertexLayerOrderID::VertexLayerOrderID(const IDObject<UiVertexDataLayerVersionListPointer>& ID) noexcept : uiLayerID(ID)
	{
	}

	VertexLayerOrderID::VertexLayerOrderID(const IDObject<WorldLayerVertexDataLayerVersionListPointer>& ID) noexcept : worldLayerID(ID)
	{
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

	VertexLayerOrderID::UILayerID::UILayerID(const IDObject<UiVertexDataLayerVersionListPointer>& ID) noexcept : ID(ID)
	{
		type = VertexLayerOrderIDType::UI_LAYER;
	}

	VertexLayerOrderID::WorldLayerID::WorldLayerID(const IDObject<WorldLayerVertexDataLayerVersionListPointer>& ID) noexcept : ID(ID)
	{
		type = VertexLayerOrderIDType::WORLD_LAYER;
	}

}