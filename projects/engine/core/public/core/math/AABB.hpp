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
template<size_t AABBDimensionCount, utility::FundamentalType ValueType>
class alignas( sizeof( ValueType ) * 2 ) AABBBase
{
	static_assert( AABBDimensionCount >= 2 && AABBDimensionCount <= 4, "Invalid number of dimensions" );

public:

	static constexpr size_t DimensionCount = AABBDimensionCount;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// First corner of the AABB, should contain lower values.
	VectorBase<DimensionCount, ValueType>									begin			= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Second corner of the AABB, should contain higher values.
	VectorBase<DimensionCount, ValueType>									end				= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr AABBBase() noexcept = default;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Initializes the AABB to a single value, useful when setting it to 0.
	///
	/// @param initial_value
	/// Initial value everything is initialized to.
	constexpr AABBBase(
		ValueType															initial_value
	) noexcept :
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
		VectorBase<DimensionCount, ValueType>								begin,
		VectorBase<DimensionCount, ValueType>								end
	) noexcept :
		begin( begin ),
		end( end )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr AABBBase													&	operator=(
		const AABBBase													&	other
	) noexcept
	{
		CopyOther( other );
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
	constexpr bool															operator==(
		const AABBBase													&	other
	) const
	{
		if constexpr( utility::FloatingPointType<ValueType> )
		{
			return math::IsFloatEqual( begin, other.begin ) && utility::IsFloatEqual( end, other.end );
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
	constexpr bool															operator!=(
		const AABBBase													&	other
	) const
	{
		if constexpr( utility::FloatingPointType<ValueType> )
		{
			return !math::IsFloatEqual( begin, other.begin ) || !utility::IsFloatEqual( end, other.end );
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
	constexpr const VectorBase<DimensionCount, ValueType>				&	operator[](
		size_t																index
	) const
	{
		if( std::is_constant_evaluated() )
		{
			// TODO: Test this works.
			[]<bool flag = false>()
				{
					static_assert( index < 2 && flag, "Index out of range" );
				}();
		}
		else
		{
			if( index > 2 ) diagnostic::Throw( U"Index out of range" );
		}

		if constexpr( index == 0 ) return begin;
		if constexpr( index == 1 ) return end;
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
	constexpr VectorBase<DimensionCount, ValueType>						&	operator[](
		size_t																index
	)
	{
		if( std::is_constant_evaluated() )
		{
			// TODO: Test this works.
			[] <bool flag = false>( )
			{
				static_assert( index < 2 && flag, "Index out of range" );
			}( );
		}
		else
		{
			if( index > 2 ) diagnostic::Throw( U"Index out of range" );
		}

		if constexpr( index == 0 ) return begin;
		if constexpr( index == 1 ) return end;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the volume of the AABB, for 2D, this is the Area, for 3D, this is Volume, for 4D, this gets the Hypervolume.
	///
	/// @return
	/// Area, Volume, or Hypervolume of the AABB.
	constexpr ValueType														GetVolume() const noexcept
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
	constexpr void															CopyOther(
		const AABBBase													&	other
	) noexcept
	{
		begin = other.begin;
		end = other.end;
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using AABB2f = AABBBase<2, float>;
using AABB2d = AABBBase<2, double>;
using AABB2i = AABBBase<2, int64_t>;
using AABB2u = AABBBase<2, uint64_t>;

using AABB3f = AABBBase<3, float>;
using AABB3d = AABBBase<3, double>;
using AABB3i = AABBBase<3, int64_t>;
using AABB3u = AABBBase<3, uint64_t>;

using AABB4f = AABBBase<4, float>;
using AABB4d = AABBBase<4, double>;
using AABB4i = AABBBase<4, int64_t>;
using AABB4u = AABBBase<4, uint64_t>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( AABB2f ) == sizeof( float ) * 4 );
static_assert( sizeof( AABB2d ) == sizeof( double ) * 4 );
static_assert( sizeof( AABB2i ) == sizeof( int64_t ) * 4 );
static_assert( sizeof( AABB2u ) == sizeof( uint64_t ) * 4 );

static_assert( sizeof( AABB3f ) == sizeof( float ) * 6 );
static_assert( sizeof( AABB3d ) == sizeof( double ) * 6 );
static_assert( sizeof( AABB3i ) == sizeof( int64_t ) * 6 );
static_assert( sizeof( AABB3u ) == sizeof( uint64_t ) * 6 );

static_assert( sizeof( AABB4f ) == sizeof( float ) * 8 );
static_assert( sizeof( AABB4d ) == sizeof( double ) * 8 );
static_assert( sizeof( AABB4i ) == sizeof( int64_t ) * 8 );
static_assert( sizeof( AABB4u ) == sizeof( uint64_t ) * 8 );

static_assert( alignof( AABB2f ) == alignof( float ) * 2 );
static_assert( alignof( AABB2d ) == alignof( double ) * 2 );
static_assert( alignof( AABB2i ) == alignof( int64_t ) * 2 );
static_assert( alignof( AABB2u ) == alignof( uint64_t ) * 2 );

static_assert( alignof( AABB3f ) == alignof( float ) * 2 );
static_assert( alignof( AABB3d ) == alignof( double ) * 2 );
static_assert( alignof( AABB3i ) == alignof( int64_t ) * 2 );
static_assert( alignof( AABB3u ) == alignof( uint64_t ) * 2 );

static_assert( alignof( AABB4f ) == alignof( float ) * 4 );
static_assert( alignof( AABB4d ) == alignof( double ) * 4 );
static_assert( alignof( AABB4i ) == alignof( int64_t ) * 4 );
static_assert( alignof( AABB4u ) == alignof( uint64_t ) * 4 );



} // math
} // bc
