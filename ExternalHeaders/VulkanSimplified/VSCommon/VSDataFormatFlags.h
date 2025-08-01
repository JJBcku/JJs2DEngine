#pragma once

#include "VSDataFormatFlagsDef.h"

#include <compare>

namespace VulkanSimplified
{
	enum DataFormatFirstFlagSetBits : DataFormatFlags
	{
		DATA_FORMAT_A1_RGB5_UNORM_PACK16 = 0x1ULL,
		DATA_FORMAT_A2_BGR10_SINT_PACK32 = 0x2ULL,
		DATA_FORMAT_A2_BGR10_SNORM_PACK32 = 0x4ULL,
		DATA_FORMAT_A2_BGR10_SSCALED_PACK32 = 0x8ULL,
		DATA_FORMAT_A2_BGR10_UINT_PACK32 = 0x10ULL,
		DATA_FORMAT_A2_BGR10_UNORM_PACK32 = 0x20ULL,
		DATA_FORMAT_A2_BGR10_USCALED_PACK32 = 0x40ULL,
		DATA_FORMAT_A2_RGB10_SINT_PACK32 = 0x80ULL,
		DATA_FORMAT_A2_RGB10_SNORM_PACK32 = 0x100ULL,
		DATA_FORMAT_A2_RGB10_SSCALED_PACK32 = 0x200ULL,
		DATA_FORMAT_A2_RGB10_UINT_PACK32 = 0x400ULL,
		DATA_FORMAT_A2_RGB10_UNORM_PACK32 = 0x800ULL,
		DATA_FORMAT_A2_RGB10_USCALED_PACK32 = 0x1000ULL,
		DATA_FORMAT_ABGR8_SINT_PACK32 = 0x2000ULL,
		DATA_FORMAT_ABGR8_SNORM_PACK32 = 0x4000ULL,
		DATA_FORMAT_ABGR8_SRGB_PACK32 = 0x8000ULL,
		DATA_FORMAT_ABGR8_SSCALED_PACK32 = 0x10000ULL,
		DATA_FORMAT_ABGR8_UINT_PACK32 = 0x20000ULL,
		DATA_FORMAT_ABGR8_UNORM_PACK32 = 0x40000ULL,
		DATA_FORMAT_ABGR8_USCALED_PACK32 = 0x80000ULL,
		DATA_FORMAT_ASTC_10X10_SRGB_PACK32 = 0x100000ULL,
		DATA_FORMAT_ASTC_10X10_UNORM_PACK32 = 0x200000ULL,
		DATA_FORMAT_ASTC_10X5_SRGB_PACK32 = 0x400000ULL,
		DATA_FORMAT_ASTC_10X5_UNORM_PACK32 = 0x800000ULL,
		DATA_FORMAT_ASTC_10X6_SRGB_PACK32 = 0x1000000ULL,
		DATA_FORMAT_ASTC_10X6_UNORM_PACK32 = 0x2000000ULL,
		DATA_FORMAT_ASTC_10X8_SRGB_PACK32 = 0x4000000ULL,
		DATA_FORMAT_ASTC_10X8_UNORM_PACK32 = 0x8000000ULL,
		DATA_FORMAT_ASTC_12X10_SRGB_PACK32 = 0x10000000ULL,
		DATA_FORMAT_ASTC_12X10_UNORM_PACK32 = 0x20000000ULL,
		DATA_FORMAT_ASTC_12X12_SRGB_PACK32 = 0x40000000ULL,
		DATA_FORMAT_ASTC_12X12_UNORM_PACK32 = 0x80000000ULL,
	};

