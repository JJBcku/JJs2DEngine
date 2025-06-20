#pragma once

#include "../VSCommon/VSDataFormatFlags.h"

namespace VulkanSimplified
{
	struct FormatsSupportedBufferFeaturesList
	{
		DataFormatFullSetList uniformTexelBuffer;
		DataFormatFullSetList storageTexelBuffer;
		DataFormatFullSetList storageTexelBufferAtomic;
		DataFormatFullSetList vertexBuffer;

		FormatsSupportedBufferFeaturesList();
	};

	struct FormatsSupportedImageFeaturesList
	{
		DataFormatFullSetList sampledImage;
		DataFormatFullSetList storageImage;
		DataFormatFullSetList storageImageAtomic;
		DataFormatFullSetList colorAttachment;
		DataFormatFullSetList colorAttachmentBlend;
		DataFormatFullSetList depthStencilAttachment;
		DataFormatFullSetList blitSrc;
		DataFormatFullSetList blitDst;
		DataFormatFullSetList sampledImageFilterLinear;

		FormatsSupportedImageFeaturesList();
	};

	struct FormatsSupportedFeatures
	{
		FormatsSupportedBufferFeaturesList formatFeaturesBufferSupport;
		FormatsSupportedImageFeaturesList formatFeaturesLinearImageSupport;
		FormatsSupportedImageFeaturesList formatFeaturesOptimalImageSupport;

		FormatsSupportedFeatures();
	};
}