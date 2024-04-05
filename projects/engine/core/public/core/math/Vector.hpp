#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/math/SmallValues.hpp>
#include <core/math/FundamentalComparison.hpp>
#include <core/diagnostic/exception/Exception.hpp>

#include <cstdint>
#include <type_traits>
#include <cmath>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Mathematical vector storing 2, 3, or 4 components
/// 
/// @tparam DimensionCount
/// Number of dimensions this vector should have, must be 2, 3, or 4.
/// 
/// @tparam ValueType
/// Type of the contained value, eg. f32, i32, f64.
template<u64 DimensionCount, utility::FundamentalValue ValueType>
class VectorBase;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalValue ValueType>
class alignas( sizeof( ValueType ) * 2 ) VectorBase<2, ValueType>
{
public:

	static constexpr u64 DimensionCount = 2;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																x				= {};
	ValueType																y				= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase() noexcept = default;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase(
		ValueType															initial_value
	) noexcept :
		x( initial_value ),
		y( initial_value )
	{}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase(
		ValueType															x,
		ValueType															y
	) noexcept :
		x( x ),
		y( y )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 OtherDimensionCount>
	constexpr VectorBase(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 OtherDimensionCount>
	constexpr VectorBase												&	operator=(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator+=(
		const VectorBase												&	other
	) noexcept
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator-=(
		const VectorBase												&	other
	) noexcept
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator*=(
		const VectorBase												&	other
	) noexcept
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator*=(
		ValueType															scalar
	) noexcept
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator/=(
		const VectorBase												&	other
	)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator/=(
		ValueType															scalar
	)
	{
		x /= scalar;
		y /= scalar;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator+(
		const VectorBase												&	other
	) const noexcept
	{
		VectorBase result = *this;
		result += other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator-(
		const VectorBase												&	other
	) const noexcept
	{
		VectorBase result = *this;
		result -= other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator*(
		const VectorBase												&	other
	) const noexcept
	{
		VectorBase result = *this;
		result *= other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator*(
		ValueType															scalar
	) const noexcept
	{
		VectorBase result = *this;
		result *= scalar;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator/(
		const VectorBase												&	other
	) const
	{
		VectorBase result = *this;
		result /= other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator/(
		ValueType															scalar
	) const
	{
		VectorBase result = *this;
		result /= scalar;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator==(
		VectorBase															other
	) const
	{
		if constexpr( utility::FloatingPointValue<ValueType> )
		{
			return IsFloatEqual( x, other.x ) && IsFloatEqual( y, other.y );
		}
		else
		{
			return x == other.x && y == other.y;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator!=(
		VectorBase															other
	) const
	{
		if constexpr( utility::FloatingPointValue<ValueType> )
		{
			return !IsFloatEqual( x, other.x ) || !IsFloatEqual( y, other.y );
		}
		else
		{
			return x != other.x || y != other.y;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType											&	operator[](
		u64																	index
	) const
	{
		if( index == 0 ) return x;
		if( index == 1 ) return y;
		diagnostic::Throw( U"Index out of range" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType													&	operator[](
		u64																	index
	)
	{
		if( index == 0 ) return x;
		if( index == 1 ) return y;
		diagnostic::Throw( U"Index out of range" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType														Magnitude() const noexcept
	{
		return std::sqrt(
			x * x +
			y * y
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType														Length() const noexcept
	{
		return Magnitude();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													Normalized() const noexcept
	{
		auto magnitude = Magnitude();
		if( magnitude <= SmallValue<ValueType> ) return VectorBase::UnitVector();
		return *this / magnitude;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval VectorBase												UnitVector() noexcept
	{
		return VectorBase { 1.0, 0.0 };
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 OtherDimensionCount>
	constexpr void															CopyOther(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		if constexpr( OtherDimensionCount == 2 )
		{
			x = other.x;
			y = other.y;
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalValue ValueType>
class VectorBase<3, ValueType>
{
public:

	static constexpr u64 DimensionCount = 3;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																x				= {};
	ValueType																y				= {};
	ValueType																z				= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase(
		ValueType															initial_value
	) noexcept :
		x( initial_value ),
		y( initial_value ),
		z( initial_value )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase(
		ValueType															x,
		ValueType															y,
		ValueType															z
	) noexcept :
		x( x ),
		y( y ),
		z( z )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 OtherDimensionCount>
	constexpr VectorBase(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 OtherDimensionCount>
	constexpr VectorBase												&	operator=(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator+=(
		const VectorBase												&	other
	) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator-=(
		const VectorBase												&	other
	) noexcept
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator*=(
		const VectorBase												&	other
	) noexcept
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator*=(
		ValueType															scalar
	) noexcept
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator/=(
		const VectorBase												&	other
	)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator/=(
		ValueType															scalar
	)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator+(
		const VectorBase												&	other
	) const noexcept
	{
		VectorBase result = *this;
		result += other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator-(
		const VectorBase												&	other
	) const noexcept
	{
		VectorBase result = *this;
		result -= other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator*(
		const VectorBase												&	other
	) const noexcept
	{
		VectorBase result = *this;
		result *= other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator*(
		ValueType															scalar
	) const noexcept
	{
		VectorBase result = *this;
		result *= scalar;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator/(
		const VectorBase												&	other
	) const
	{
		VectorBase result = *this;
		result /= other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator/(
		ValueType															scalar
	) const
	{
		VectorBase result = *this;
		result /= scalar;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator==(
		VectorBase															other
	) const
	{
		if constexpr( utility::FloatingPointValue<ValueType> )
		{
			return IsFloatEqual( x, other.x ) && IsFloatEqual( y, other.y ) && IsFloatEqual( z, other.z );
		}
		else
		{
			return x == other.x && y == other.y && z == other.z;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator!=(
		VectorBase															other
	) const
	{
		if constexpr( utility::FloatingPointValue<ValueType> )
		{
			return !IsFloatEqual( x, other.x ) || !IsFloatEqual( y, other.y ) || !IsFloatEqual( z, other.z );
		}
		else
		{
			return x != other.x || y != other.y || z != other.z;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType											&	operator[](
		u64																	index
	) const
	{
		if( index == 0 ) return x;
		if( index == 1 ) return y;
		if( index == 2 ) return z;
		diagnostic::Throw( U"Index out of range" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType													&	operator[](
		u64																	index
	)
	{
		if( index == 0 ) return x;
		if( index == 1 ) return y;
		if( index == 2 ) return z;
		diagnostic::Throw( U"Index out of range" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType														Magnitude() const noexcept
	{
		return std::sqrt(
			x * x +
			y * y +
			z * z
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType														Length() const noexcept
	{
		return Magnitude();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													Normalized() const noexcept
	{
		auto magnitude = Magnitude();
		if( magnitude <= SmallValue<ValueType> ) return VectorBase::UnitVector();
		return *this / magnitude;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval VectorBase												UnitVector() noexcept
	{
		return VectorBase { 1.0, 0.0, 0.0 };
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 OtherDimensionCount>
	constexpr void															CopyOther(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) requires( OtherDimensionCount <= DimensionCount )
	{
		if constexpr( OtherDimensionCount == 2 )
		{
			x = other.x;
			y = other.y;
			z = {};
		}
		if constexpr( OtherDimensionCount == 3 )
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalValue ValueType>
class alignas( sizeof( ValueType ) * 4 ) VectorBase<4, ValueType>
{
public:

	static constexpr u64 DimensionCount = 4;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																x					= {};
	ValueType																y					= {};
	ValueType																z					= {};
	ValueType																w					= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase(
		ValueType															initial_value
	) noexcept :
		x( initial_value ),
		y( initial_value ),
		z( initial_value ),
		w( initial_value )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase(
		ValueType															x,
		ValueType															y,
		ValueType															z,
		ValueType															w
	) noexcept :
		x( x ),
		y( y ),
		z( z ),
		w( w )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 OtherDimensionCount>
	constexpr VectorBase(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 OtherDimensionCount>
	constexpr VectorBase												&	operator=(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator+=(
		const VectorBase												&	other
	) noexcept
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator-=(
		const VectorBase												&	other
	) noexcept
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator*=(
		const VectorBase												&	other
	) noexcept
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator*=(
		ValueType															scalar
	) noexcept
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator/=(
		const VectorBase												&	other
	)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase												&	operator/=(
		ValueType															scalar
	)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator+(
		const VectorBase												&	other
	) const noexcept
	{
		VectorBase result = *this;
		result += other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator-(
		const VectorBase												&	other
	) const noexcept
	{
		VectorBase result = *this;
		result -= other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator*(
		const VectorBase												&	other
	) const noexcept
	{
		VectorBase result = *this;
		result *= other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator*(
		ValueType															scalar
	) const noexcept
	{
		VectorBase result = *this;
		result *= scalar;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator/(
		const VectorBase												&	other
	) const
	{
		VectorBase result = *this;
		result /= other;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													operator/(
		ValueType															scalar
	) const
	{
		VectorBase result = *this;
		result /= scalar;
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator==(
		VectorBase															other
		) const
	{
		if constexpr( utility::FloatingPointValue<ValueType> )
		{
			return IsFloatEqual( x, other.x ) && IsFloatEqual( y, other.y ) && IsFloatEqual( z, other.z ) && IsFloatEqual( w, other.w );
		}
		else
		{
			return x == other.x && y == other.y && z == other.z && w == other.w;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator!=(
		VectorBase															other
		) const
	{
		if constexpr( utility::FloatingPointValue<ValueType> )
		{
			return !IsFloatEqual( x, other.x ) || !IsFloatEqual( y, other.y ) || !IsFloatEqual( z, other.z ) || !IsFloatEqual( w, other.w );
		}
		else
		{
			return x != other.x || y != other.y || z != other.z || w != other.w;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType											&	operator[](
		u64																	index
	) const
	{
		if( index == 0 ) return x;
		if( index == 1 ) return y;
		if( index == 2 ) return z;
		if( index == 3 ) return w;
		diagnostic::Throw( U"Index out of range" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType													&	operator[](
		u64																	index
	)
	{
		if( index == 0 ) return x;
		if( index == 1 ) return y;
		if( index == 2 ) return z;
		if( index == 3 ) return w;
		diagnostic::Throw( U"Index out of range" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType														Magnitude() const noexcept
	{
		return std::sqrt(
			x * x +
			y * y +
			z * z +
			w * w
		);
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType														Length() const noexcept
	{
		return Magnitude();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase													Normalized() const noexcept
	{
		auto magnitude = Magnitude();
		if( magnitude <= SmallValue<ValueType> ) return VectorBase::UnitVector();
		return *this / magnitude;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval VectorBase												UnitVector() noexcept
	{
		return VectorBase { 1.0, 0.0, 0.0, 0.0 };
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 OtherDimensionCount>
	constexpr void															CopyOther(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		if constexpr( OtherDimensionCount == 2 )
		{
			x = other.x;
			y = other.y;
			z = {};
			w = {};
		}
		if constexpr( OtherDimensionCount == 3 )
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = {};
		}
		if constexpr( OtherDimensionCount == 4 )
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = other.w;
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Vec2f32 = VectorBase<2, f32>;
using Vec2f64 = VectorBase<2, f64>;
using Vec2i32 = VectorBase<2, i32>;
using Vec2u32 = VectorBase<2, u32>;
using Vec2i64 = VectorBase<2, i64>;
using Vec2u64 = VectorBase<2, u64>;

using Vec3f32 = VectorBase<3, f32>;
using Vec3f64 = VectorBase<3, f64>;
using Vec3i32 = VectorBase<3, i32>;
using Vec3u32 = VectorBase<3, u32>;
using Vec3i64 = VectorBase<3, i64>;
using Vec3u64 = VectorBase<3, u64>;

using Vec4f32 = VectorBase<4, f32>;
using Vec4f64 = VectorBase<4, f64>;
using Vec4i32 = VectorBase<4, i32>;
using Vec4u32 = VectorBase<4, u32>;
using Vec4i64 = VectorBase<4, i64>;
using Vec4u64 = VectorBase<4, u64>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( Vec2f32 ) == sizeof( f32 ) * 2 );
static_assert( sizeof( Vec2f64 ) == sizeof( f64 ) * 2 );
static_assert( sizeof( Vec2i32 ) == sizeof( i32 ) * 2 );
static_assert( sizeof( Vec2u32 ) == sizeof( u32 ) * 2 );
static_assert( sizeof( Vec2i64 ) == sizeof( i64 ) * 2 );
static_assert( sizeof( Vec2u64 ) == sizeof( u64 ) * 2 );

static_assert( sizeof( Vec3f32 ) == sizeof( f32 ) * 3 );
static_assert( sizeof( Vec3f64 ) == sizeof( f64 ) * 3 );
static_assert( sizeof( Vec3i32 ) == sizeof( i32 ) * 3 );
static_assert( sizeof( Vec3u32 ) == sizeof( u32 ) * 3 );
static_assert( sizeof( Vec3i64 ) == sizeof( i64 ) * 3 );
static_assert( sizeof( Vec3u64 ) == sizeof( u64 ) * 3 );

static_assert( sizeof( Vec4f32 ) == sizeof( f32 ) * 4 );
static_assert( sizeof( Vec4f64 ) == sizeof( f64 ) * 4 );
static_assert( sizeof( Vec4i32 ) == sizeof( i32 ) * 4 );
static_assert( sizeof( Vec4u32 ) == sizeof( u32 ) * 4 );
static_assert( sizeof( Vec4i64 ) == sizeof( i64 ) * 4 );
static_assert( sizeof( Vec4u64 ) == sizeof( u64 ) * 4 );

static_assert( alignof( Vec2f32 ) == alignof( f32 ) * 2 );
static_assert( alignof( Vec2f64 ) == alignof( f64 ) * 2 );
static_assert( alignof( Vec2i32 ) == alignof( i32 ) * 2 );
static_assert( alignof( Vec2u32 ) == alignof( u32 ) * 2 );
static_assert( alignof( Vec2i64 ) == alignof( i64 ) * 2 );
static_assert( alignof( Vec2u64 ) == alignof( u64 ) * 2 );

static_assert( alignof( Vec3f32 ) == alignof( f32 ) );
static_assert( alignof( Vec3f64 ) == alignof( f64 ) );
static_assert( alignof( Vec3i32 ) == alignof( i32 ) );
static_assert( alignof( Vec3u32 ) == alignof( u32 ) );
static_assert( alignof( Vec3i64 ) == alignof( i64 ) );
static_assert( alignof( Vec3u64 ) == alignof( u64 ) );

static_assert( alignof( Vec4f32 ) == alignof( f32 ) * 4 );
static_assert( alignof( Vec4f64 ) == alignof( f64 ) * 4 );
static_assert( alignof( Vec4i32 ) == alignof( i32 ) * 4 );
static_assert( alignof( Vec4u32 ) == alignof( u32 ) * 4 );
static_assert( alignof( Vec4i64 ) == alignof( i64 ) * 4 );
static_assert( alignof( Vec4u64 ) == alignof( u64 ) * 4 );



} // math
} // bc
