#pragma once

#include <core/containers/backend/ContainerBase.hpp>

#include <core/containers/backend/LinearContainerBase.hpp>

#include <algorithm>
#include <limits>



namespace bc {



template<utility::TextCharacter CharacterType>
class TextBase;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<utility::TextCharacter CharacterType, bool IsConst>
class TextViewBase : public container_bases::LinearContainerViewBase<CharacterType, IsConst>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= container_bases::LinearContainerViewBase<CharacterType, IsConst>;
	using ContainedValueType				= CharacterType;
	using ContainedCharacterType			= CharacterType;
	static constexpr bool IsDataConst		= IsConst;

	template<utility::TextCharacter OtherValueType, bool IsOtherConst>
	using ThisContainerType					= TextViewBase<OtherValueType, IsOtherConst>;
	using ThisType							= ThisContainerType<CharacterType, IsConst>;

	template<bool IsOtherConst>
	using ThisTypeOtherConst				= ThisContainerType<CharacterType, IsOtherConst>;

	template<utility::TextCharacter OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= TextViewBase<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<CharacterType, IsOtherConst>;

	template<utility::TextCharacter OtherValueType>
	using ThisContainerFullType				= TextBase<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<CharacterType>;

	template<bool IsOtherConst>
	using IteratorBase						= container_bases::LinearContainerIteratorBase<ThisType, IsOtherConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= CharacterType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<typename OtherValueType, bool IsOtherConst>
	//friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

	template<utility::TextCharacter OtherValueType, bool IsOtherConst>
	friend class TextViewBase;

