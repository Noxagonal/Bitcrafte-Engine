#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/concepts/ValueConcepts.hpp>
#include <core/utility/math/SmallValues.hpp>
#include <core/math/linear_algebra/Vector.hpp>

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
	union
	{
		struct
		{
			ValueType
				m11, m12,
				m21, m22;
		};
		struct
		{
			VectorBase<MatrixDimensions, ValueType> column_1;
			VectorBase<MatrixDimensions, ValueType> column_2;
		};
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase() noexcept :
		column_1( {} ),
		column_2( {} )
	{};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase(
		const VectorBase<MatrixDimensions, ValueType>						column_1,
		const VectorBase<MatrixDimensions, ValueType>						column_2
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
		m11( m11 ), m12( m12 ),
		m21( m21 ), m22( m22 )
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
		*this = operator*( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase													operator*(
		const MatrixBase												&	other
	) const noexcept
	{
		MatrixBase r;
		auto & o = other;

		r.m11	= m11 * o.m11 + m12 * o.m21;		r.m12	= m11 * o.m12 + m12 * o.m22;
		r.m21	= m21 * o.m11 + m22 * o.m21;		r.m22	= m21 * o.m12 + m22 * o.m21;

		return r;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr VectorBase<MatrixDimensions, ValueType>						operator*(
		const VectorBase<MatrixDimensions, ValueType>					&	other
	) const noexcept
	{
		VectorBase<MatrixDimensions, ValueType> r;
		auto & o = other;

		r.x	= m11 * o.x + m12 * o.y;
		r.y	= m21 * o.x + m22 * o.y;

		return r;
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
class alignas( sizeof( MatrixBase<3, ValueType> ) ) MatrixBase<3, ValueType>
{
	// TODO
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::FundamentalType ValueType>
class alignas( sizeof( MatrixBase<4, ValueType> ) ) MatrixBase<4, ValueType>
{
	// TODO
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
//static_assert( sizeof( Matrix2f ) == sizeof( float ) * 2 );
//static_assert( sizeof( Matrix2d ) == sizeof( double ) * 2 );
//static_assert( sizeof( Matrix2i ) == sizeof( int64_t ) * 2 );
//
//static_assert( sizeof( Matrix3f ) == sizeof( float ) * 4 );
//static_assert( sizeof( Matrix3d ) == sizeof( double ) * 4 );
//static_assert( sizeof( Matrix3i ) == sizeof( int64_t ) * 4 );
//
//static_assert( sizeof( Matrix4f ) == sizeof( float ) * 4 );
//static_assert( sizeof( Matrix4d ) == sizeof( double ) * 4 );
//static_assert( sizeof( Matrix4i ) == sizeof( int64_t ) * 4 );
//
//static_assert( alignof( Matrix2f ) == alignof( float ) * 2 );
//static_assert( alignof( Matrix2d ) == alignof( double ) * 2 );
//static_assert( alignof( Matrix2i ) == alignof( int64_t ) * 2 );
//
//static_assert( alignof( Matrix3f ) == alignof( float ) * 4 );
//static_assert( alignof( Matrix3d ) == alignof( double ) * 4 );
//static_assert( alignof( Matrix3i ) == alignof( int64_t ) * 4 );
//
//static_assert( alignof( Matrix4f ) == alignof( float ) * 4 );
//static_assert( alignof( Matrix4d ) == alignof( double ) * 4 );
//static_assert( alignof( Matrix4i ) == alignof( int64_t ) * 4 );



} // math
} // bc
