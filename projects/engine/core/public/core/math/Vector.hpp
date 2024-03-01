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
template<size_t DimensionCount, utility::FundamentalType ValueType>
class VectorBase;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType ValueType>
class alignas( sizeof( ValueType ) * 2 ) VectorBase<2, ValueType>
{
public:

	static constexpr size_t DimensionCount = 2;

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
	template<size_t OtherDimensionCount>
	constexpr VectorBase(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherDimensionCount>
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
		if constexpr( utility::FloatingPointType<ValueType> )
		{
			return utility::IsFloatEqual( x, other.x ) && utility::IsFloatEqual( y, other.y );
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
		if constexpr( utility::FloatingPointType<ValueType> )
		{
			return !utility::IsFloatEqual( x, other.x ) || !utility::IsFloatEqual( y, other.y );
		}
		else
		{
			return x != other.x || y != other.y;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType											&	operator[](
		size_t																index
	) const
	{
		if( index == 0 ) return x;
		if( index == 1 ) return y;
		diagnostic::Throw( U"Index out of range" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType													&	operator[](
		size_t																index
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
		if( magnitude <= utility::GetSmallValue<ValueType>() ) return VectorBase::UnitVector();
		return *this / magnitude;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval VectorBase												UnitVector() noexcept
	{
		return VectorBase { 1.0, 0.0 };
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherDimensionCount>
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
template<utility::FundamentalType ValueType>
class VectorBase<3, ValueType>
{
public:

	static constexpr size_t DimensionCount = 3;

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
	template<size_t OtherDimensionCount>
	constexpr VectorBase(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherDimensionCount>
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
		if constexpr( utility::FloatingPointType<ValueType> )
		{
			return utility::IsFloatEqual( x, other.x ) && utility::IsFloatEqual( y, other.y ) && utility::IsFloatEqual( z, other.z );
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
		if constexpr( utility::FloatingPointType<ValueType> )
		{
			return !utility::IsFloatEqual( x, other.x ) || !utility::IsFloatEqual( y, other.y ) || !utility::IsFloatEqual( z, other.z );
		}
		else
		{
			return x != other.x || y != other.y || z != other.z;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType											&	operator[](
		size_t																index
	) const
	{
		if( index == 0 ) return x;
		if( index == 1 ) return y;
		if( index == 2 ) return z;
		diagnostic::Throw( U"Index out of range" );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType													&	operator[](
		size_t																index
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
		if( magnitude <= utility::GetSmallValue<ValueType>() ) return VectorBase::UnitVector();
		return *this / magnitude;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval VectorBase												UnitVector() noexcept
	{
		return VectorBase { 1.0, 0.0, 0.0 };
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherDimensionCount>
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
template<utility::FundamentalType ValueType>
class alignas( sizeof( ValueType ) * 4 ) VectorBase<4, ValueType>
{
public:

	static constexpr size_t DimensionCount = 4;

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
	template<size_t OtherDimensionCount>
	constexpr VectorBase(
		const VectorBase<OtherDimensionCount, ValueType>				&	other
	) noexcept requires( OtherDimensionCount <= DimensionCount )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherDimensionCount>
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
		if constexpr( utility::FloatingPointType<ValueType> )
		{
			return utility::IsFloatEqual( x, other.x ) && utility::IsFloatEqual( y, other.y ) && utility::IsFloatEqual( z, other.z ) && utility::IsFloatEqual( w, other.w );
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
		if constexpr( utility::FloatingPointType<ValueType> )
		{
			return !utility::IsFloatEqual( x, other.x ) || !utility::IsFloatEqual( y, other.y ) || !utility::IsFloatEqual( z, other.z ) || !utility::IsFloatEqual( w, other.w );
		}
		else
		{
			return x != other.x || y != other.y || z != other.z || w != other.w;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType											&	operator[](
		size_t																index
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
		size_t																index
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
		if( magnitude <= utility::GetSmallValue<ValueType>() ) return VectorBase::UnitVector();
		return *this / magnitude;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval VectorBase												UnitVector() noexcept
	{
		return VectorBase { 1.0, 0.0, 0.0, 0.0 };
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherDimensionCount>
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
using Vector2f = VectorBase<2, float>;
using Vector2d = VectorBase<2, double>;
using Vector2i = VectorBase<2, int64_t>;
using Vector2u = VectorBase<2, uint64_t>;

using Vector3f = VectorBase<3, float>;
using Vector3d = VectorBase<3, double>;
using Vector3i = VectorBase<3, int64_t>;
using Vector3u = VectorBase<3, uint64_t>;

using Vector4f = VectorBase<4, float>;
using Vector4d = VectorBase<4, double>;
using Vector4i = VectorBase<4, int64_t>;
using Vector4u = VectorBase<4, uint64_t>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( Vector2f ) == sizeof( float ) * 2 );
static_assert( sizeof( Vector2d ) == sizeof( double ) * 2 );
static_assert( sizeof( Vector2i ) == sizeof( int64_t ) * 2 );
static_assert( sizeof( Vector2u ) == sizeof( uint64_t ) * 2 );

static_assert( sizeof( Vector3f ) == sizeof( float ) * 3 );
static_assert( sizeof( Vector3d ) == sizeof( double ) * 3 );
static_assert( sizeof( Vector3i ) == sizeof( int64_t ) * 3 );
static_assert( sizeof( Vector3u ) == sizeof( uint64_t ) * 3 );

static_assert( sizeof( Vector4f ) == sizeof( float ) * 4 );
static_assert( sizeof( Vector4d ) == sizeof( double ) * 4 );
static_assert( sizeof( Vector4i ) == sizeof( int64_t ) * 4 );
static_assert( sizeof( Vector4u ) == sizeof( uint64_t ) * 4 );

static_assert( alignof( Vector2f ) == alignof( float ) * 2 );
static_assert( alignof( Vector2d ) == alignof( double ) * 2 );
static_assert( alignof( Vector2i ) == alignof( int64_t ) * 2 );
static_assert( alignof( Vector2u ) == alignof( uint64_t ) * 2 );

static_assert( alignof( Vector3f ) == alignof( float ) );
static_assert( alignof( Vector3d ) == alignof( double ) );
static_assert( alignof( Vector3i ) == alignof( int64_t ) );
static_assert( alignof( Vector3u ) == alignof( uint64_t ) );

static_assert( alignof( Vector4f ) == alignof( float ) * 4 );
static_assert( alignof( Vector4d ) == alignof( double ) * 4 );
static_assert( alignof( Vector4i ) == alignof( int64_t ) * 4 );
static_assert( alignof( Vector4u ) == alignof( uint64_t ) * 4 );



} // math
} // bc
