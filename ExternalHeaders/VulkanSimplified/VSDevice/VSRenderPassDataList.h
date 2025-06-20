#pragma once

#include <CustomLists\IDObjectDef.h>

#include "VSRenderPassDataListDef.h"

#include <vector>

namespace VulkanSimplified
{
	class RenderPassListInternal;
}

namespace VulkanSimplified
{
	struct SubpassCreationDataWithoutResolving;
	struct SubpassCreationDataWithResolving;

	class RenderPassList
	{
	public:
		RenderPassList(RenderPassListInternal& ref);
		~RenderPassList();

		IDObject<AutoCleanupRenderPass> AddRenderPassWithoutResolveAttachments(
			const std::vector<IDObject<RenderPassAttachmentData>>& attachments, const std::vector<SubpassCreationDataWithoutResolving>& subpasses,
			const std::vector<IDObject<SubpassDependencyData>>& subpassDependencies, size_t addOnReserving = 0);

		IDObject<AutoCleanupRenderPass> AddRenderPassWithResolveAttachments(
			const std::vector<IDObject<RenderPassAttachmentData>>& attachments, const std::vector<SubpassCreationDataWithResolving>& subpasses,
			const std::vector<IDObject<SubpassDependencyData>>& subpassDependencies, size_t addOnReserving = 0);

	private:
		RenderPassListInternal& _internal;
	};
}