	enum DataFormatSecondFlagSetBits : DataFormatFlags
	{
		DATA_FORMAT_ASTC_4X4_SRGB_PACK32 = 0x1ULL,
		DATA_FORMAT_ASTC_4X4_UNORM_PACK32 = 0x2ULL,
		DATA_FORMAT_ASTC_5X4_SRGB_PACK32 = 0x4ULL,
		DATA_FORMAT_ASTC_5X4_UNORM_PACK32 = 0x8ULL,
		DATA_FORMAT_ASTC_5X5_SRGB_PACK32 = 0x10ULL,
		DATA_FORMAT_ASTC_5X5_UNORM_PACK32 = 0x20ULL,
		DATA_FORMAT_ASTC_6X5_SRGB_PACK32 = 0x40ULL,
		DATA_FORMAT_ASTC_6X5_UNORM_PACK32 = 0x80ULL,
		DATA_FORMAT_ASTC_6X6_SRGB_PACK32 = 0x100ULL,
		DATA_FORMAT_ASTC_6X6_UNORM_PACK32 = 0x200ULL,
		DATA_FORMAT_ASTC_8X5_SRGB_PACK32 = 0x400ULL,
		DATA_FORMAT_ASTC_8X5_UNORM_PACK32 = 0x800ULL,
		DATA_FORMAT_ASTC_8X6_SRGB_PACK32 = 0x1000ULL,
		DATA_FORMAT_ASTC_8X6_UNORM_PACK32 = 0x2000ULL,
		DATA_FORMAT_ASTC_8X8_SRGB_PACK32 = 0x4000ULL,
		DATA_FORMAT_ASTC_8X8_UNORM_PACK32 = 0x8000ULL,
		DATA_FORMAT_B10_GR11_UFLOAT_PACK32 = 0x10000ULL,
		DATA_FORMAT_B10X6_G10X6_R10X6_G10X6_422_UNORM_4PACK16 = 0x20000ULL,
		DATA_FORMAT_B12X4_G12X4_R12X4_G12X4_422_UNORM_4PACK16 = 0x40000ULL,
		DATA_FORMAT_BGRG16_422_UNORM = 0x80000ULL,
		DATA_FORMAT_BGRA4_UNORM_PACK16 = 0x100000ULL,
		DATA_FORMAT_BGR5_A1_UNORM_PACK16 = 0x200000ULL,
		DATA_FORMAT_B5_G6_R5_UNORM_PACK16 = 0x400000ULL,
		DATA_FORMAT_BGR8_SINT = 0x800000ULL,
		DATA_FORMAT_BGR8_SNORM = 0x1000000ULL,
		DATA_FORMAT_BGR8_SRGB = 0x2000000ULL,
		DATA_FORMAT_BGR8_UINT = 0x4000000ULL,
		DATA_FORMAT_BGR8_UNORM = 0x8000000ULL,
		DATA_FORMAT_BGRA8_SINT = 0x10000000ULL,
		DATA_FORMAT_BGRA8_SNORM = 0x20000000ULL,
		DATA_FORMAT_BGRA8_SRGB = 0x40000000ULL,
		DATA_FORMAT_BGRA8_SSCALED = 0x80000000ULL,
	};

	enum DataFormatThirdFlagSetBits : DataFormatFlags
	{
		DATA_FORMAT_BGRA8_UINT = 0x1ULL,
		DATA_FORMAT_BGRA8_UNORM = 0x2ULL,
		DATA_FORMAT_BGRA8_USCALED = 0x4ULL,
		DATA_FORMAT_BGRG8_422_UNORM = 0x8ULL,
		DATA_FORMAT_BC1_RGB_SRGB_BLOCK = 0x10ULL,
		DATA_FORMAT_BC1_RGB_UNORM_BLOCK = 0x20ULL,
		DATA_FORMAT_BC1_RGBA_SRGB_BLOCK = 0x40ULL,
		DATA_FORMAT_BC1_RGBA_UNORM_BLOCK = 0x80ULL,
		DATA_FORMAT_BC2_SRGB_BLOCK = 0x100ULL,
		DATA_FORMAT_BC2_UNORM_BLOCK = 0x200ULL,
		DATA_FORMAT_BC3_SRGB_BLOCK = 0x400ULL,
		DATA_FORMAT_BC3_UNORM_BLOCK = 0x800ULL,
		DATA_FORMAT_BC4_SNORM_BLOCK = 0x1000ULL,
		DATA_FORMAT_BC4_UNORM_BLOCK = 0x2000ULL,
		DATA_FORMAT_BC5_SNORM_BLOCK = 0x4000ULL,
		DATA_FORMAT_BC5_UNORM_BLOCK = 0x8000ULL,
		DATA_FORMAT_BC6H_SFLOAT_BLOCK = 0x10000ULL,
		DATA_FORMAT_BC6H_UFLOAT_BLOCK = 0x20000ULL,
		DATA_FORMAT_BC7_SRGB_BLOCK = 0x40000ULL,
		DATA_FORMAT_BC7_UNORM_BLOCK = 0x80000ULL,
		DATA_FORMAT_D16_UNORM = 0x100000ULL,
		DATA_FORMAT_D16_UNORM_S8_UINT = 0x200000ULL,
		DATA_FORMAT_D24_UNORM_S8_UINT = 0x400000ULL,
		DATA_FORMAT_D32_SFLOAT = 0x800000ULL,
		DATA_FORMAT_D32_SFLOAT_S8_UINT = 0x1000000ULL,
		DATA_FORMAT_E5_BGR9_UFLOAT_PACK32 = 0x2000000ULL,
		DATA_FORMAT_EAC_R11_SNORM_BLOCK = 0x4000000ULL,
		DATA_FORMAT_EAC_R11_UNORM_BLOCK = 0x8000000ULL,
		DATA_FORMAT_EAC_RG11_SNORM_BLOCK = 0x10000000ULL,
		DATA_FORMAT_EAC_RG11_UNORM_BLOCK = 0x20000000ULL,
		DATA_FORMAT_ETC2_RGB8_SRGB_BLOCK = 0x40000000ULL,
		DATA_FORMAT_ETC2_RGB8_UNORM_BLOCK = 0x80000000ULL,
	};

