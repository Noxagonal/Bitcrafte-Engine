#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <cstdint>



namespace bc {
namespace rhi {



enum class ColorFormat : uint32_t
{
	NONE			= 0,

	// Basic formats
	R8_UNorm,
	R8_G8_UNorm,
	R8_G8_B8_UNorm,
	R8_G8_B8_A8_UNorm,

	B8_G8_R8_A8_UNorm,

	R8_G8_B8_A8_SRGB,
	B8_G8_R8_A8_SRGB,

	R16_UNorm,
	R16_G16_UNorm,
	R16_G16_B16_UNorm,
	R16_G16_B16_A16_UNorm,

	R16_SFloat,
	R16_G16_SFloat,
	R16_G16_B16_SFloat,
	R16_G16_B16_A16_SFloat,

	R32_SFloat,
	R32_G32_SFloat,
	R32_G32_B32_SFloat,
	R32_G32_B32_A32_SFloat,

	// Packed formats
	R4_G4_UNorm,			// Pack 8
	R4_G4_B4_A4_UNorm,		// Pack 16
	R5_G6_B5_UNorm,			// Pack 16
	R5_G5_B5_A1_UNorm,		// Pack 16
	A2_R10_G10_B10_UNorm,	// Pack 32
	A2_B10_G10_R10_UNorm,	// Pack 32
	B10_G11_R11_UFloat,		// Pack 32
	E5_B9_G9_R9_UFloat,		// Pack 32

	// DXT/BC formats
	BC1_RGB_UNorm,			// DXT1, no alpha
	BC1_RGBA_UNorm,			// DXT1, 1-bit alpha
	BC3_RGBA_UNorm,			// DXT5, full alpha
	BC4_R_UNorm,			// Grayscale
	BC5_RG_UNorm,			// Red and Green channels (often used for normal maps)
	BC5_RG_SNorm,			// Red and Green channels (often used for normal maps)
	BC6H_RGB_SFloat,		// Floating point
	BC7_RGBA_UNorm,			// High-quality

	// ASTC formats, lower block sizes are higher quality
	ASTC_4x4_RGBA_UNorm,	// 4x4 block size, RGBA
	ASTC_5x5_RGBA_UNorm,	// 5x5 block size, RGBA
	ASTC_6x6_RGBA_UNorm,	// 6x6 block size, RGBA
	ASTC_8x8_RGBA_UNorm,	// 8x8 block size, RGBA
	ASTC_10x10_RGBA_UNorm,	// 8x8 block size, RGBA
	ASTC_12x12_RGBA_UNorm,	// 8x8 block size, RGBA

	D32_SFloat,				// Depth
	D32_SFloat_S8			// Depth and stencil
};



} // rhi
} // bc
