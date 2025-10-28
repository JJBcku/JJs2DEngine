#pragma once

#include <vector>

#include <CustomLists/IDObjectDef.h>

#include "VertexDataMainDef.h"

namespace JJs2DEngine
{
	class VertexDataMainInternal;

	struct BackgroundObjectData;

	class VertexDataMain
	{
	public:
		VertexDataMain(VertexDataMainInternal& ref);
		~VertexDataMain();

		void SetCameraPosition(float X, float Y);
		void SetCameraRotation(float rotation);
		void SetCameraZoom(float zoom);

		void CreateBackgroundLayerVersionList(const std::vector<BackgroundObjectData>& versionDataList);
		IDObject<UiVertexDataLayerVersionListPointer> AddUiLayerVersionList(const std::vector<size_t>& versionsMaxVerticesList, size_t addOnReserving);
		IDObject<WorldLayerVertexDataLayerVersionListPointer> AddWorldLayerVersionList(const std::vector<size_t>& versionsMaxObjectAmountsList, size_t addOnReserving);

		BackgroundVertexDataLayerVersionList GetBackgroundVertexDataLayerVersionList();
		UiVertexDataLayerVersionList GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID);
		WorldLayerVertexDataLayerVersionList GetWorldLayerVertexDataLayerVersionList(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID);

		const BackgroundVertexDataLayerVersionList GetBackgroundVertexDataLayerVersionList() const;
		const UiVertexDataLayerVersionList GetUiVertexDataLayerVersionList(IDObject<UiVertexDataLayerVersionListPointer> ID) const;
		const WorldLayerVertexDataLayerVersionList GetWorldLayerVertexDataLayerVersionList(IDObject<WorldLayerVertexDataLayerVersionListPointer> ID) const;

	private:
		VertexDataMainInternal& _internal;
	};
}