	enum DataFormatFourthFlagSetBits : DataFormatFlags
	{
		DATA_FORMAT_ETC2_RGB8_A1_SRGB_BLOCK = 0x1ULL,
		DATA_FORMAT_ETC2_RGB8_A1_UNORM_BLOCK = 0x2ULL,
		DATA_FORMAT_ETC2_RGBA8_SRGB_BLOCK = 0x4ULL,
		DATA_FORMAT_ETC2_RGBA8_UNORM_BLOCK = 0x8ULL,
		DATA_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16 = 0x10ULL,
		DATA_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16 = 0x20ULL,
		DATA_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16 = 0x40ULL,
		DATA_FORMAT_G10X6_BR10X6_2PLANE_420_UNORM_3PACK16 = 0x80ULL,
		DATA_FORMAT_G10X6_BR10X6_2PLANE_422_UNORM_3PACK16 = 0x100ULL,
		DATA_FORMAT_G10X6_B10X6_G10X6_R10X6_422_UNORM_4PACK16 = 0x200ULL,
		DATA_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16 = 0x400ULL,
		DATA_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16 = 0x800ULL,
		DATA_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16 = 0x1000ULL,
		DATA_FORMAT_G12X4_BR12X4_2PLANE_420_UNORM_3PACK16 = 0x2000ULL,
		DATA_FORMAT_G12X4_BR12X4_2PLANE_422_UNORM_3PACK16 = 0x4000ULL,
		DATA_FORMAT_G12X4_B12X4_G12X4_R12X4_422_UNORM_4PACK16 = 0x8000ULL,
		DATA_FORMAT_G16_B16_R16_3PLANE_420_UNORM = 0x10000ULL,
		DATA_FORMAT_G16_B16_R16_3PLANE_422_UNORM = 0x20000ULL,
		DATA_FORMAT_G16_BR16_2PLANE_420_UNORM = 0x40000ULL,
		DATA_FORMAT_G16_BR16_2PLANE_422_UNORM = 0x80000ULL,
		DATA_FORMAT_GBGR16_422_UNORM = 0x100000ULL,
		DATA_FORMAT_G8_B8_R8_3PLANE_420_UNORM = 0x200000ULL,
		DATA_FORMAT_G8_B8_R8_3PLANE_422_UNORM = 0x400000ULL,
		DATA_FORMAT_G8_B8_R8_3PLANE_444_UNORM = 0x800000ULL,
		DATA_FORMAT_G8_BR8_2PLANE_420_UNORM = 0x1000000ULL,
		DATA_FORMAT_G8_BR8_2PLANE_422_UNORM = 0x2000000ULL,
		DATA_FORMAT_GBGR8_422_UNORM = 0x4000000ULL,
		DATA_FORMAT_R10X6_UNORM_PACK16 = 0x8000000ULL,
		DATA_FORMAT_R10X6_G10X6_UNORM_2PACK16 = 0x10000000ULL,
		DATA_FORMAT_R10X6_G10X6_B10X6_A10X6_UNORM_4PACK16 = 0x20000000ULL,
		DATA_FORMAT_R12X4_UNORM_PACK16 = 0x40000000ULL,
		DATA_FORMAT_R12X4_G12X4_UNORM_2PACK16 = 0x80000000ULL,
	};

