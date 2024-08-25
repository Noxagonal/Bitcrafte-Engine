#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/data_types/FundamentalTypes.hpp>

#include <algorithm>



namespace bc {
namespace utility {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Simple utility class for containing version info used throughout the engine.
class Version
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr Version() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline constexpr Version(
		u32	major,
		u32	minor,
		u32	patch
	) noexcept
	:
		major( major ),
		minor( minor ),
		patch( patch )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Converts the current version info to vulkan packed format.
	///
	/// @warning
	/// Some values may be cut off, make sure that the version numbers fit.
	///
	/// @return
	/// Packed version info where the 3 highest bits are the variant number, next 7 highest bits are major, next 10 bits are minor,
	/// and the last 12 bits are the patch number.
	inline constexpr auto ToVulkanPacked() const noexcept -> u32
	{
		return ( ( ( (u32)( major ) & 0x7FU ) << 22U ) ) | ( ( (u32)( minor ) & 0x3FFU ) << 12U ) | ( (u32)( patch ) & 0xFFFU );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Converts the current version info to packed format. Compatible with Vulkan version info.
	///
	/// @param packed
	/// Packed version info where the 3 highest bits are the variant number, next 7 highest bits are major, next 10 bits are minor,
	/// and the last 12 bits are the patch number.
	inline constexpr void FromVulkanPacked( u32 packed ) noexcept
	{
		major = ( ( (u32)( packed ) >> 22U ) & 0x7FU );
		minor = ( ( (u32)( packed ) >> 12U ) & 0x3FFU );
		patch = ( (u32)( packed ) & 0xFFFU );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	u32	major = {};
	u32	minor = {};
	u32	patch = {};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( Version ) == sizeof( u32 ) * 3 );



} // utility
} // bc
