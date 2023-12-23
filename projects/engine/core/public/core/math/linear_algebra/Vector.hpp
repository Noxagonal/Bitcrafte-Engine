#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/utility/math/SmallValues.hpp>

#include <cstdint>
#include <type_traits>
#include <cmath>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t VectorLength, utility::FundamentalType ValueType>
class VectorBase;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType ValueType>
class alignas( sizeof( ValueType ) * 2 ) VectorBase<2, ValueType>
{
public:

	static constexpr size_t VectorLength = 2;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																x				= {};
	ValueType																y				= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase() noexcept = default;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase(
		ValueType															x,
		ValueType															y
	) noexcept :
		x( x ),
		y( y )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherVectorLength>
	constexpr VectorBase(
		const VectorBase<OtherVectorLength, ValueType>					&	other
	) noexcept requires( OtherVectorLength <= VectorLength )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherVectorLength>
	constexpr VectorBase												&	operator=(
		const VectorBase<OtherVectorLength, ValueType>					&	other
	) noexcept requires( OtherVectorLength <= VectorLength )
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
	constexpr ValueType														Magnitude() const noexcept
	{
		return std::sqrt(
			x * x +
			y * y
		);
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
	template<size_t OtherVectorLength>
	constexpr void															CopyOther(
		const VectorBase<OtherVectorLength, ValueType>					&	other
	) noexcept requires( OtherVectorLength <= VectorLength )
	{
		if constexpr( OtherVectorLength == 2 )
		{
			x = other.x;
			y = other.y;
		}
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType ValueType>
class alignas( sizeof( ValueType ) * 4 ) VectorBase<3, ValueType>
{
public:

	static constexpr size_t VectorLength = 3;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																x				= {};
	ValueType																y				= {};
	ValueType																z				= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase() noexcept = default;

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
	template<size_t OtherVectorLength>
	constexpr VectorBase(
		const VectorBase<OtherVectorLength, ValueType>					&	other
	) noexcept requires( OtherVectorLength <= VectorLength )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherVectorLength>
	constexpr VectorBase												&	operator=(
		const VectorBase<OtherVectorLength, ValueType>					&	other
	) noexcept requires( OtherVectorLength <= VectorLength )
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
	constexpr ValueType														Magnitude() const noexcept
	{
		return std::sqrt(
			x * x +
			y * y +
			z * z
		);
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
	template<size_t OtherVectorLength>
	constexpr void															CopyOther(
		const VectorBase<OtherVectorLength, ValueType>					&	other
	) requires( OtherVectorLength <= VectorLength )
	{
		if constexpr( OtherVectorLength == 2 )
		{
			x = other.x;
			y = other.y;
			z = {};
		}
		if constexpr( OtherVectorLength == 3 )
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

	static constexpr size_t VectorLength = 4;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ValueType																x					= {};
	ValueType																y					= {};
	ValueType																z					= {};
	ValueType																w					= {};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase() noexcept = default;

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
	template<size_t OtherVectorLength>
	constexpr VectorBase(
		const VectorBase<OtherVectorLength, ValueType>					&	other
	) noexcept requires( OtherVectorLength <= VectorLength )
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t OtherVectorLength>
	constexpr VectorBase												&	operator=(
		const VectorBase<OtherVectorLength, ValueType>					&	other
	) noexcept requires( OtherVectorLength <= VectorLength )
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
	template<size_t OtherVectorLength>
	constexpr void															CopyOther(
		const VectorBase<OtherVectorLength, ValueType>					&	other
	) requires( OtherVectorLength <= VectorLength )
	{
		if constexpr( OtherVectorLength == 2 )
		{
			x = other.x;
			y = other.y;
			z = {};
			w = {};
		}
		if constexpr( OtherVectorLength == 3 )
		{
			x = other.x;
			y = other.y;
			z = other.z;
			w = {};
		}
		if constexpr( OtherVectorLength == 4 )
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

using Vector3f = VectorBase<3, float>;
using Vector3d = VectorBase<3, double>;
using Vector3i = VectorBase<3, int64_t>;

using Vector4f = VectorBase<4, float>;
using Vector4d = VectorBase<4, double>;
using Vector4i = VectorBase<4, int64_t>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( Vector2f ) == sizeof( float ) * 2 );
static_assert( sizeof( Vector2d ) == sizeof( double ) * 2 );
static_assert( sizeof( Vector2i ) == sizeof( int64_t ) * 2 );

static_assert( sizeof( Vector3f ) == sizeof( float ) * 4 );
static_assert( sizeof( Vector3d ) == sizeof( double ) * 4 );
static_assert( sizeof( Vector3i ) == sizeof( int64_t ) * 4 );

static_assert( sizeof( Vector4f ) == sizeof( float ) * 4 );
static_assert( sizeof( Vector4d ) == sizeof( double ) * 4 );
static_assert( sizeof( Vector4i ) == sizeof( int64_t ) * 4 );

static_assert( alignof( Vector2f ) == alignof( float ) * 2 );
static_assert( alignof( Vector2d ) == alignof( double ) * 2 );
static_assert( alignof( Vector2i ) == alignof( int64_t ) * 2 );

static_assert( alignof( Vector3f ) == alignof( float ) * 4 );
static_assert( alignof( Vector3d ) == alignof( double ) * 4 );
static_assert( alignof( Vector3i ) == alignof( int64_t ) * 4 );

static_assert( alignof( Vector4f ) == alignof( float ) * 4 );
static_assert( alignof( Vector4d ) == alignof( double ) * 4 );
static_assert( alignof( Vector4i ) == alignof( int64_t ) * 4 );



} // math
} // bc
