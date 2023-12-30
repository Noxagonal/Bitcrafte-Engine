#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

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
		uint32_t						major,
		uint32_t						minor,
		uint32_t						patch
	) noexcept :
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
	inline constexpr uint32_t			ToVulkanPacked() const noexcept
	{
		return ( ( ( (uint32_t)( major ) & 0x7FU ) << 22U ) ) | ( ( (uint32_t)( minor ) & 0x3FFU ) << 12U ) | ( (uint32_t)( patch ) & 0xFFFU );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Converts the current version info to packed format. Compatible with Vulkan version info.
	///
	/// @param packed
	/// Packed version info where the 3 highest bits are the variant number, next 7 highest bits are major, next 10 bits are minor,
	/// and the last 12 bits are the patch number.
	inline constexpr void				FromVulkanPacked(
		uint32_t						packed
	) noexcept
	{
		major = ( ( (uint32_t)( packed ) >> 22U ) & 0x7FU );
		minor = ( ( (uint32_t)( packed ) >> 12U ) & 0x3FFU );
		patch = ( (uint32_t)( packed ) & 0xFFFU );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	uint32_t					major				= {};
	uint32_t					minor				= {};
	uint32_t					patch				= {};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( Version ) == sizeof( uint32_t ) * 3 );



} // utility
} // bc
