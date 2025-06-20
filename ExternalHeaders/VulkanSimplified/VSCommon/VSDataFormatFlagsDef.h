#pragma once

#include <stdint.h>

namespace VulkanSimplified
{
	typedef uint64_t DataFormatFlags;

	enum DataFormatFirstFlagSetBits : DataFormatFlags;
	enum DataFormatSecondFlagSetBits : DataFormatFlags;
	enum DataFormatThirdFlagSetBits : DataFormatFlags;
	enum DataFormatFourthFlagSetBits : DataFormatFlags;
	enum DataFormatFifthFlagSetBits : DataFormatFlags;
	enum DataFormatSixthFlagSetBits : DataFormatFlags;
	enum DataFormatSeventhFlagSetBits : DataFormatFlags;

	struct DataFormatFullSetList;

	enum class DataFormatSetEnum : DataFormatFlags;
	struct DataFormatSetIndependentID;

	bool CheckFormatSupport(const DataFormatFullSetList& checkedSet, const DataFormatSetIndependentID& formatID);
}
