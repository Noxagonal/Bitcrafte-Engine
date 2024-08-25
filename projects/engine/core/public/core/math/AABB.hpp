#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/math/Vector.hpp>
#include <core/math/VectorComparison.hpp>
#include <core/diagnostic/exception/Exception.hpp>

#include <cstdint>
#include <type_traits>
#include <cmath>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Axis Aligned Bounding Box.
///
/// Used to define initial space something occupies, AABBs are always aligned to axis and cannot be rotated. It is often used as
/// initial test in occlusion queries and physics to see if something should be included in calculations or be drawn.
///
/// @tparam AABBDimensionCount
/// Dimension count for this bounding box, this must be 2, 3 or 4.
///
/// @tparam ValueType
/// Value type of this bounding box.
template<
	u64							AABBDimensionCount,
	utility::FundamentalValue	ValueType
>
class AABBBase
{
	static_assert( AABBDimensionCount >= 2 && AABBDimensionCount <= 4, "Invalid number of dimensions" );

public:

	using VectorType = VectorBase<AABBDimensionCount, ValueType>;

	static constexpr u64 DimensionCount = AABBDimensionCount;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// First corner of the AABB, contains low values.
	VectorType		begin			= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Second corner of the AABB, contains high values.
	VectorType		end				= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr AABBBase() noexcept = default;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Initializes the AABB to a single value, useful when setting it to 0.
	///
	/// @param initial_value
	/// Initial value everything is initialized to.
	constexpr AABBBase( ValueType initial_value ) noexcept
	:
		begin( initial_value ),
		end( initial_value )
	{}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Initialize corners of the bounding box.
	///
	/// @param begin
	/// The corner containing lower values.
	///
	/// @param end
	/// The corner containing higher values.
	constexpr AABBBase(
		VectorType	begin,
		VectorType	end
	) noexcept
	:
		begin( begin ),
		end( end )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const AABBBase& other ) noexcept -> AABBBase&
	{
		Copy( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Test if this AABB has the same coordinates as another AABB.
	///
	/// @note
	/// If AABB is floating point type, then floating point rounding errors are automatically taken into consideration.
	///
	/// @param other
	/// Other AABB to compare against.
	///
	/// @return
	/// True if AABBs match or in case of floating point value types, matches closely. False otherwise.
	constexpr auto operator==( const AABBBase& other ) const -> bool
	{
		if constexpr( utility::FloatingPointValue<ValueType> )
		{
			return IsFloatEqual( begin, other.begin ) && IsFloatEqual( end, other.end );
		}
		else
		{
			return begin == other.begin && end == other.end;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Test if this AABB has different coordinates from another AABB.
	///
	/// @note
	/// If AABB is floating point type, then floating point rounding errors are automatically taken into consideration.
	///
	/// @param other
	/// Other AABB to compare against.
	///
	/// @return
	/// True if AABBs do not match or in case of floating point value types, differ more than rounding error threshold. False if
	/// AABBs match, or match closely enough to be within rounding errors.
	constexpr auto operator!=( const AABBBase& other ) const -> bool
	{
		if constexpr( utility::FloatingPointValue<ValueType> )
		{
			return !IsFloatEqual( begin, other.begin ) || !IsFloatEqual( end, other.end );
		}
		else
		{
			return begin != other.begin || end != other.end;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get AABB corners via [] operator.
	///
	/// @param index
	/// Index to a corner, 0 is the starting corner, 1 is the ending corner, other values will throw an error.
	///
	/// @return
	/// Reference to the vector which contains the coordinates to AABB corner.
	constexpr auto operator[]( u64 index ) const -> const VectorType&
	{
		if( std::is_constant_evaluated() )
		{
			// TODO: Test this works.
			[index]<bool flag = false>()
				{
					static_assert( index < 2 && flag, "Index out of range" );
				}();
		}
		else
		{
			if( index > 2 ) diagnostic::Throw( U"Index out of range" );
		}

		if( index == 0 ) return begin;
		if( index == 1 ) return end;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get AABB corners via [] operator.
	///
	/// @param index
	/// Index to a corner, 0 is the starting corner, 1 is the ending corner, other values will throw an error.
	///
	/// @return
	/// Reference to the vector which contains the coordinates to AABB corner.
	constexpr auto operator[]( u64 index ) -> VectorType&
	{
		if( std::is_constant_evaluated() )
		{
			// TODO: Test this works.
			[index] <bool flag = false>( )
			{
				static_assert( index < 2 && flag, "Index out of range" );
			}( );
		}
		else
		{
			if( index > 2 ) diagnostic::Throw( U"Index out of range" );
		}

		if( index == 0 ) return begin;
		if( index == 1 ) return end;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the volume of the AABB, for 2D, this is the Area, for 3D, this is Volume, for 4D, this gets the Hypervolume.
	///
	/// @return
	/// Area, Volume, or Hypervolume of the AABB.
	constexpr auto GetVolume() const noexcept -> ValueType
	{
		if constexpr( DimensionCount == 2 )
		{
			auto size_x = end.x - begin.x;
			auto size_y = end.y - begin.y;
			return std::abs( size_x * size_y );
		}
		if constexpr( DimensionCount == 3 )
		{
			auto size_x = end.x - begin.x;
			auto size_y = end.y - begin.y;
			auto size_z = end.z - begin.z;
			return std::abs( size_x * size_y * size_z );
		}
		if constexpr( DimensionCount == 4 )
		{
			auto size_x = end.x - begin.x;
			auto size_y = end.y - begin.y;
			auto size_z = end.z - begin.z;
			auto size_w = end.w - begin.w;
			return std::abs( size_x * size_y * size_z * size_w );
		}
		return {};
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Copy( const AABBBase& other ) noexcept
	{
		begin = other.begin;
		end = other.end;
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using AABB2f32 = AABBBase<2, f32>;
using AABB2f64 = AABBBase<2, f64>;
using AABB2i32 = AABBBase<2, i32>;
using AABB2u32 = AABBBase<2, u32>;
using AABB2i64 = AABBBase<2, i64>;
using AABB2u64 = AABBBase<2, u64>;

using AABB3f32 = AABBBase<3, f32>;
using AABB3f64 = AABBBase<3, f64>;
using AABB3i32 = AABBBase<3, i32>;
using AABB3u32 = AABBBase<3, u32>;
using AABB3i64 = AABBBase<3, i64>;
using AABB3u64 = AABBBase<3, u64>;

using AABB4f32 = AABBBase<4, f32>;
using AABB4f64 = AABBBase<4, f64>;
using AABB4i32 = AABBBase<4, i32>;
using AABB4u32 = AABBBase<4, u32>;
using AABB4i64 = AABBBase<4, i64>;
using AABB4u64 = AABBBase<4, u64>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( AABB2f32 ) == sizeof( f32 ) * 4 );
static_assert( sizeof( AABB2f64 ) == sizeof( f64 ) * 4 );
static_assert( sizeof( AABB2i32 ) == sizeof( i32 ) * 4 );
static_assert( sizeof( AABB2u32 ) == sizeof( u32 ) * 4 );
static_assert( sizeof( AABB2i64 ) == sizeof( i64 ) * 4 );
static_assert( sizeof( AABB2u64 ) == sizeof( u64 ) * 4 );

static_assert( sizeof( AABB3f32 ) == sizeof( f32 ) * 6 );
static_assert( sizeof( AABB3f64 ) == sizeof( f64 ) * 6 );
static_assert( sizeof( AABB3i32 ) == sizeof( i32 ) * 6 );
static_assert( sizeof( AABB3u32 ) == sizeof( u32 ) * 6 );
static_assert( sizeof( AABB3i64 ) == sizeof( i64 ) * 6 );
static_assert( sizeof( AABB3u64 ) == sizeof( u64 ) * 6 );

static_assert( sizeof( AABB4f32 ) == sizeof( f32 ) * 8 );
static_assert( sizeof( AABB4f64 ) == sizeof( f64 ) * 8 );
static_assert( sizeof( AABB4i32 ) == sizeof( i32 ) * 8 );
static_assert( sizeof( AABB4u32 ) == sizeof( u32 ) * 8 );
static_assert( sizeof( AABB4i64 ) == sizeof( i64 ) * 8 );
static_assert( sizeof( AABB4u64 ) == sizeof( u64 ) * 8 );

static_assert( alignof( AABB2f32 ) == alignof( f32 ) * 2 );
static_assert( alignof( AABB2f64 ) == alignof( f64 ) * 2 );
static_assert( alignof( AABB2i32 ) == alignof( i32 ) * 2 );
static_assert( alignof( AABB2u32 ) == alignof( u32 ) * 2 );
static_assert( alignof( AABB2i64 ) == alignof( i64 ) * 2 );
static_assert( alignof( AABB2u64 ) == alignof( u64 ) * 2 );

static_assert( alignof( AABB3f32 ) == alignof( f32 ) * 1 );
static_assert( alignof( AABB3f64 ) == alignof( f64 ) * 1 );
static_assert( alignof( AABB3i32 ) == alignof( i32 ) * 1 );
static_assert( alignof( AABB3u32 ) == alignof( u32 ) * 1 );
static_assert( alignof( AABB3i64 ) == alignof( i64 ) * 1 );
static_assert( alignof( AABB3u64 ) == alignof( u64 ) * 1 );

static_assert( alignof( AABB4f32 ) == alignof( f32 ) * 4 );
static_assert( alignof( AABB4f64 ) == alignof( f64 ) * 4 );
static_assert( alignof( AABB4i32 ) == alignof( i32 ) * 4 );
static_assert( alignof( AABB4u32 ) == alignof( u32 ) * 4 );
static_assert( alignof( AABB4i64 ) == alignof( i64 ) * 4 );
static_assert( alignof( AABB4u64 ) == alignof( u64 ) * 4 );



} // math
} // bc
