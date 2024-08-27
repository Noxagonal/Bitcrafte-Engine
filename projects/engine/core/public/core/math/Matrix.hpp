#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/math/SmallValues.hpp>
#include <core/math/Vector.hpp>

#include <cstdint>
#include <type_traits>
#include <cmath>



namespace bc {
namespace math {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<u64 MatrixDimensions, utility::Fundamental ValueType>
class MatrixBase;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::Fundamental ValueType>
class alignas( sizeof( ValueType ) * 2 * 2 ) MatrixBase<2, ValueType>
{
public:

	static constexpr u64 MatrixDimensions = 2;

	using VectorType = VectorBase<MatrixDimensions, ValueType>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VectorType	column_1;
	VectorType	column_2;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs an unitialized matrix.
	constexpr MatrixBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase( const MatrixBase& other ) noexcept
	{
		Copy( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs an identity matrix.
	///
	/// @param identity_value
	/// Value to use for the identity matrix diagonal elements.
	constexpr MatrixBase( ValueType identity_value ) noexcept
	{
		*this = MakeIdentity( identity_value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs a matrix directly from column vectors.
	constexpr MatrixBase(
		const VectorType&	column_1,
		const VectorType&	column_2
	) noexcept
	:
		column_1( column_1 ),
		column_2( column_2 )
	{}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs a matrix directly from values.
	///
	/// Parameters are interepreted as ```m<column><row>```. The column number comes first and row number comes second.
	/// m11 is the first value of column 1, m21 is the first value of column 2, m42 is the second value of column 4, etc.
	/// This allows us to write code like this:
	/// @code
	/// auto m = Matrix2f32 {
	///		1.0f, 2.0f,
	///		3.0f, 4.0f
	/// };
	/// // 1.0f, 3.0f make up the first column.
	/// // 2.0f, 4.0f make up the second column.
	/// @endcode
	constexpr MatrixBase(
		ValueType m11,	ValueType m21,
		ValueType m12,	ValueType m22
	) noexcept :
		column_1( m11, m12 ),
		column_2( m21, m22 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const MatrixBase& other ) noexcept -> MatrixBase&
	{
		Copy( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Multiplies this matrix with another matrix and stores the result in this matrix.
	///
	/// @param other
	/// Matrix to multiply with this matrix.
	///
	/// @returns
	/// Reference to this matrix.
	constexpr auto operator*=( const MatrixBase& other ) noexcept -> MatrixBase&
	{
		*this = *this * other;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Multiplies this matrix with another matrix.
	///
	/// @param other
	/// Matrix to multiply with this matrix.
	///
	/// @returns
	/// New matrix result of the multiplication.
	constexpr auto operator*( const MatrixBase& other ) const noexcept -> MatrixBase
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
	/// @brief
	/// Multiplies this matrix with a vector, transforming it.
	///
	/// @param other
	/// Vector to multiply with this matrix.
	///
	/// @returns
	/// New vector result of the multiplication.
	constexpr auto operator*( const VectorType& other ) const noexcept -> VectorType
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
	constexpr auto operator==( const MatrixBase& other ) const noexcept -> bool
	{
		return column_1 == other.column_1 && column_2 == other.column_2;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator!=( const MatrixBase& other ) const noexcept -> bool
	{
		return column_1 != other.column_1 || column_2 != other.column_2;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) const -> const VectorType&
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( diagnostic::Exception( U"Index out of range" ) );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) -> VectorType&
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( diagnostic::Exception( U"Index out of range" ) );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval auto Identity() noexcept -> MatrixBase
	{
		return MakeIdentity( static_cast<ValueType>( 1 ) );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Copy( const MatrixBase& other ) noexcept
	{
		column_1 = other.column_1;
		column_2 = other.column_2;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr auto MakeIdentity( ValueType identity_value ) noexcept -> MatrixBase
	{
		return MatrixBase {
			identity_value, {},
			{}, identity_value
		};
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::Fundamental ValueType>
class MatrixBase<3, ValueType>
{
public:

	static constexpr u64 MatrixDimensions = 3;

	using VectorType = VectorBase<MatrixDimensions, ValueType>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VectorType	column_1;
	VectorType	column_2;
	VectorType	column_3;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs an unitialized matrix.
	constexpr MatrixBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase( const MatrixBase& other ) noexcept
	{
		Copy( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs an identity matrix.
	///
	/// @param identity_value
	/// Value to use for the identity matrix diagonal elements.
	constexpr MatrixBase( ValueType identity_value ) noexcept
	{
		*this = MakeIdentity( identity_value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs a matrix directly from column vectors.
	constexpr MatrixBase(
		const VectorType&	column_1,
		const VectorType&	column_2,
		const VectorType&	column_3
	) noexcept
	:
		column_1( column_1 ),
		column_2( column_2 ),
		column_3( column_3 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs a matrix directly from values.
	///
	/// Parameters are interepreted as ```m<column><row>```. The column number comes first and row number comes second.
	/// m11 is the first value of column 1, m21 is the first value of column 2, m42 is the second value of column 4, etc.
	/// This allows us to write code like this:
	/// @code
	/// auto m = Matrix3f32 {
	///		1.0f, 2.0f, 3.0f,
	///		4.0f, 5.0f, 6.0f,
	///		7.0f, 8.0f, 9.0f
	/// };
	/// // 1.0f, 4.0f, 7.0f make up the first column.
	/// // 2.0f, 5.0f, 8.0f make up the second column.
	/// // 3.0f, 6.0f, 9.0f make up the second column.
	/// @endcode
	constexpr MatrixBase(
		ValueType m11, ValueType m21, ValueType m31,
		ValueType m12, ValueType m22, ValueType m32,
		ValueType m13, ValueType m23, ValueType m33
	) noexcept
	:
		column_1( m11, m12, m13 ),
		column_2( m21, m22, m23 ),
		column_3( m31, m32, m33 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const MatrixBase& other ) noexcept -> MatrixBase&
	{
		Copy( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Multiplies this matrix with another matrix and stores the result in this matrix.
	///
	/// @param other
	/// Matrix to multiply with this matrix.
	///
	/// @returns
	/// Reference to this matrix.
	constexpr auto operator*=( const MatrixBase& other ) noexcept -> MatrixBase&
	{
		*this = *this * other;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Multiplies this matrix with another matrix.
	///
	/// @param other
	/// Matrix to multiply with this matrix.
	///
	/// @returns
	/// New matrix result of the multiplication.
	constexpr auto operator*( const MatrixBase& other ) const noexcept -> MatrixBase
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
	/// @brief
	/// Multiplies this matrix with a vector, transforming it.
	///
	/// @param other
	/// Vector to multiply with this matrix.
	///
	/// @returns
	/// New vector result of the multiplication.
	constexpr auto operator*( const VectorType& other ) const noexcept -> VectorType
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
	constexpr auto operator==( const MatrixBase& other ) const noexcept -> bool
	{
		return column_1 == other.column_1 && column_2 == other.column_2 && column_3 == other.column_3;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator!=( const MatrixBase& other ) const noexcept -> bool
	{
		return column_1 != other.column_1 || column_2 != other.column_2 || column_3 != other.column_3;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) const -> const VectorType&
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( index == 2 ) return column_3;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( diagnostic::Exception( U"Index out of range" ) );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) -> VectorType&
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( index == 2 ) return column_3;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( diagnostic::Exception( U"Index out of range" ) );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr auto Identity() noexcept -> MatrixBase
	{
		return MakeIdentity( static_cast<ValueType>( 1 ) );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Copy( const MatrixBase& other ) noexcept
	{
		column_1 = other.column_1;
		column_2 = other.column_2;
		column_3 = other.column_3;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr auto MakeIdentity( ValueType identity_value ) noexcept -> MatrixBase
	{
		return {
			identity_value, {}, {},
			{}, identity_value, {},
			{}, {}, identity_value
		};
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::Fundamental ValueType>
class alignas( sizeof( ValueType ) * 4 * 4 ) MatrixBase<4, ValueType>
{
public:

	static constexpr u64 MatrixDimensions = 4;

	using VectorType = VectorBase<MatrixDimensions, ValueType>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	VectorType	column_1;
	VectorType	column_2;
	VectorType	column_3;
	VectorType	column_4;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs an unitialized matrix.
	constexpr MatrixBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr MatrixBase( const MatrixBase& other ) noexcept
	{
		Copy( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs an identity matrix.
	///
	/// @param identity_value
	/// Value to use for the identity matrix diagonal elements.
	constexpr MatrixBase( ValueType identity_value ) noexcept
	{
		*this = MakeIdentity( identity_value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs a matrix directly from column vectors.
	constexpr MatrixBase(
		const VectorType&	column_1,
		const VectorType&	column_2,
		const VectorType&	column_3,
		const VectorType&	column_4
	) noexcept
	:
		column_1( column_1 ),
		column_2( column_2 ),
		column_3( column_3 ),
		column_4( column_4 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Constructs a matrix directly from values.
	///
	/// Parameters are interepreted as ```m<column><row>```. The column number comes first and row number comes second.
	/// m11 is the first value of column 1, m21 is the first value of column 2, m42 is the second value of column 4, etc.
	/// This allows us to write code like this:
	/// @code
	/// auto m = Matrix4f32 {
	///		1.0f,  2.0f,  3.0f,  4.0f,
	/// 	5.0f,  6.0f,  7.0f,  8.0f,
	///		9.0f,  10.0f, 11.0f, 12.0f,
	/// 	13.0f, 14.0f, 15.0f, 16.0f
	/// };
	/// // 1.0f, 5.0f, 9.0f, 13.0f make up the first column.
	/// // 2.0f, 6.0f, 10.0f, 14.0f make up the second column.
	/// // 3.0f, 7.0f, 11.0f, 15.0f make up the third column.
	/// // 4.0f, 8.0f, 12.0f, 16.0f make up the fourth column.
	/// @endcode
	constexpr MatrixBase(
		ValueType m11, ValueType m21, ValueType m31, ValueType m41,
		ValueType m12, ValueType m22, ValueType m32, ValueType m42,
		ValueType m13, ValueType m23, ValueType m33, ValueType m43,
		ValueType m14, ValueType m24, ValueType m34, ValueType m44
	) noexcept
	:
		column_1( m11, m12, m13, m14 ),
		column_2( m21, m22, m23, m24 ),
		column_3( m31, m32, m33, m34 ),
		column_4( m41, m42, m43, m44 )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const MatrixBase& other ) noexcept -> MatrixBase&
	{
		Copy( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Multiplies this matrix with another matrix and stores the result in this matrix.
	///
	/// @param other
	/// Matrix to multiply with this matrix.
	///
	/// @returns
	/// Reference to this matrix.
	constexpr auto operator*=( const MatrixBase& other ) noexcept -> MatrixBase&
	{
		*this = *this * other;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Multiplies this matrix with another matrix.
	///
	/// @param other
	/// Matrix to multiply with this matrix.
	///
	/// @returns
	/// New matrix result of the multiplication.
	constexpr auto operator*( const MatrixBase& other ) const noexcept -> MatrixBase
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
	/// @brief
	/// Multiplies this matrix with a vector, transforming it.
	///
	/// @param other
	/// Vector to multiply with this matrix.
	///
	/// @returns
	/// New vector result of the multiplication.
	constexpr auto operator*( const VectorType& other ) const noexcept -> VectorType
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
	constexpr auto operator==( const MatrixBase& other ) const noexcept -> bool
	{
		return column_1 == other.column_1 && column_2 == other.column_2 && column_3 == other.column_3 && column_4 == other.column_4;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator!=( const MatrixBase& other ) const noexcept -> bool
	{
		return column_1 != other.column_1 || column_2 != other.column_2 || column_3 != other.column_3 || column_4 != other.column_4;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) const -> const VectorType&
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( index == 2 ) return column_3;
		if( index == 3 ) return column_4;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( diagnostic::Exception( U"Index out of range" ) );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator[]( i64 index ) -> VectorType&
	{
		if( index == 0 ) return column_1;
		if( index == 1 ) return column_2;
		if( index == 2 ) return column_3;
		if( index == 3 ) return column_4;
		if( !std::is_constant_evaluated() )
		{
			diagnostic::Throw( diagnostic::Exception( U"Index out of range" ) );
		}
		return column_1;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static consteval auto Identity() noexcept -> MatrixBase
	{
		return MakeIdentity( static_cast<ValueType>( 1 ) );
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void Copy( const MatrixBase& other ) noexcept
	{
		column_1 = other.column_1;
		column_2 = other.column_2;
		column_3 = other.column_3;
		column_4 = other.column_4;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr auto MakeIdentity( ValueType identity_value ) noexcept -> MatrixBase
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
using Matrix2f32 = MatrixBase<2, f32>;
using Matrix2f64 = MatrixBase<2, f64>;
using Matrix2i64 = MatrixBase<2, i64>;

using Matrix3f32 = MatrixBase<3, f32>;
using Matrix3f64 = MatrixBase<3, f64>;
using Matrix3i64 = MatrixBase<3, i64>;

using Matrix4f32 = MatrixBase<4, f32>;
using Matrix4f64 = MatrixBase<4, f64>;
using Matrix4i64 = MatrixBase<4, i64>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static_assert( sizeof( Matrix2f32 ) == sizeof( f32 ) * 2 * 2 );
static_assert( sizeof( Matrix2f64 ) == sizeof( f64 ) * 2 * 2 );
static_assert( sizeof( Matrix2i64 ) == sizeof( i64 ) * 2 * 2 );

static_assert( sizeof( Matrix3f32 ) == sizeof( f32 ) * 3 * 3 );
static_assert( sizeof( Matrix3f64 ) == sizeof( f64 ) * 3 * 3 );
static_assert( sizeof( Matrix3i64 ) == sizeof( i64 ) * 3 * 3 );

static_assert( sizeof( Matrix4f32 ) == sizeof( f32 ) * 4 * 4 );
static_assert( sizeof( Matrix4f64 ) == sizeof( f64 ) * 4 * 4 );
static_assert( sizeof( Matrix4i64 ) == sizeof( i64 ) * 4 * 4 );

static_assert( alignof( Matrix2f32 ) == alignof( f32 ) * 4 );
static_assert( alignof( Matrix2f64 ) == alignof( f64 ) * 4 );
static_assert( alignof( Matrix2i64 ) == alignof( i64 ) * 4 );

static_assert( alignof( Matrix3f32 ) == alignof( f32 ) );
static_assert( alignof( Matrix3f64 ) == alignof( f64 ) );
static_assert( alignof( Matrix3i64 ) == alignof( i64 ) );

static_assert( alignof( Matrix4f32 ) == alignof( f32 ) * 16 );
static_assert( alignof( Matrix4f64 ) == alignof( f64 ) * 16 );
static_assert( alignof( Matrix4i64 ) == alignof( i64 ) * 16 );



} // math
} // bc
