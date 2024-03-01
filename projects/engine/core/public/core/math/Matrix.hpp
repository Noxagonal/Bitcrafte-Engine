#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/math/SmallValues.hpp>
#include <core/math/Vector.hpp>

#include <cstdint>
#include <type_traits>
#include <cmath>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t MatrixDimensions, utility::FundamentalType ValueType>
class MatrixBase;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType ValueType>
class alignas( sizeof( ValueType ) * 2 * 2 ) MatrixBase<2, ValueType>
{
public:

	static constexpr size_t MatrixDimensions = 2;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VectorBase<MatrixDimensions, ValueType>									column_1;
	VectorBase<MatrixDimensions, ValueType>									column_2;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase() noexcept :
		column_1( {} ),
		column_2( {} )
	{};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		const VectorBase<MatrixDimensions, ValueType>					&	column_1,
		const VectorBase<MatrixDimensions, ValueType>					&	column_2
	) noexcept :
		column_1( column_1 ),
		column_2( column_2 )
	{}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		ValueType															identity_value
	) noexcept
	{
		*this = MakeIdentity( identity_value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		ValueType m11, ValueType m12,
		ValueType m21, ValueType m22
	) noexcept :
		column_1( m11, m12 ),
		column_2( m21, m22 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		const MatrixBase												&	other
	) noexcept
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase												&	operator=(
		const MatrixBase												&	other
	) noexcept
	{
		CopyOther( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase												&	operator*=(
		const MatrixBase												&	other
	) noexcept
	{
		*this = *this * other;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase													operator*(
		const MatrixBase												&	other
	) const noexcept
	{
		auto & m11 = column_1[ 0 ], & m12 = column_1[ 1 ];
		auto & m21 = column_2[ 0 ], & m22 = column_2[ 1 ];

		auto & o11 = other[ 0 ][ 0 ], & o12 = other[ 0 ][ 1 ];
		auto & o21 = other[ 1 ][ 0 ], & o22 = other[ 1 ][ 1 ];

		return {
			m11 * o11 + m21 * o12,		m12 * o11 + m22 * o12,
			m11 * o21 + m21 * o22,		m12 * o21 + m22 * o22
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase<MatrixDimensions, ValueType>						operator*(
		const VectorBase<MatrixDimensions, ValueType>					&	other
	) const noexcept
	{
		auto & m11 = column_1[ 0 ], & m12 = column_1[ 1 ];
		auto & m21 = column_2[ 0 ], & m22 = column_2[ 1 ];

		auto & o = other;

		return {
			m11 * o.x + m21 * o.y,
			m12 * o.x + m22 * o.y
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator==(
		const MatrixBase												&	other
	) const noexcept
	{
		return column_1 == other.column_1 && column_2 == other.column_2;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator!=(
		const MatrixBase												&	other
	) const noexcept
	{
		return column_1 != other.column_1 || column_2 != other.column_2;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const VectorBase<MatrixDimensions, ValueType>				&	operator[](
		size_t																index
	) const
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( U"Index out of range" );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase<MatrixDimensions, ValueType>					&	operator[](
		size_t																index
	)
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( U"Index out of range" );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval MatrixBase												Identity() noexcept
	{
		return MakeIdentity( static_cast<ValueType>( 1 ) );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void															CopyOther(
		const MatrixBase												&	other
	) noexcept
	{
		column_1 = other.column_1;
		column_2 = other.column_2;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr MatrixBase												MakeIdentity(
		ValueType															identity_value
	)
	{
		return MatrixBase {
			identity_value, {},
			{}, identity_value
		};
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType ValueType>
class MatrixBase<3, ValueType>
{
public:

	static constexpr size_t MatrixDimensions = 3;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VectorBase<MatrixDimensions, ValueType>									column_1;
	VectorBase<MatrixDimensions, ValueType>									column_2;
	VectorBase<MatrixDimensions, ValueType>									column_3;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase() noexcept :
		column_1( {} ),
		column_2( {} ),
		column_3( {} )
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		const VectorBase<MatrixDimensions, ValueType>					&	column_1,
		const VectorBase<MatrixDimensions, ValueType>					&	column_2,
		const VectorBase<MatrixDimensions, ValueType>					&	column_3
	) noexcept :
		column_1( column_1 ),
		column_2( column_2 ),
		column_3( column_3 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		ValueType															identity_value
	) noexcept
	{
		*this = MakeIdentity( identity_value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		ValueType m11, ValueType m12, ValueType m13,
		ValueType m21, ValueType m22, ValueType m23,
		ValueType m31, ValueType m32, ValueType m33
	) noexcept :
		column_1( m11, m12, m13 ),
		column_2( m21, m22, m23 ),
		column_3( m31, m32, m33 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		const MatrixBase												&	other
	) noexcept
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase												&	operator=(
		const MatrixBase												&	other
	) noexcept
	{
		CopyOther( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase												&	operator*=(
		const MatrixBase												&	other
	) noexcept
	{
		*this = *this * other;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase													operator*(
		const MatrixBase												&	other
		) const noexcept
	{
		auto & m11 = column_1[ 0 ], & m12 = column_1[ 1 ], & m13 = column_1[ 2 ];
		auto & m21 = column_2[ 0 ], & m22 = column_2[ 1 ], & m23 = column_2[ 2 ];
		auto & m31 = column_3[ 0 ], & m32 = column_3[ 1 ], & m33 = column_3[ 2 ];

		auto & o11 = other[ 0 ][ 0 ], & o12 = other[ 0 ][ 1 ], & o13 = other[ 0 ][ 2 ];
		auto & o21 = other[ 1 ][ 0 ], & o22 = other[ 1 ][ 1 ], & o23 = other[ 1 ][ 2 ];
		auto & o31 = other[ 2 ][ 0 ], & o32 = other[ 2 ][ 1 ], & o33 = other[ 2 ][ 2 ];

		return {
			m11 * o11 + m21 * o12 + m31 * o13,		m12 * o11 + m22 * o12 + m32 * o13,		m13 * o11 + m23 * o12 + m33 * o13,
			m11 * o21 + m21 * o22 + m31 * o23,		m12 * o21 + m22 * o22 + m32 * o23,		m13 * o21 + m23 * o22 + m33 * o23,
			m11 * o31 + m21 * o32 + m31 * o33,		m12 * o31 + m22 * o32 + m32 * o33,		m13 * o31 + m23 * o32 + m33 * o33
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase<MatrixDimensions, ValueType>						operator*(
		const VectorBase<MatrixDimensions, ValueType>					&	other
	) const noexcept
	{
		auto & m11 = column_1[ 0 ], & m12 = column_1[ 1 ], & m13 = column_1[ 2 ];
		auto & m21 = column_2[ 0 ], & m22 = column_2[ 1 ], & m23 = column_2[ 2 ];
		auto & m31 = column_3[ 0 ], & m32 = column_3[ 1 ], & m33 = column_3[ 2 ];

		auto & o = other;

		return {
			m11 * o.x + m21 * o.y + m31 * o.z,
			m12 * o.x + m22 * o.y + m32 * o.z,
			m13 * o.x + m23 * o.y + m33 * o.z
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator==(
		const MatrixBase												&	other
	) const noexcept
	{
		return column_1 == other.column_1 && column_2 == other.column_2 && column_3 == other.column_3;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator!=(
		const MatrixBase												&	other
	) const noexcept
	{
		return column_1 != other.column_1 || column_2 != other.column_2 || column_3 != other.column_3;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const VectorBase<MatrixDimensions, ValueType>				&	operator[](
		size_t																index
	) const
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( index == 2 ) return column_3;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( U"Index out of range" );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase<MatrixDimensions, ValueType>					&	operator[](
		size_t																index
	)
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( index == 2 ) return column_3;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( U"Index out of range" );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr MatrixBase												Identity() noexcept
	{
		return MakeIdentity( static_cast<ValueType>( 1 ) );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void															CopyOther(
		const MatrixBase												&	other
	) noexcept
	{
		column_1 = other.column_1;
		column_2 = other.column_2;
		column_3 = other.column_3;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr MatrixBase												MakeIdentity(
		ValueType															identity_value
	)
	{
		return {
			identity_value, {}, {},
			{}, identity_value, {},
			{}, {}, identity_value
		};
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType ValueType>
class alignas( sizeof( ValueType ) * 4 * 4 ) MatrixBase<4, ValueType>
{
public:

	static constexpr size_t MatrixDimensions = 4;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VectorBase<MatrixDimensions, ValueType>									column_1;
	VectorBase<MatrixDimensions, ValueType>									column_2;
	VectorBase<MatrixDimensions, ValueType>									column_3;
	VectorBase<MatrixDimensions, ValueType>									column_4;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase() noexcept :
		column_1( {} ),
		column_2( {} ),
		column_3( {} ),
		column_4( {} )
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		const VectorBase<MatrixDimensions, ValueType>					&	column_1,
		const VectorBase<MatrixDimensions, ValueType>					&	column_2,
		const VectorBase<MatrixDimensions, ValueType>					&	column_3,
		const VectorBase<MatrixDimensions, ValueType>					&	column_4
	) noexcept :
		column_1( column_1 ),
		column_2( column_2 ),
		column_3( column_3 ),
		column_4( column_4 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		ValueType															identity_value
	) noexcept
	{
		*this = MakeIdentity( identity_value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		ValueType m11, ValueType m12, ValueType m13, ValueType m14,
		ValueType m21, ValueType m22, ValueType m23, ValueType m24,
		ValueType m31, ValueType m32, ValueType m33, ValueType m34,
		ValueType m41, ValueType m42, ValueType m43, ValueType m44
	) noexcept :
		column_1( m11, m12, m13, m14 ),
		column_2( m21, m22, m23, m24 ),
		column_3( m31, m32, m33, m34 ),
		column_4( m41, m42, m43, m44 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		const MatrixBase												&	other
	) noexcept
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase												&	operator=(
		const MatrixBase												&	other
	) noexcept
	{
		CopyOther( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase												&	operator*=(
		const MatrixBase												&	other
	) noexcept
	{
		*this = *this * other;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase													operator*(
		const MatrixBase												&	other
	) const noexcept
	{
		auto & m11 = column_1[ 0 ], & m12 = column_1[ 1 ], & m13 = column_1[ 2 ], & m14 = column_1[ 3 ];
		auto & m21 = column_2[ 0 ], & m22 = column_2[ 1 ], & m23 = column_2[ 2 ], & m24 = column_2[ 3 ];
		auto & m31 = column_3[ 0 ], & m32 = column_3[ 1 ], & m33 = column_3[ 2 ], & m34 = column_3[ 3 ];
		auto & m41 = column_4[ 0 ], & m42 = column_4[ 1 ], & m43 = column_4[ 2 ], & m44 = column_4[ 3 ];

		auto & o11 = other[ 0 ][ 0 ], & o12 = other[ 0 ][ 1 ], & o13 = other[ 0 ][ 2 ], & o14 = other[ 0 ][ 3 ];
		auto & o21 = other[ 1 ][ 0 ], & o22 = other[ 1 ][ 1 ], & o23 = other[ 1 ][ 2 ], & o24 = other[ 1 ][ 3 ];
		auto & o31 = other[ 2 ][ 0 ], & o32 = other[ 2 ][ 1 ], & o33 = other[ 2 ][ 2 ], & o34 = other[ 2 ][ 3 ];
		auto & o41 = other[ 3 ][ 0 ], & o42 = other[ 3 ][ 1 ], & o43 = other[ 3 ][ 2 ], & o44 = other[ 3 ][ 3 ];

		return {
			m11 * o11 + m21 * o12 + m31 * o13 + m41 * o14,		m12 * o11 + m22 * o12 + m32 * o13 + m42 * o14,		m13 * o11 + m23 * o12 + m33 * o13 + m43 * o14,		m14 * o11 + m24 * o12 + m34 * o13 + m44 * o14,
			m11 * o21 + m21 * o22 + m31 * o23 + m41 * o24,		m12 * o21 + m22 * o22 + m32 * o23 + m42 * o24,		m13 * o21 + m23 * o22 + m33 * o23 + m43 * o24,		m14 * o21 + m24 * o22 + m34 * o23 + m44 * o24,
			m11 * o31 + m21 * o32 + m31 * o33 + m41 * o34,		m12 * o31 + m22 * o32 + m32 * o33 + m42 * o34,		m13 * o31 + m23 * o32 + m33 * o33 + m43 * o34,		m14 * o31 + m24 * o32 + m34 * o33 + m44 * o34,
			m11 * o41 + m21 * o42 + m31 * o43 + m41 * o44,		m12 * o41 + m22 * o42 + m32 * o43 + m42 * o44,		m13 * o41 + m23 * o42 + m33 * o43 + m43 * o44,		m14 * o41 + m24 * o42 + m34 * o43 + m44 * o44
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase<MatrixDimensions, ValueType>						operator*(
		const VectorBase<MatrixDimensions, ValueType>					&	other
	) const noexcept
	{
		auto & m11 = column_1[ 0 ], & m12 = column_1[ 1 ], & m13 = column_1[ 2 ], & m14 = column_1[ 3 ];
		auto & m21 = column_2[ 0 ], & m22 = column_2[ 1 ], & m23 = column_2[ 2 ], & m24 = column_2[ 3 ];
		auto & m31 = column_3[ 0 ], & m32 = column_3[ 1 ], & m33 = column_3[ 2 ], & m34 = column_3[ 3 ];
		auto & m41 = column_4[ 0 ], & m42 = column_4[ 1 ], & m43 = column_4[ 2 ], & m44 = column_4[ 3 ];

		auto & o = other;

		return {
			m11 * o.x + m21 * o.y + m31 * o.z + m41 * o.w,
			m12 * o.x + m22 * o.y + m32 * o.z + m42 * o.w,
			m13 * o.x + m23 * o.y + m33 * o.z + m43 * o.w,
			m14 * o.x + m24 * o.y + m34 * o.z + m44 * o.w
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator==(
		const MatrixBase												&	other
	) const noexcept
	{
		return column_1 == other.column_1 && column_2 == other.column_2 && column_3 == other.column_3 && column_4 == other.column_4;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool															operator!=(
		const MatrixBase												&	other
	) const noexcept
	{
		return column_1 != other.column_1 || column_2 != other.column_2 || column_3 != other.column_3 || column_4 != other.column_4;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const VectorBase<MatrixDimensions, ValueType>				&	operator[](
		size_t																index
	) const
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( index == 2 ) return column_3;
		if( index == 3 ) return column_4;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( U"Index out of range" );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase<MatrixDimensions, ValueType>					&	operator[](
		size_t																index
	)
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( index == 2 ) return column_3;
		if( index == 3 ) return column_4;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( U"Index out of range" );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval MatrixBase												Identity() noexcept
	{
		return MakeIdentity( static_cast<ValueType>( 1 ) );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void															CopyOther(
		const MatrixBase												&	other
	) noexcept
	{
		column_1 = other.column_1;
		column_2 = other.column_2;
		column_3 = other.column_3;
		column_4 = other.column_4;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr MatrixBase												MakeIdentity(
		ValueType															identity_value
	)
	{
		return {
			identity_value, {}, {}, {},
			{}, identity_value, {}, {},
			{}, {}, identity_value, {},
			{}, {}, {}, identity_value
		};
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Matrix2f = MatrixBase<2, float>;
using Matrix2d = MatrixBase<2, double>;
using Matrix2i = MatrixBase<2, int64_t>;

using Matrix3f = MatrixBase<3, float>;
using Matrix3d = MatrixBase<3, double>;
using Matrix3i = MatrixBase<3, int64_t>;

using Matrix4f = MatrixBase<4, float>;
using Matrix4d = MatrixBase<4, double>;
using Matrix4i = MatrixBase<4, int64_t>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( Matrix2f ) == sizeof( float ) * 2 * 2 );
static_assert( sizeof( Matrix2d ) == sizeof( double ) * 2 * 2 );
static_assert( sizeof( Matrix2i ) == sizeof( int64_t ) * 2 * 2 );

static_assert( sizeof( Matrix3f ) == sizeof( float ) * 3 * 3 );
static_assert( sizeof( Matrix3d ) == sizeof( double ) * 3 * 3 );
static_assert( sizeof( Matrix3i ) == sizeof( int64_t ) * 3 * 3 );

static_assert( sizeof( Matrix4f ) == sizeof( float ) * 4 * 4 );
static_assert( sizeof( Matrix4d ) == sizeof( double ) * 4 * 4 );
static_assert( sizeof( Matrix4i ) == sizeof( int64_t ) * 4 * 4 );

static_assert( alignof( Matrix2f ) == alignof( float ) * 4 );
static_assert( alignof( Matrix2d ) == alignof( double ) * 4 );
static_assert( alignof( Matrix2i ) == alignof( int64_t ) * 4 );

static_assert( alignof( Matrix3f ) == alignof( float ) );
static_assert( alignof( Matrix3d ) == alignof( double ) );
static_assert( alignof( Matrix3i ) == alignof( int64_t ) );

static_assert( alignof( Matrix4f ) == alignof( float ) * 16 );
static_assert( alignof( Matrix4d ) == alignof( double ) * 16 );
static_assert( alignof( Matrix4i ) == alignof( int64_t ) * 16 );



} // math
} // bc