	enum DataFormatFifthFlagSetBits : DataFormatFlags
	{
		DATA_FORMAT_R12X4_G12X4_B12X4_A12X4_UNORM_4PACK16 = 0x1ULL,
		DATA_FORMAT_R16_SFLOAT = 0x2ULL,
		DATA_FORMAT_R16_SINT = 0x4ULL,
		DATA_FORMAT_R16_SNORM = 0x8ULL,
		DATA_FORMAT_R16_SSCALED = 0x10ULL,
		DATA_FORMAT_R16_UINT = 0x20ULL,
		DATA_FORMAT_R16_UNORM = 0x40ULL,
		DATA_FORMAT_R16_USCALED = 0x80ULL,
		DATA_FORMAT_RG16_SFLOAT = 0x100ULL,
		DATA_FORMAT_RG16_SINT = 0x200ULL,
		DATA_FORMAT_RG16_SNORM = 0x400ULL,
		DATA_FORMAT_RG16_SSCALED = 0x800ULL,
		DATA_FORMAT_RG16_UINT = 0x1000ULL,
		DATA_FORMAT_RG16_UNORM = 0x2000ULL,
		DATA_FORMAT_RG16_USCALED = 0x4000ULL,
		DATA_FORMAT_RGB16_SFLOAT = 0x8000ULL,
		DATA_FORMAT_RGB16_SINT = 0x10000ULL,
		DATA_FORMAT_RGB16_SNORM = 0x20000ULL,
		DATA_FORMAT_RGB16_UINT = 0x40000ULL,
		DATA_FORMAT_RGB16_UNORM = 0x80000ULL,
		DATA_FORMAT_RGBA16_SFLOAT = 0x100000ULL,
		DATA_FORMAT_RGBA16_SINT = 0x200000ULL,
		DATA_FORMAT_RGBA16_SNORM = 0x400000ULL,
		DATA_FORMAT_RGBA16_SSCALED = 0x800000ULL,
		DATA_FORMAT_RGBA16_UINT = 0x1000000ULL,
		DATA_FORMAT_RGBA16_UNORM = 0x2000000ULL,
		DATA_FORMAT_RGBA16_USCALED = 0x4000000ULL,
		DATA_FORMAT_R32_SFLOAT = 0x8000000ULL,
		DATA_FORMAT_R32_SINT = 0x10000000ULL,
		DATA_FORMAT_R32_UINT = 0x20000000ULL,
		DATA_FORMAT_RG32_SFLOAT = 0x40000000ULL,
		DATA_FORMAT_RG32_SINT = 0x80000000ULL,
	};

	enum DataFormatSixthFlagSetBits : DataFormatFlags
	{
		DATA_FORMAT_RG32_UINT = 0x1ULL,
		DATA_FORMAT_RGB32_SFLOAT = 0x2ULL,
		DATA_FORMAT_RGB32_SINT = 0x4ULL,
		DATA_FORMAT_RGB32_UINT = 0x8ULL,
		DATA_FORMAT_RGBA32_SFLOAT = 0x10ULL,
		DATA_FORMAT_RGBA32_SINT = 0x20ULL,
		DATA_FORMAT_RGBA32_UINT = 0x40ULL,
		DATA_FORMAT_RG4_UNORM_PACK8 = 0x80ULL,
		DATA_FORMAT_RGBA4_UNORM_PACK16 = 0x100ULL,
		DATA_FORMAT_RGB5_A1_UNORM_PACK16 = 0x200ULL,
		DATA_FORMAT_R5_G6_B5_UNORM_PACK16 = 0x400ULL,
		DATA_FORMAT_R64_SFLOAT = 0x800ULL,
		DATA_FORMAT_R64_SINT = 0x1000ULL,
		DATA_FORMAT_R64_UINT = 0x2000ULL,
		DATA_FORMAT_RG64_SINT = 0x4000ULL,
		DATA_FORMAT_RG64_UINT = 0x8000ULL,
		DATA_FORMAT_RGBA64_SINT = 0x10000ULL,
		DATA_FORMAT_RGBA64_UINT = 0x20000ULL,
		DATA_FORMAT_R8_SINT = 0x40000ULL,
		DATA_FORMAT_R8_SNORM = 0x80000ULL,
		DATA_FORMAT_R8_SRGB = 0x100000ULL,
		DATA_FORMAT_R8_SSCALED = 0x200000ULL,
		DATA_FORMAT_R8_UINT = 0x400000ULL,
		DATA_FORMAT_R8_UNORM = 0x800000ULL,
		DATA_FORMAT_R8_USCALED = 0x1000000ULL,
		DATA_FORMAT_RG8_SINT = 0x2000000ULL,
		DATA_FORMAT_RG8_SNORM = 0x4000000ULL,
		DATA_FORMAT_RG8_SRGB = 0x8000000ULL,
		DATA_FORMAT_RG8_SSCALED = 0x10000000ULL,
		DATA_FORMAT_RG8_UINT = 0x20000000ULL,
		DATA_FORMAT_RG8_UNORM = 0x40000000ULL,
		DATA_FORMAT_RG8_USCALED = 0x80000000ULL,
	};