	template<utility::TextCharacter OtherValueType>
	friend class TextBase;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextViewBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr TextViewBase( ThisTypeOtherConst<IsOtherConst> other ) noexcept
		requires( utility::IsConstConvertible<IsConst, IsOtherConst> )
	:
		Base(
			other.Data(),
			other.Size()
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr TextViewBase(
		IteratorBase<IsOtherConst>	begin_iterator,
		IteratorBase<IsOtherConst>	end_iterator
	)
		requires( utility::IsConstConvertible<IsConst, IsOtherConst> )
	:
		Base(
			begin_iterator.GetAddress(),
			end_iterator.GetAddress() - begin_iterator.GetAddress()
		)
	{
		BAssert(
			begin_iterator.GetContainer() == end_iterator.GetContainer(),
			U"Begin and end iterators point to different containers"
		);
		BAssert(
			begin_iterator.GetIndex() <= end_iterator.GetIndex(),
			U"Begin iterator is after end iterator"
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<i64 ArraySize>
	constexpr TextViewBase( const CharacterType( &c_string )[ ArraySize ] ) noexcept
		requires( IsConst == true )
	:
		Base(
			c_string,
			ArraySize ? ( c_string[ ArraySize - 1 ] == '\0' ? ArraySize - 1 : ArraySize ) : 0
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextViewBase(
		const CharacterType*	ptr,
		const i64				size
	) noexcept requires( IsConst == true )
		:
		Base(
			ptr,
			size ? ( ptr[ size - 1 ] == '\0' ? size - 1 : size ) : 0
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	explicit constexpr TextViewBase( const CharacterType* ptr ) noexcept
		requires( IsConst == true )
	:
		Base(
			ptr,
			std::char_traits<CharacterType>::length( ptr )
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextViewBase(
		CharacterType*	ptr,
		i64				size
	) noexcept
		requires( IsConst == false )
	:
		Base(
			ptr,
			size ? ( ptr[ size - 1 ] == '\0' ? size - 1 : size ) : 0
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextViewBase(
		const CharacterType*	begin_ptr,
		const CharacterType*	end_ptr
	)
		requires( IsConst == true )
	:
		Base(
			begin_ptr,
			i64( end_ptr - begin_ptr )
		)
	{
		BAssert(
			begin_ptr <= end_ptr,
			U"Begin pointer is after end pointer"
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextViewBase(
		CharacterType*	begin_ptr,
		CharacterType*	end_ptr
	)
		requires( IsConst == false )
	:
		Base(
			begin_ptr,
			i64( end_ptr - begin_ptr )
		)
	{
		BAssert(
			begin_ptr <= end_ptr,
			U"Begin pointer is after end pointer"
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr auto operator=( ThisTypeOtherConst<IsOtherConst> other ) noexcept -> ThisType&
		requires( utility::IsConstConvertible<IsConst, IsOtherConst> )
	{
		this->data_ptr	= other.Data();
		this->data_size	= other.Size();
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text matches another.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// true if this text matches the other, false otherwise.
	template<bool IsOtherConst>
	constexpr auto operator==( ThisTypeOtherConst<IsOtherConst> other ) const noexcept -> bool
	{
		return container_bases::internal_::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text differs from another.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// true if text differs from the other, false if they match.
	template<bool IsOtherConst>
	constexpr auto operator!=( ThisTypeOtherConst<IsOtherConst> other ) const noexcept -> bool
	{
		return container_bases::internal_::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text match with C-style sting literal.
	///
	/// @tparam InputStringSize
	///	Tells the input string size.
	/// 
	/// @param c_string
	///	C-style text to compare with.
	/// 
	/// @return
	/// true if this text matches the other, false otherwise.
	template<i64 ArraySize>
	constexpr auto operator==( const CharacterType( &c_string )[ ArraySize ] ) const noexcept -> bool
	{
		return container_bases::internal_::CheckContainerContentsMatch( *this, ThisTypeOtherConst<true> { c_string, ArraySize } );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text differs from C-style sting.
	/// 
	/// @tparam InputStringSize
	///	Tells the input string size.
	/// 
	/// @param c_string
	///	C-style text to compare with.
	/// 
	/// @return
	/// true if text differs from the other, false if they match.
	template<i64 ArraySize>
	constexpr auto operator!=( const CharacterType( &c_string )[ ArraySize ] ) const noexcept -> bool
	{
		return container_bases::internal_::CheckContainerContentsDiffer( *this, ThisTypeOtherConst<true> { c_string, ArraySize } );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if this text is lexicographically less than another text.
	/// 
	///	Needed for sorting and sorted lists.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// True if this text is lexicographically less than the other. False if the other text equal or greater.
	constexpr auto operator<( ThisTypeOtherConst<true> other ) const -> bool
	{
		return std::lexicographical_compare<ConstIterator, ConstIterator>(
			this->begin(), this->end(),
			other.begin(), other.end()
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param character
	///	Character to find and count occurances of.
	/// 
	/// @param position
	///	Position of where to start searching from, default 0. Range will be clamped between 0 and size of the text.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Number of characters found.
	[[nodiscard]]
	constexpr auto CountCharacters(
		CharacterType	character,
		i64				position			= 0,
		i64				search_length		= std::numeric_limits<i64>::max()
	) const noexcept -> i64
	{
		if( this->data_size == 0 ) [[unlikely]] return 0;
		if( search_length == 0 ) [[unlikely]] return 0;

		auto [start, end] = ClampSearchRange( position, search_length );

		auto counter = i64 {};
		auto it = this->data_ptr + start;
		auto it_end = this->data_ptr + end;
		while( it != it_end )
		{
			if( *it == character )
			{
				++counter;
			}
			++it;
		}
		return counter;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param character
	///	Character to find and count.
	/// 
	/// @param position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Number of characters found.
	[[nodiscard]]
	constexpr auto CountCharacters(
		CharacterType	character,
		ConstIterator	position,
		i64				search_length		= std::numeric_limits<i64>::max()
	) const -> i64
	{
		BAssert( position.container == this, U"Wrong iterator used on this container" );
		BAssert(
			position.data >= this->data_ptr && position.data < this->data_ptr + this->data_size,
			U"Iterator out of range"
		);
		i64 p = position.data - this->data_ptr;
		return this->CountCharacters( character, p, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a character inside this text view.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param position
	///	Position of where to start searching from, default 0. Range will be clamped between 0 and size of the text.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]]
	constexpr auto FindCharacter(
		CharacterType	character,
		i64				position			= 0,
		i64				search_length		= std::numeric_limits<i64>::max()
	) const noexcept -> ConstIterator
	{
		if( this->data_size == 0 ) [[unlikely]] return 0;
		if( search_length == 0 ) [[unlikely]] return 0;

		auto [start, end] = ClampSearchRange( position, search_length );

		auto it = this->data_ptr + start;
		auto it_end = this->data_ptr + end;
		while( it < it_end )
		{
			if( *it == character )
			{
				return ConstIterator( this, it );
			}
			++it;
		}
		return ConstIterator( this, it_end );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a character inside this text view.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]]
	constexpr auto FindCharacter(
		CharacterType	character,
		ConstIterator	position,
		i64				search_length		= std::numeric_limits<i64>::max()
	) const -> ConstIterator
	{
		BAssert( position.container == this, U"Wrong iterator used on this container" );
		BAssert(
			position.data >= this->data_ptr && position.data < this->data_ptr + this->data_size,
			U"Iterator out of range"
		);
		i64 p = position.data - this->data_ptr;
		return this->FindCharacter( character, p, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text view.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param position
	///	Position of where to start searching from, default 0. Range will be clamped between 0 and size of the text.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]]
	constexpr auto Find(
		ThisViewType<true>	text_to_find,
		i64					position			= 0,
		i64					search_length		= std::numeric_limits<i64>::max()
	) const noexcept -> ConstIterator
	{
		if( this->data_size == 0 ) [[unlikely]] return {};

		auto my_size = this->Size();
		auto other_size = text_to_find.Size();

		auto [start, end] = ClampSearchRange( position, search_length );
		i64 max_end = my_size - other_size;
		if( end > max_end ) end = max_end;

		for( i64 outer = start; outer <= end; ++outer )
		{
			i64 inner;
			for( inner = 0; inner < text_to_find.Size(); ++inner )
			{
				if( this->data_ptr[ outer + inner ] != text_to_find.Data()[ inner ] ) break;
			}
			if( inner == text_to_find.Size() )
			{
				return ConstIterator( this, this->data_ptr + outer );
			}
		}
		return ConstIterator( this, this->data_ptr + my_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text view.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]]
	constexpr auto Find(
		ThisViewType<true>	text_to_find,
		ConstIterator		position,
		i64					search_length		= std::numeric_limits<i64>::max()
	) const -> ConstIterator
	{
		BAssert( position.container == this, U"Wrong iterator used on this container" );
		BAssert(
			position.data >= this->data_ptr && position.data < this->data_ptr + this->data_size,
			U"Iterator out of range"
		);
		i64 p = position.data - this->data_ptr;
		return this->Find( text_to_find, p, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get a sub-text from this text view.
	///
	/// @note
	/// If ```size``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param position
	///	Position from the start where to start including text.
	///
	/// @param size
	///	Number of characters to include. Default is ```std::numeric_limits<i64>::max()```. Positive numbers include text after
	/// ```position```, negative numbers include text in front of ```position```.
	///
	/// @return
	/// A new text view containing a range to data in this text view.
	constexpr auto SubText(
		i64	position,
		i64	size		= std::numeric_limits<i64>::max()
	) const noexcept -> ThisType
	{
		auto [start, end] = ClampSearchRange( position, size );
		i64 length = end - start;

		if( length <= 0 ) return {};

		return { this->data_ptr + start, length };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get a sub-text from this text view.
	///
	/// @param begin_iterator
	///	Iterator start position where to start including text.
	///
	/// @param end_iterator
	///	Iterator end position where to stop including text. (non-inclusive)
	///
	/// @return
	/// A new text view containing a range to data in this text view.
	constexpr auto SubText(
		ConstIterator begin_iterator,
		ConstIterator end_iterator
	) const -> ThisType
	{
		BAssert(
			reinterpret_cast<const void*>( begin_iterator.GetContainer() ) == this,
			U"Begin iterator points to a different container"
		);
		BAssert(
			reinterpret_cast<const void*>( end_iterator.GetContainer() ) == this,
			U"End iterator points to a different container"
		);
		i64 begin_position = begin_iterator.GetAddress() - this->data_ptr;
		i64 end_position = end_iterator.GetAddress() - this->data_ptr;
		i64 length = end_position - begin_position;
		if( length < 0 ) length = 0;
		return { begin_iterator.GetAddress(), length };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get a sub-text from this text view.
	///
	/// @note
	/// Includes text starting from ```begin_iterator``` to the end of the text.
	///
	/// @param begin_iterator
	///	Iterator start position where to start including text.
	///
	/// @return
	/// A new text view containing a range to data in this text view.
	constexpr auto SubText(
		ConstIterator begin_iterator
	) const -> ThisType
	{
		return this->SubText( begin_iterator, this->end() );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first character.
	[[nodiscard]]
	constexpr auto begin() noexcept -> IteratorBase<IsDataConst>
	{
		return IteratorBase<IsDataConst> { this, &this->data_ptr[ 0 ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first character.
	[[nodiscard]]
	constexpr auto begin() const noexcept -> ConstIterator
	{
		return ConstIterator { this, &this->data_ptr[ 0 ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first character.
	[[nodiscard]]
	constexpr auto cbegin() const noexcept -> ConstIterator
	{
		return this->begin();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to one over the last character.
	[[nodiscard]]
	constexpr auto end() noexcept -> IteratorBase<IsDataConst>
	{
		return IteratorBase<IsDataConst> { this, &this->data_ptr[ this->data_size ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to one over the last character.
	[[nodiscard]]
	constexpr auto end() const noexcept -> ConstIterator
	{
		return ConstIterator { this, &this->data_ptr[ this->data_size ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to one over the last character.
	[[nodiscard]]
	constexpr auto cend() const noexcept -> ConstIterator
	{
		return this->end();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct ClampSearchRangeResult { i64 start, end; };
	auto ClampSearchRange(
		i64	position,
		i64	search_length
	) const -> ClampSearchRangeResult
	{
		i64 my_size = this->Size();

		auto result = ClampSearchRangeResult {};
		if( search_length > 0 )
		{
			// Positive search range
			if( search_length > my_size ) search_length = my_size;
			result.start	= position;
			result.end		= position + search_length;
		}
		else
		{
			// Negative search range
			if( search_length < -my_size ) search_length = -my_size;
			result.start	= position + search_length;
			result.end		= position;
		}
		result.start = std::clamp( result.start, i64( 0 ), my_size );
		result.end = std::clamp( result.end, i64( 0 ), my_size );
		return result;
	}

};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Text is a fixed length character string similarly to std::string.
/// 
/// @tparam CharacterType
/// Data type of single character.
template<utility::TextCharacter CharacterType>
class TextBase : public container_bases::LinearContainerBase<CharacterType>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= container_bases::LinearContainerBase<CharacterType>;
	using ContainedValueType				= CharacterType;
	using ContainedCharacterType			= CharacterType;
	static constexpr bool IsDataConst		= false;

	template<utility::TextCharacter OtherValueType>
	using ThisContainerType					= TextBase<OtherValueType>;
	using ThisType							= ThisContainerType<CharacterType>;

	template<utility::TextCharacter OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= TextViewBase<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<CharacterType, IsOtherConst>;

	template<utility::TextCharacter OtherValueType>
	using ThisContainerFullType				= TextBase<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<CharacterType>;

	template<bool IsOtherConst>
	using IteratorBase						= container_bases::LinearContainerIteratorBase<ThisType, IsOtherConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= CharacterType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<bool IsOtherConst>
	//friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

	template<utility::TextCharacter OtherValueType, bool IsOtherConst>
	friend class TextViewBase;

	template<utility::TextCharacter OtherValueType>
	friend class TextBase;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextBase( const ThisType& other )
	{
		this->Append( other, 1, 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextBase( ThisType&& other ) noexcept
	{
		this->Swap( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextBase( const std::initializer_list<CharacterType>& init_list )
	{
		this->Append( init_list, 1, 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr TextBase( ThisViewType<IsOtherConst> other )
	{
		// TODO: Consider removing implicit conversion from view types as this may be accidental leading to performance degradation. It would be better to be explixit about text view to text conversions.
		this->Append( other, 1, 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<i64 ArraySize>
	constexpr TextBase( const CharacterType( &c_string )[ ArraySize ] )
	{
		this->Append( ThisViewType<true> { c_string, ArraySize }, 1, 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<i64 ArraySize>
	constexpr TextBase( const char( &c_string )[ ArraySize ] )
		requires( !std::is_same_v<CharacterType, char> )
	{
		// TODO: Consider removing construction from plain c style char strings as this may be confusing. Instead, create conversion functions to handle construction from plain c style char strings. Then again, this is convenient.
		this->Append( ThisContainerViewType<char, true> { c_string, ArraySize }, 1, 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextBase(
		CharacterType	fill_with_character,
		i64				initialize_with_size	= 1
	)
	{
		this->FillBack( fill_with_character, initialize_with_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const ThisType& other ) -> ThisType&
	{
		if( &other == this ) return *this;

		ThisType { other }.Swap( *this );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( ThisType&& other ) noexcept -> ThisType&
	{
		if( &other == this ) return *this;

		this->Swap( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr auto operator=( const std::initializer_list<CharacterType>& init_list ) -> ThisType&
	{
		this->Clear();
		this->Append( init_list, 1, 0 );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr auto operator=( ThisViewType<IsOtherConst> other ) -> ThisType&
	{
		if( other.Data() >= this->Data() && other.Data() < this->Data() + this->Size() )
		{
			// Other data is a part of this container, we'll need to do a copy first.
			auto other_copy = TextBase<CharacterType> { other };
			*this = std::move( other_copy );
			return *this;
		}

		this->Clear();
		this->Append( other, 1, 0 );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<i64 ArraySize>
	constexpr auto operator=( const CharacterType( &c_string )[ ArraySize ] ) -> ThisType&
	{
		this->Clear();
		this->Append( ThisViewType<true> { c_string, ArraySize }, 1, 0 );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<i64 ArraySize>
	constexpr auto operator=( const char( &c_string )[ ArraySize ] )->ThisType&
		requires( !std::is_same_v<CharacterType, char> )
	{
		// TODO: Consider removing assigning from plain c style char strings as this may be confusing. Instead, create conversion functions to handle construction from plain c style char strings. Then again, this is convenient.
		this->Clear();
		this->Append( ThisContainerViewType<char, true> { c_string, ArraySize }, 1, 0 );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if this string is lexicographically less than another string.
	/// 
	///	Needed for sorting and sorted lists.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// True if this text is lexicographically less than the other. False if the other text equal or greater.
	template<bool IsOtherConst>
	constexpr auto operator<( ThisViewType<IsOtherConst> other ) const -> bool
	{
		return std::lexicographical_compare(
			this->begin(), this->end(),
			other.begin(), other.end()
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if this string is lexicographically less than another string.
	/// 
	///	Needed for sorting and sorted lists.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// True if this text is lexicographically less than the other. False if the other text equal or greater.
	constexpr auto operator<( const ThisType& other ) const -> bool
	{
		return std::lexicographical_compare(
			this->begin(), this->end(),
			other.begin(), other.end()
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Returns a new text object which begins with this and is extended with the other.
	/// 
	/// @param other
	///	Other text to attach at the back of the returned text.
	/// 
	/// @return
	/// New text object.
	template<utility::ContainerView OtherContainerType>
	[[nodiscard]]
	constexpr auto operator+( const OtherContainerType& other ) const -> ThisType
	{
		// TODO: Consider removing this function. It currently accepts any container view type which may be confusing. If removed, add operator+ for the string view instead.
		auto result = TextBase {};
		result.Reserve( this->data_size + other.Size() );
		result.Append( *this, 1, 0 );
		result.Append( other, 1, 0 );
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Returns a new text object which begins with this and is extended with the other.
	/// 
	/// @param other
	///	Other text to attach at the back of the returned text.
	/// 
	/// @return
	/// New text object.
	template<i64 ArraySize>
	[[nodiscard]]
	constexpr auto operator+( const CharacterType( &c_string )[ ArraySize ] ) const -> ThisType
	{
		auto result = TextBase {};
		result.Reserve( this->data_size + ArraySize );
		result.Append( *this, 1, 0 );
		result.Append( TextViewBase<CharacterType, true>( c_string, ArraySize ), 1, 0 );
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Returns a new text object which begins with this and is extended with the other.
	/// 
	/// @param other
	///	Other text to attach at the back of the returned text.
	/// 
	/// @return
	/// New text object.
	template<i64 ArraySize>
	[[nodiscard]]
	constexpr auto operator+( const char( &c_string )[ ArraySize ] ) const->ThisType
		requires( !std::is_same_v<CharacterType, char> )
	{
		// TODO: Consider removing this function, it allows extending any unicode style string with a C style char string.
		auto result = TextBase {};
		result.Reserve( this->data_size + ArraySize );
		result.Append( *this, 1, 0 );
		result.Append( TextViewBase<char, true>( c_string, ArraySize ), 1, 0 );
		return result;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append other text to itself.
	/// 
	/// @param other
	///	Other text to append to the back.
	/// 
	/// @return
	/// Reference to this.
	constexpr auto operator+=( const ThisType& other ) -> ThisType&
	{
		this->Append( other, 1, this->data_size + other.Size() );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append other text to itself.
	/// 
	/// @param other
	///	Other text to append to the back.
	/// 
	/// @return
	/// Reference to this.
	template<bool IsOtherConst>
	constexpr auto operator+=( ThisViewType<IsOtherConst> other ) -> ThisType&
	{
		this->Append( other, 1, this->data_size + other.Size() );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append other text to itself.
	/// 
	/// @param other
	///	Other text to append to the back.
	/// 
	/// @return
	/// Reference to this.
	constexpr auto operator+=( const std::initializer_list<CharacterType>& other ) -> ThisType&
	{
		this->Append( other, 1, this->data_size + other.size() );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append other container contents to itself.
	/// 
	/// @param other
	///	Other container to append to the back.
	/// 
	/// @return
	/// Reference to this.
	template<utility::ContainerView OtherContainerType>
	constexpr auto operator+=( const OtherContainerType& other ) -> ThisType&
	{
		// TODO: Consider removing this function. It currently accepts any container view type which may be confusing. If removed, add operator+ for the string view instead.
		this->Append( other, 1, this->data_size + other.Size() );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append other text to itself.
	/// 
	/// @param c_string
	///	Other text to append to the back.
	/// 
	/// @return
	/// Reference to this.
	template<i64 ArraySize>
	constexpr auto operator+=( const CharacterType( &c_string )[ ArraySize ] ) -> ThisType&
	{
		this->Append( ThisViewType<true> { c_string, ArraySize }, 1, this->data_size + ArraySize );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append other text to itself.
	/// 
	/// @param c_string
	///	Other text to append to the back.
	/// 
	/// @return
	/// Reference to this.
	template<i64 ArraySize>
	constexpr auto operator+=( const char( &c_string )[ ArraySize ] )->ThisType&
		requires( !std::is_same_v<CharacterType, char> )
	{
		// TODO: Consider removing this function, it allows extending any unicode style string with a C style char string.
		this->Append( TextViewBase<char, true>( c_string, ArraySize ), 1, this->data_size + ArraySize );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text matches another.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// true if this text matches the other, false otherwise.
	constexpr auto operator==( const ThisType& other ) const noexcept -> bool
	{
		if( &other == this ) return true;

		return container_bases::internal_::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text differs from another.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// true if text differs from the other, false if they match.
	constexpr auto operator!=( const ThisType& other ) const noexcept -> bool
	{
		if( &other == this ) return false;

		return container_bases::internal_::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text matches another.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// true if this text matches the other, false otherwise.
	template<bool IsOtherConst>
	constexpr auto operator==( ThisViewType<IsOtherConst> other ) const noexcept -> bool
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::internal_::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief Check if text differs from another.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// true if text differs from the other, false if they match.
	template<bool IsOtherConst>
	constexpr auto operator!=( ThisViewType<IsOtherConst> other ) const noexcept -> bool
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return false;

		return container_bases::internal_::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text matches another.
	/// 
	/// @param other
	///	Other text to compare with.
	/// 
	/// @return
	/// true if this text matches the other, false otherwise.
	template<bool IsOtherConst>
	constexpr auto operator==( ThisContainerViewType<char, IsOtherConst> other ) const noexcept -> bool
		requires( !std::is_same_v<CharacterType, char> )
	{
		// TODO: Consider removing this function as it allows testing string equality with a text view of type char. This may be confusing.
		return container_bases::internal_::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text differs from another.
	/// 
	/// @param other
	/// Other text to compare with.
	/// 
	/// @return
	/// true if text differs from the other, false if they match.
	template<bool IsOtherConst>
	constexpr auto operator!=( ThisContainerViewType<char, IsOtherConst> other ) const noexcept -> bool
		requires( !std::is_same_v<CharacterType, char> )
	{
		return container_bases::internal_::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text match with C-style sting literal.
	///
	/// @tparam InputStringSize
	///	Tells the input string size.
	/// 
	/// @param c_string
	///	C-style text to compare with.
	/// 
	/// @return
	/// true if this text matches the other, false otherwise.
	template<i64 ArraySize>
	constexpr auto operator==( const CharacterType( &c_string )[ ArraySize ] ) const noexcept -> bool
	{
		return container_bases::internal_::CheckContainerContentsMatch( *this, TextViewBase<CharacterType, true>{ c_string, ArraySize } );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text differs from C-style sting.
	/// 
	/// @tparam InputStringSize
	///	Tells the input string size.
	/// 
	/// @param c_string
	///	C-style text to compare with.
	/// 
	/// @return
	/// true if text differs from the other, false if they match.
	template<i64 ArraySize>
	constexpr auto operator!=( const CharacterType( &c_string )[ ArraySize ] ) const noexcept -> bool
	{
		return container_bases::internal_::CheckContainerContentsDiffer( *this, TextViewBase<CharacterType, true>{ c_string, ArraySize } );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text match with C-style sting literal.
	///
	/// @tparam InputStringSize
	///	Tells the input string size.
	/// 
	/// @param c_string
	///	C-style text to compare with.
	/// 
	/// @return
	/// true if this text matches the other, false otherwise.
	template<i64 ArraySize>
	constexpr auto operator==( const char( &c_string )[ ArraySize ] ) const noexcept -> bool
		requires( !std::is_same_v<char, CharacterType> )
	{
		// TODO: Consider removing this function as it allows testing string equality with C style char strings. This may be confusing.
		return container_bases::internal_::CheckContainerContentsMatch( *this, TextViewBase<char, true>{ c_string, ArraySize } );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text differs from C-style sting.
	/// 
	/// @tparam InputStringSize
	/// Tells the input string size.
	/// 
	/// @param c_string
	///	C-style text to compare with.
	/// 
	/// @return
	/// true if text differs from the other, false if they match.
	template<i64 ArraySize>
	constexpr auto operator!=( const char( &c_string )[ ArraySize ] ) const noexcept -> bool
		requires( !std::is_same_v<char, CharacterType> )
	{
		// TODO: Consider removing this function as it allows testing string in-equality with C style char strings. This may be confusing.
		return container_bases::internal_::CheckContainerContentsDiffer( *this, TextViewBase<char, true>{ c_string, ArraySize } );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase the first found character from this container.
	/// 
	///	Remaining character at the end are moved to fill the gap if erasing from the middle.
	/// 
	/// @param character
	///	Character to erase from the list, if not found, returns an iterator to the end.
	/// 
	/// @return
	/// Iterator to the next character which replaced the erased character or end if not found.
	constexpr auto Erase( const CharacterType& character ) -> Iterator
	{
		return Iterator {
			this,
			this->DoErase( character )
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase character at iterator location.
	///
	///	Remaining values at the end are moved to fill the gap if erasing from the middle.
	/// 
	/// @param at
	///	Iterator position to character which to erase.
	/// 
	/// @return
	/// Iterator to the next character which replaced the erased character, if erased last character then returned iterator points
	/// to the end.
	constexpr auto Erase( ConstIterator at ) -> Iterator
	{
		BAssert(
			at.GetContainer() == this,
			U"Cannot erase using iterator that doesn't point to the container we're erasing from"
		);
		return Iterator {
			this,
			this->DoErase( at.GetAddress() )
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase character range.
	///
	///	Remaining values at the end are moved to fill the gap if erasing from the middle.
	/// 
	/// @param from
	///	Iterator to first character to erase.
	/// 
	/// @param to
	///	Iterator to last character where to stop erasing. This character is not erased from this container, rather it tells the
	/// position where to stop.
	/// 
	/// @return
	/// Iterator to the the first character which was not erased.
	constexpr auto Erase(
		ConstIterator from,
		ConstIterator to
	) -> Iterator
	{
		BAssert(
			from.GetContainer() == this,
			U"Cannot erase using 'from' iterator that doesn't point to the container we're erasing from"
		);
		BAssert(
			to.GetContainer() == this,
			U"Cannot erase using 'to' iterator that doesn't point to the container we're erasing from"
		);
		return Iterator {
			this,
			this->DoErase( from.GetAddress(), to.GetAddress() )
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Inserts another character at position.
	///
	///	This insert function copies the character.
	///
	/// @param at
	///	Iterator location where to insert the new character.
	///
	/// @param character
	/// New character to insert.
	///
	/// @param count
	///	How many times should the new character be inserted. Eg. When inserting a character, 't' with count 4, you may consider it
	/// as if "tttt" was inserted. Must be positive number.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded. You typically do not need this. Must be positive number.
	///
	/// @return
	/// Iterator to the next character after inserted character.
	constexpr auto Insert(
		ConstIterator			at,
		const CharacterType&	character,
		i64						count			= 1,
		i64						headroom		= 0
	) -> Iterator
		requires( std::is_copy_constructible_v<CharacterType> )
	{
		BAssert( count >= 0, U"Count must be positive number" );
		BAssert( headroom >= 0, U"Headroom must be positive number" );
		BAssert( at.GetContainer() && at.Get(), U"Empty iterator" );
		BAssert( at.GetContainer() == this, U"Iterator points to a wrong container" );
		return Iterator {
			this,
			this->DoInsert(
				at.GetAddress(),
				character,
				count,
				headroom
			)
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Inserts values from another container at position, optionally multiple times.
	///
	/// This insert function copies values.
	///
	/// @param at
	///	Iterator location where to start inserting other values.
	///
	/// @param other
	///	Linear container view type to insert values from.
	///
	/// @param count
	///	How many times should the new character be inserted. Eg. When inserting a character, 't' with count 4, you may consider it
	/// as if "tttt" was inserted. Must be positive number.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded. You typically do not need this. Must be positive number.
	///
	/// @return
	/// Iterator one past the the last inserted character. Or first original character after insertion.
	template<utility::ContainerView OtherContainerType>
	constexpr auto Insert(
		ConstIterator				at,
		const OtherContainerType&	other,
		i64							count			= 1,
		i64							headroom		= 0
	) -> Iterator
		requires( std::is_copy_constructible_v<CharacterType> && std::is_same_v<CharacterType, typename OtherContainerType::ContainedValueType> )
	{
		BAssert( count >= 0, U"Count must be positive number" );
		BAssert( headroom >= 0, U"Headroom must be positive number" );
		BAssert( at.GetContainer() && at.Get(), U"Empty iterator" );
		BAssert( at.GetContainer() == this, U"Iterator points to a wrong container" );
		return Iterator {
			this,
			this->DoInsert(
				at.GetAddress(),
				other,
				count,
				headroom
			)
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Insert C-style string anywhere in this text.
	///
	///	If inserting in the middle of the text, existing text is split to make space for the new text.
	///
	/// @tparam ArraySize
	///	Size of the C-style string.
	///
	/// @param at
	///	Iterator position where new text should be added. Eg. Consider text "abcd", if inserting "HH" at position <tt>begin() +
	/// 2</tt> then resulting text is "abHHcd".
	/// 
	/// @param c_string
	///	C-style string to insert.
	///
	/// @param count
	///	How many times should the new character be inserted. Eg. When inserting a character, 't' with count 4, you may consider it
	/// as if "tttt" was inserted. Must be positive number.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded. You typically do not need this. Must be positive number.
	///
	/// @return
	/// Iterator to the first original character after insertion. Eg. If inserting "HH" at position <tt>begin() + 2</tt> then
	/// returned iterator points to <tt>begin() + 4</tt>.
	template<i64 ArraySize>
	constexpr auto Insert(
		ConstIterator			at,
		const CharacterType( &c_string )[ ArraySize ],
		i64						count							= 1,
		i64						headroom						= 0
	) -> Iterator
	{
		return this->Insert( at, ThisViewType<CharacterType, true> { c_string, ArraySize }, count, headroom );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Insert C-style string anywhere in this text.
	///
	///	If inserting in the middle of the text, existing text is split to make space for the new text.
	///
	/// @tparam ArraySize
	///	Size of the C-style string.
	///
	/// @param at
	///	Iterator position where new text should be added. Eg. Consider text "abcd", if inserting "HH" at position <tt>begin() +
	/// 2</tt> then resulting text is "abHHcd".
	/// 
	/// @param c_string
	///	C-style string to insert.
	///
	/// @param count
	///	How many times should the new character be inserted. Eg. When inserting a character, 't' with count 4, you may consider it
	/// as if "tttt" was inserted. Must be positive number.
	///
	/// @param headroom
	/// How much extra space is reserved if the capacity is expanded. You typically do not need this. Must be positive number.
	///
	/// @return
	/// Iterator to the first original character after insertion. Eg. If inserting "HH" at position <tt>begin() + 2</tt> then
	/// returned iterator points to <tt>begin() + 4</tt>.
	template<i64 ArraySize>
	constexpr auto Insert(
		ConstIterator	at,
		const char( &c_string )[ ArraySize ],
		i64				count							= 1,
		i64				headroom						= 0
	) -> Iterator
		requires( !std::is_same_v<CharacterType, char> )
	{
		// TODO: Consider removing this function. It accepts char strings, which may be confusing.
		return this->Insert( at, ThisContainerViewType<char, true> { c_string, ArraySize }, count, headroom );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Append(
		const std::initializer_list<CharacterType>&	init_list,
		i64											count			= 1,
		i64											headroom		= 0
	)
	{
		// Cannot use this->Base::Append() because other element type is always allowed to be char which is not necessarily
		// the same type as this element type. Base::Append() only handles inputs with same element types as this.

		if( init_list.size() <= 0 ) return;
		if( count <= 0 ) return;
		if( headroom < 0 ) headroom = 0;

		i64 old_size			= this->Size();
		i64 other_size			= init_list.size();
		i64 total_insert_size	= other_size * count;

		this->ResizeNoConstruct( old_size + total_insert_size, headroom );

		for( i64 c = 0; c < count; ++c )
		{
			auto other_it			= init_list.begin();
			auto write_location		= other_size * c + old_size;
			for( i64 i = 0; i < other_size; ++i )
			{
				new( &this->data_ptr[ write_location + i ] ) CharacterType( *other_it );
				++other_it;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<utility::TextContainerView OtherContainerType>
	void Append(
		const OtherContainerType&	other,
		i64							count			= 1,
		i64							headroom		= 0
	)
		requires( std::is_same_v<CharacterType, typename OtherContainerType::ContainedCharacterType> || std::is_same_v<char, typename OtherContainerType::ContainedCharacterType> )
	{
		// TODO: Consider preventing appending from a container which character type is not the same as this. It is confusing to mix text types with char.

		// Cannot use this->Base::Append() because other container element type is always allowed to be char which is not
		// necessarily the same type as this element type. Base::Append() only handles inputs with same element types as this.

		if( other.Size() <= 0 ) return;
		if( count <= 0 ) return;
		if( headroom < 0 ) headroom = 0;

		i64 old_size			= this->Size();
		i64 other_size			= other.Size();
		i64 total_insert_size	= other_size * count;

		this->ResizeNoConstruct( old_size + total_insert_size, headroom );

		for( i64 c = 0; c < count; ++c )
		{
			auto other_it			= other.begin();
			auto write_location		= other_size * c + old_size;
			for( i64 i = 0; i < other_size; ++i )
			{
				new( &this->data_ptr[ write_location + i ] ) CharacterType( *other_it );
				++other_it;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Extend this text by appending another text at the back.
	///
	/// @param c_string
	///	Other text to append elements from.
	template<i64 ArraySize>
	constexpr void Append(
		const CharacterType( &c_string )[ ArraySize ],
		i64						count							= 1,
		i64						headroom						= 0
	)
	{
		this->Append( ThisViewType<true>{ c_string, ArraySize }, count, headroom );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Extend this text by appending another text at the back.
	///
	///	This append allows appending with char type. Bitcrafte considers char as ASCII so it can be
	///	direcly appended to all types.
	///
	/// @param c_string
	/// Other text to append elements from.
	template<i64 ArraySize>
	constexpr void Append(
		const char( &c_string )[ ArraySize ],
		i64			count							= 1,
		i64			headroom						= 0
	)
		requires( !std::is_same_v<CharacterType, char> )
	{
		// TODO: Consider removing this function, it allows appending unicode text with a C style char string. Mixing text types may be confusing.
		this->Append( TextViewBase<char, true>{ c_string, ArraySize }, count, headroom );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param character
	///	Character to find and count occurances of.
	/// 
	/// @param position
	///	Position of where to start searching from, default 0. Range will be clamped between 0 and size of the text.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Number of characters found.
	[[nodiscard]]
	constexpr auto CountCharacters(
		CharacterType	character,
		i64				position			= 0,
		i64				search_length		= std::numeric_limits<i64>::max()
	) const noexcept -> i64
	{
		return ThisViewType<true>( *this ).CountCharacters( character, position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param character
	///	Character to find and count.
	/// 
	/// @param position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Number of characters found.
	[[nodiscard]]
	constexpr auto CountCharacters(
		CharacterType	character,
		ConstIterator	position,
		i64				search_length	= std::numeric_limits<i64>::max()
	) const noexcept -> i64
	{
		return ThisViewType<true>( *this ).CountCharacters( character, position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a character inside this text view.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param position
	///	Position of where to start searching from, default 0. Range will be clamped between 0 and size of the text.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]]
	constexpr auto FindCharacter(
		CharacterType	character,
		i64				position		= 0,
		i64				search_length	= std::numeric_limits<i64>::max()
	) const noexcept -> ConstIterator
	{
		auto result = ThisViewType<true>( *this ).FindCharacter( character, position, search_length );
		return ConstIterator { this, result.GetAddress() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a character inside this text view.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]]
	constexpr auto FindCharacter(
		CharacterType	character,
		ConstIterator	position,
		i64				search_length	= std::numeric_limits<i64>::max()
	) const noexcept -> ConstIterator
	{
		auto result = ThisViewType<true>( *this ).FindCharacter( character, position, search_length );
		return ConstIterator { this, result.GetAddress() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text view.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param position
	///	Position of where to start searching from, default 0. Range will be clamped between 0 and size of the text.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]]
	constexpr auto Find(
		ThisViewType<true>	text_to_find,
		i64					position		= 0,
		i64					search_length	= std::numeric_limits<i64>::max()
	) const noexcept -> ConstIterator
	{
		auto result = ThisViewType<true>( *this ).Find( text_to_find, position, search_length );
		return ConstIterator { this, result.GetAddress() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text view.
	///
	/// @note
	/// If ```search_length``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]]
	constexpr auto Find(
		ThisViewType<true>	text_to_find,
		ConstIterator		position,
		i64					search_length	= std::numeric_limits<i64>::max()
	) const -> ConstIterator
	{
		auto result = ThisViewType<true>( *this ).Find( text_to_find, position, search_length );
		return ConstIterator { this, result.GetAddress() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Replace all occurences of a text inside this text view with another text.
	///
	/// @param text_to_replace
	///	Source text to find.
	/// 
	/// @param replace_with
	///	Text to replace with.
	/// 
	/// @param count
	///	How many times should the ```replace_with``` be inserted in the position where ```text_to_replace``` was found.
	/// Default is 1.
	/// 
	/// @param search_position
	///	Position of where to start searching from, default 0. Range will be clamped between 0 and size of the text. Default is 0.
	/// 
	/// @param search_length
	///	How many characters are included in the search, default is ```std::numeric_limits<i64>::max()```. Positive numbers search
	/// after ```position```, negative numbers search in front of ```position```. Search range will be clamped between 0 and size of
	/// the text.
	void Replace(
		ThisViewType<true>	text_to_replace,
		ThisViewType<true>	replace_with,
		i64					count				= 1,
		i64					search_position		= 0,
		i64					search_length		= std::numeric_limits<i64>::max()
	)
	{
		if( this->IsEmpty() ) return;
		if( text_to_replace.IsEmpty() ) return;

		auto replace_position = this->Find( text_to_replace, search_position, search_length );
		if( !replace_position ) return;

		BAssert( count >= 0, U"Count must be positive" );

		i64 replace_start = replace_position.GetIndex();
		i64 replace_size = replace_with.Size() * count;
		i64 new_size = this->Size() - text_to_replace.Size() + replace_size;
		i64 diff = new_size - this->Size();
		i64 remaining = this->Size() + diff;
		if( diff > 0 )
		{
			// move the remaining text to the end
			this->Resize( new_size );
			for( i64 i = this->Size() - 1; i >= replace_start + replace_size; --i )
			{
				this->data_ptr[ i ] = this->data_ptr[ i - diff ];
			}
		}
		else if( diff < 0 )
		{
			// move the remaining text closer to the start
			for( i64 i = replace_start + replace_size; i < this->Size() + diff; ++i )
			{
				this->data_ptr[ i ] = this->data_ptr[ i - diff ];
			}
			this->Resize( new_size );
		}

		for( i64 i = 0; i < count; ++i )
		{
			for( i64 j = 0; j < replace_with.Size(); ++j )
			{
				this->data_ptr[ replace_start + i * replace_with.Size() + j ] = replace_with[ j ];
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get a sub-text from this text view.
	/// 
	/// @note
	/// If ```size``` is negative, the search will start in front of ```position``` and goes up to ```position``` but not
	/// including ```position```.
	///
	/// @param position
	///	Position from the start where to start including text.
	/// 
	/// @param size
	///	Number of characters to include. Default is ```std::numeric_limits<i64>::max()```. Positive numbers include text after
	/// ```position```, negative numbers include text in front of ```position```.
	/// 
	/// @return
	/// A new text view containing a range to data in this text view.
	constexpr auto SubText(
		i64	position,
		i64	size		= std::numeric_limits<i64>::max()
	) const -> ThisType
	{
		return ThisType( ThisViewType( *this ).SubText( position, size ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get a sub text from this text.
	///
	/// @param begin_it
	///	Iterator to position where to start copying from.
	/// 
	/// @param size
	///	Iterator to the last position where copying will stop. (non-inclusive)
	/// 
	/// @return
	/// New text container object.
	constexpr auto SubText(
		ConstIterator	begin_it,
		ConstIterator	end_it
	)  const -> ThisType
	{
		BAssert( reinterpret_cast<void*>( begin_it.container ) == this, U"Wrong start iterator" );
		BAssert( reinterpret_cast<void*>( end_it.container ) == this, U"Wrong end iterator" );
		i64 begin_position	= begin_it.GetIndex();
		i64 end_position	= end_it.GetIndex();
		return this->SubText( begin_position, end_position );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Converts this text into a null terminated C-style character string.
	///
	/// @note
	/// If the container is empty, this function will return nullptr.
	///
	/// @note
	/// This function will cannot be const because it makes sure the string is null terminated.
	///
	/// @warning
	/// Always use this function to get a C-style string as using Data() will not return a null terminated character string.
	///
	/// @return
	/// C-style, null-terminated character string with same character type as this text.
	constexpr auto ToCStr() -> const CharacterType*
	{
		if( !this->data_ptr ) return nullptr;
		this->Reserve( this->data_size + 1 );
		this->data_ptr[ this->data_size ] = '\0';
		return this->data_ptr;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first character.
	[[nodiscard]]
	constexpr auto begin() noexcept -> IteratorBase<IsDataConst>
	{
		return IteratorBase<IsDataConst> { this, &this->data_ptr[ 0 ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first character.
	[[nodiscard]]
	constexpr auto begin() const noexcept -> ConstIterator
	{
		return ConstIterator { this, &this->data_ptr[ 0 ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to the first character.
	[[nodiscard]]
	constexpr auto cbegin() const noexcept -> ConstIterator
	{
		return this->begin();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to one over the last character.
	[[nodiscard]]
	constexpr auto end() noexcept -> IteratorBase<IsDataConst>
	{
		return IteratorBase<IsDataConst> { this, &this->data_ptr[ this->data_size ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to one over the last character.
	[[nodiscard]]
	constexpr auto end() const noexcept -> ConstIterator
	{
		return ConstIterator { this, &this->data_ptr[ this->data_size ] };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used when iterating over the container.
	/// 
	/// @return
	/// Iterator that points to one over the last character.
	[[nodiscard]]
	constexpr auto cend() const noexcept -> ConstIterator
	{
		return this->end();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	operator ThisViewType<true>() const noexcept
	{
		return ThisViewType<true> { this->data_ptr, this->data_size };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	operator ThisViewType<false>() noexcept
	{
		return ThisViewType<false> { this->data_ptr, this->data_size };
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Text					= TextBase<char>;
using Text8					= TextBase<char8_t>;
using Text16				= TextBase<char16_t>;
using Text32				= TextBase<char32_t>;
using TextView				= TextViewBase<char, true>;
using TextView8				= TextViewBase<char8_t, true>;
using TextView16			= TextViewBase<char16_t, true>;
using TextView32			= TextViewBase<char32_t, true>;
using EditableTextView		= TextViewBase<char, false>;
using EditableTextView8		= TextViewBase<char8_t, false>;
using EditableTextView16	= TextViewBase<char16_t, false>;
using EditableTextView32	= TextViewBase<char32_t, false>;



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( Text ) == 24 );
static_assert( sizeof( Text8 ) == 24 );
static_assert( sizeof( Text16 ) == 24 );
static_assert( sizeof( Text32 ) == 24 );

static_assert( sizeof( TextView ) == 16 );
static_assert( sizeof( TextView8 ) == 16 );
static_assert( sizeof( TextView16 ) == 16 );
static_assert( sizeof( TextView32 ) == 16 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( utility::Container<Text> );
static_assert( utility::Container<Text8> );
static_assert( utility::Container<Text16> );
static_assert( utility::Container<Text32> );

static_assert( utility::ContainerEditableView<EditableTextView> );
static_assert( utility::ContainerEditableView<EditableTextView8> );
static_assert( utility::ContainerEditableView<EditableTextView16> );
static_assert( utility::ContainerEditableView<EditableTextView32> );

static_assert( utility::ContainerView<TextView> );
static_assert( utility::ContainerView<TextView8> );
static_assert( utility::ContainerView<TextView16> );
static_assert( utility::ContainerView<TextView32> );

static_assert( utility::LinearContainer<Text> );
static_assert( utility::LinearContainer<Text8> );
static_assert( utility::LinearContainer<Text16> );
static_assert( utility::LinearContainer<Text32> );

static_assert( utility::LinearContainerEditableView<EditableTextView> );
static_assert( utility::LinearContainerEditableView<EditableTextView8> );
static_assert( utility::LinearContainerEditableView<EditableTextView16> );
static_assert( utility::LinearContainerEditableView<EditableTextView32> );

static_assert( utility::LinearContainerView<TextView> );
static_assert( utility::LinearContainerView<TextView8> );
static_assert( utility::LinearContainerView<TextView16> );
static_assert( utility::LinearContainerView<TextView32> );

static_assert( utility::TextContainer<Text> );
static_assert( utility::TextContainer<Text8> );
static_assert( utility::TextContainer<Text16> );
static_assert( utility::TextContainer<Text32> );

static_assert( utility::TextContainerEditableView<EditableTextView> );
static_assert( utility::TextContainerEditableView<EditableTextView8> );
static_assert( utility::TextContainerEditableView<EditableTextView16> );
static_assert( utility::TextContainerEditableView<EditableTextView32> );

static_assert( utility::TextContainerView<TextView> );
static_assert( utility::TextContainerView<TextView8> );
static_assert( utility::TextContainerView<TextView16> );
static_assert( utility::TextContainerView<TextView32> );


static_assert( utility::ContainerView<Text> );
static_assert( utility::ContainerEditableView<Text> );
static_assert( utility::Container<Text> );

static_assert( utility::ContainerView<EditableTextView> );
static_assert( utility::ContainerEditableView<EditableTextView> );
static_assert( !utility::Container<EditableTextView> );

static_assert( utility::ContainerView<TextView> );
static_assert( !utility::ContainerEditableView<TextView> );
static_assert( !utility::Container<TextView> );

static_assert( utility::LinearContainerView<Text> );
static_assert( utility::LinearContainerEditableView<Text> );
static_assert( utility::LinearContainer<Text> );

static_assert( utility::LinearContainerView<EditableTextView> );
static_assert( utility::LinearContainerEditableView<EditableTextView> );
static_assert( !utility::LinearContainer<EditableTextView> );

static_assert( utility::LinearContainerView<TextView> );
static_assert( !utility::LinearContainerEditableView<TextView> );
static_assert( !utility::LinearContainer<TextView> );

static_assert( utility::TextContainerView<Text> );
static_assert( utility::TextContainerEditableView<Text> );
static_assert( utility::TextContainer<Text> );

static_assert( utility::TextContainerView<EditableTextView> );
static_assert( utility::TextContainerEditableView<EditableTextView> );
static_assert( !utility::TextContainer<EditableTextView> );

static_assert( utility::TextContainerView<TextView> );
static_assert( !utility::TextContainerEditableView<TextView> );
static_assert( !utility::TextContainer<TextView> );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // bc