	enum DataFormatSeventhFlagSetBits : DataFormatFlags
	{
		DATA_FORMAT_RGB8_SINT = 0x1ULL,
		DATA_FORMAT_RGB8_SNORM = 0x2ULL,
		DATA_FORMAT_RGB8_SRGB = 0x4ULL,
		DATA_FORMAT_RGB8_UINT = 0x8ULL,
		DATA_FORMAT_RGB8_UNORM = 0x10ULL,
		DATA_FORMAT_RGBA8_SINT = 0x20ULL,
		DATA_FORMAT_RGBA8_SNORM = 0x40ULL,
		DATA_FORMAT_RGBA8_SRGB = 0x80ULL,
		DATA_FORMAT_RGBA8_SSCALED = 0x100ULL,
		DATA_FORMAT_RGBA8_UINT = 0x200ULL,
		DATA_FORMAT_RGBA8_UNORM = 0x400ULL,
		DATA_FORMAT_RGBA8_USCALED = 0x800ULL,
		DATA_FORMAT_S8_UINT = 0x1000ULL,
		DATA_FORMAT_X8D24_UNORM_PACK32 = 0x2000ULL,
	};

	struct DataFormatFullSetList
	{
		DataFormatFlags firstSet;
		DataFormatFlags secondSet;
		DataFormatFlags thirdSet;
		DataFormatFlags fourthSet;
		DataFormatFlags fifthSet;
		DataFormatFlags sixthSet;
		DataFormatFlags seventhSet;

		DataFormatFullSetList();
	};

	enum class DataFormatSetEnum : DataFormatFlags
	{
		DATA_SET_UNKNOWN = 0x0ULL,
		DATA_SET_ONE = 0x1ULL,
		DATA_SET_TWO = 0x2ULL,
		DATA_SET_THREE = 0x4ULL,
		DATA_SET_FOUR = 0x8ULL,
		DATA_SET_FIVE = 0x10ULL,
		DATA_SET_SIX = 0x20ULL,
		DATA_SET_SEVEN = 0x40ULL,
	};

	struct DataFormatSetIndependentID
	{
		DataFormatSetEnum dataSet;
		DataFormatFlags dataFormat;

		DataFormatSetIndependentID();

		DataFormatSetIndependentID(DataFormatFirstFlagSetBits format);
		DataFormatSetIndependentID(DataFormatSecondFlagSetBits format);
		DataFormatSetIndependentID(DataFormatThirdFlagSetBits format);
		DataFormatSetIndependentID(DataFormatFourthFlagSetBits format);
		DataFormatSetIndependentID(DataFormatFifthFlagSetBits format);
		DataFormatSetIndependentID(DataFormatSixthFlagSetBits format);
		DataFormatSetIndependentID(DataFormatSeventhFlagSetBits format);

		std::strong_ordering operator<=>(const DataFormatSetIndependentID& rhs) const noexcept = default;
		bool operator==(const DataFormatSetIndependentID& rhs) const noexcept = default;
	};

	bool CheckFormatSupport(const DataFormatFullSetList& checkedSet, const DataFormatSetIndependentID& formatID);
}