
#if BI_CONTAINER_IMPLEMENTATION_NORMAL
#include <core/containers/backend/LinearContainerBaseNormal.hpp>
#elif BI_CONTAINER_IMPLEMENTATION_SIMPLE
#include <core/containers/backend/LinearContainerBaseSimple.hpp>
#else
#error "Container implementation type not given"
#endif

#include <core/conversion/text/utf/UTFConversion.hpp>

#include <cuchar>
#include <climits>
#include <xstring>

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>



namespace bc {



template<container_bases::TextContainerCharacterType CharacterType>
class BC_CONTAINER_NAME( TextBase );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<container_bases::TextContainerCharacterType CharacterType, bool IsConst>
class BC_CONTAINER_NAME( TextViewBase ):
	public container_bases::BC_CONTAINER_NAME( LinearContainerViewBase )<CharacterType, IsConst>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= container_bases::BC_CONTAINER_NAME( LinearContainerViewBase )<CharacterType, IsConst>;
	using ContainedValueType				= CharacterType;
	using ContainedCharacterType			= CharacterType;
	static constexpr bool IsDataConst		= IsConst;

	template<container_bases::TextContainerCharacterType OtherValueType, bool IsOtherConst>
	using ThisContainerType					= BC_CONTAINER_NAME( TextViewBase )<OtherValueType, IsOtherConst>;
	using ThisType							= ThisContainerType<CharacterType, IsConst>;

	template<container_bases::TextContainerCharacterType OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= BC_CONTAINER_NAME( TextViewBase )<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<CharacterType, IsOtherConst>;

	template<container_bases::TextContainerCharacterType OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( TextBase )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<CharacterType>;

	template<bool IsOtherConst>
	using IteratorBase						= container_bases::BC_CONTAINER_NAME( LinearContainerIteratorBase )<CharacterType, IsOtherConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= CharacterType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<typename OtherValueType, bool IsOtherConst>
	//friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

	template<container_bases::TextContainerCharacterType OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( TextViewBase );

	template<container_bases::TextContainerCharacterType OtherValueType>
	friend class BC_CONTAINER_NAME( TextBase );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextViewBase )() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( TextViewBase )(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst>									other
	) noexcept requires( container_bases::IsConstConvertible<IsConst, IsOtherConst> ) :
		Base(
			other.Data(),
			other.Size()
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( TextViewBase )(
		IteratorBase<IsOtherConst>																		begin_iterator,
		IteratorBase<IsOtherConst>																		end_iterator
	) BC_CONTAINER_NOEXCEPT requires( container_bases::IsConstConvertible<IsConst, IsOtherConst> ) :
		Base(
			begin_iterator.Get(),
			end_iterator.Get() - begin_iterator.Get()
		)
	{
		BC_ContainerAssert( begin_iterator.GetContainer() == end_iterator.GetContainer(),
			U"Incompatible begin and end iterators, begin and end iterators should point to the same container",
			U"Begin iterator points to text", conversion::ToUTF32( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true> { begin_iterator.container->Data(), begin_iterator.container->Size() } ),
			U"Begin iterator starts at index", begin_iterator.GetIndex(),
			U"End iterator points to text", conversion::ToUTF32( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true> { end_iterator.container->Data(), end_iterator.container->Size() } ),
			U"End iterator starts at index", end_iterator.GetIndex()
		);
		BC_ContainerAssert( begin_iterator.GetIndex() <= end_iterator.GetIndex(),
			U"Begin iterator is after end iterator, begin must appear before end iterator",
			U"Text", conversion::ToUTF32( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true> { begin_iterator.container->Data(), begin_iterator.container->Size() } ),
			U"Begin iterator starts at index", begin_iterator.GetIndex(),
			U"End iterator starts at index", end_iterator.GetIndex()
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr BC_CONTAINER_NAME( TextViewBase )(
		const CharacterType( &c_string )[ ArraySize ]
	) noexcept requires( IsConst == true ) :
		Base(
			c_string,
			ArraySize ? ( c_string[ ArraySize - 1 ] == '\0' ? ArraySize - 1 : ArraySize ) : 0
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextViewBase )(
		const CharacterType																			*	ptr,
		const size_t																					size
	) noexcept requires( IsConst == true ) :
		Base(
			ptr,
			size ? ( ptr[ size - 1 ] == '\0' ? size - 1 : size ) : 0
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	explicit constexpr BC_CONTAINER_NAME( TextViewBase )(
		const CharacterType																			*	ptr
	) noexcept requires( IsConst == true ) :
		Base(
			ptr,
			std::char_traits<CharacterType>::length( ptr )
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextViewBase )(
		CharacterType																				*	ptr,
		const size_t																					size
	) noexcept requires( IsConst == false ) :
		Base(
			ptr,
			size ? ( ptr[ size - 1 ] == '\0' ? size - 1 : size ) : 0
		)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextViewBase )(
		const CharacterType																			*	begin_ptr,
		const CharacterType																			*	end_ptr
	) BC_CONTAINER_NOEXCEPT requires( IsConst == true ) :
		Base(
			begin_ptr,
			size_t( end_ptr - begin_ptr )
		)
	{
		BC_ContainerAssert( begin_ptr <= end_ptr,
			U"Begin pointer is after end pointer, begin must appear before end memory pointer",
			U"Begin points at memory location", reinterptet_cast<size_t>( begin_ptr ),
			U"End points at memory location", reinterptet_cast<size_t>( end_ptr )
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextViewBase )(
		CharacterType																				*	begin_ptr,
		CharacterType																				*	end_ptr
	) BC_CONTAINER_NOEXCEPT requires( IsConst == false ) :
		Base(
			begin_ptr,
			size_t( end_ptr - begin_ptr )
		)
	{
		BC_ContainerAssert( begin_ptr <= end_ptr,
			U"Begin pointer is after end pointer, begin must appear before end memory pointer",
			U"Begin points at memory location", reinterptet_cast<size_t>( begin_ptr ),
			U"End points at memory location", reinterptet_cast<size_t>( end_ptr )
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( TextViewBase )														&	operator=(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst> other
	) noexcept requires( container_bases::IsConstConvertible<IsConst, IsOtherConst> )
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
	constexpr bool																						operator==(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst>									other
	) const noexcept
	{
		return container_bases::CheckContainerContentsMatch( *this, other );
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
	constexpr bool																						operator!=(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst>									other
	) const noexcept
	{
		return container_bases::CheckContainerContentsDiffer( *this, other );
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
	constexpr bool																						operator==(
		BC_CONTAINER_NAME( TextViewBase )<char, IsOtherConst>											other
	) const noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		return container_bases::CheckContainerContentsMatch( *this, other );
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
	constexpr bool																						operator!=(
		BC_CONTAINER_NAME( TextViewBase )<char, IsOtherConst>											other
	) const noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		return container_bases::CheckContainerContentsDiffer( *this, other );
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
	template<size_t ArraySize>
	constexpr bool																						operator==(
		const CharacterType( &c_string )[ ArraySize ]
	) const noexcept
	{
		return container_bases::CheckContainerContentsMatch( *this, BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>{ c_string, ArraySize } );
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
	template<size_t ArraySize>
	constexpr bool																						operator!=(
		const CharacterType( &c_string )[ ArraySize ]
	) const noexcept
	{
		return container_bases::CheckContainerContentsDiffer( *this, BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>{ c_string, ArraySize } );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if text match with C-style sting literal.
	///
	/// @tparam InputStringSize
	///	Tells the input string size.
	/// 
	/// @param c_string
	/// C-style text to compare with.
	/// 
	/// @return
	/// true if this text matches the other, false otherwise.
	template<size_t ArraySize>
	constexpr bool																						operator==(
		const char( &c_string )[ ArraySize ]
	) const noexcept requires( !std::is_same_v<char, CharacterType> )
	{
		return container_bases::CheckContainerContentsMatch( *this, BC_CONTAINER_NAME( TextViewBase )<char, true>{ c_string, ArraySize } );
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
	template<size_t ArraySize>
	constexpr bool																						operator!=(
		const char( &c_string )[ ArraySize ]
	) const noexcept requires( !std::is_same_v<char, CharacterType> )
	{
		return container_bases::CheckContainerContentsDiffer( *this, BC_CONTAINER_NAME( TextViewBase )<char, true>{ c_string, ArraySize } );
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
	constexpr bool																						operator<(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>											other
	) const BC_CONTAINER_NOEXCEPT
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
	/// @param character
	///	Character to find and count.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr size_t																		CountCharacters(
		CharacterType																					character,
		size_t																							start_position					= 0,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const noexcept
	{
		auto counter = size_t {};

		auto my_size = this->data_size;
		start_position = std::min( start_position, my_size );
		search_length = std::min( search_length, my_size );
		auto it = this->data_ptr + start_position;
		auto it_end = this->data_ptr + search_length;
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
	/// @param character
	///	Character to find and count.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr size_t																		CountCharacters(
		CharacterType																					character,
		ConstIterator																					start_position,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( start_position.container == this, U"Wrong iterator used on this container" );
		BC_ContainerAssert(
			start_position.data >= this->data_ptr &&
			start_position.data < this->data_ptr + this->data_size,
			U"Iterator out of range",
			U"Container size", this->Size(),
			U"Start position", start_position.GetIndex_NoCheck()
		);
		size_t position = start_position.data - this->data_ptr;
		return this->CountCharacters( character, position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a character inside this text view.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator																FindCharacter(
		CharacterType																					character,
		size_t																							start_position					= 0,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const noexcept
	{
		auto my_size = this->Size();
		start_position = std::min( start_position, my_size );
		search_length = std::min( search_length, my_size );
		auto it = this->data_ptr + start_position;
		auto it_end = this->data_ptr + search_length;
		while( it < it_end ) {
			if( *it == character ) {
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
	/// @param character
	///	Character to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator																FindCharacter(
		CharacterType																					character,
		ConstIterator																					start_position,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( start_position.container == this, U"Wrong iterator used on this container" );
		BC_ContainerAssert(
			start_position.data >= this->data_ptr &&
			start_position.data < this->data_ptr + this->data_size,
			U"Iterator out of range",
			U"Container size", this->Size(),
			U"Start position", start_position.GetIndex_NoCheck()
		);
		size_t position = start_position.data - this->data_ptr;
		return this->FindCharacter( character, position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text view.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding text, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator																Find(
		ThisViewType<true>																				text_to_find,
		size_t																							start_position					= 0,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const noexcept
	{
		auto my_size = this->Size();
		auto other_size = text_to_find.Size();
		start_position = std::min( start_position, my_size );
		search_length = std::min( search_length, my_size );
		auto possible_start_count = int64_t( start_position ) - int64_t( std::max( other_size, search_length ) );
		for( int64_t outer = start_position; outer < possible_start_count; ++outer )
		{
			int64_t inner;
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
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding text, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator																Find(
		ThisViewType<true>																				text_to_find,
		ConstIterator																					start_position,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( start_position.container == this, U"Wrong iterator used on this container" );
		BC_ContainerAssert(
			start_position.data >= this->data_ptr &&
			start_position.data < this->data_ptr + this->data_size,
			U"Iterator out of range",
			U"Container size", this->Size(),
			U"Start position", start_position.GetIndex_NoCheck()
		);
		size_t position = start_position.data - this->data_ptr;
		return this->Find( text_to_find, position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get a sub-text from this text view.
	/// 
	/// @param start_position
	///	Position from the start where to start including text.
	/// 
	/// @param size
	///	Number of characters to include. Up to the end of this text view.
	/// 
	/// @return
	/// A new text view containing a range to data in this text view.
	constexpr BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsConst>									SubText(
		size_t																							start_position,
		size_t																							size							= std::numeric_limits<size_t>::max()
	) const noexcept
	{
		if( start_position >= this->data_size ) return {};
		auto length = std::min( this->data_size - start_position, size );
		return { this->data_ptr + start_position, length };
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
	constexpr BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsConst>									SubText(
		ConstIterator																					begin_iterator,
		ConstIterator																					end_iterator
	) const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert(
			reinterpret_cast<const void*>( begin_iterator.GetContainer() ) == this,
			U"Incompatible begin iterator, begin iterator must point to the container it was used with",
			U"Current container text", conversion::ToUTF32( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true> { this->data_ptr, this->data_size } ),
			U"Begin iterator container text", conversion::ToUTF32( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true> { begin_iterator.container->Data(), begin_iterator.container->Size() } ),
			U"Begin iterator points to index", begin_iterator.GetIndex()
		);
		BC_ContainerAssert(
			reinterpret_cast<const void*>( begin_iterator.GetContainer() ) == this,
			U"Incompatible end iterator, end iterator must point to the container it was used with",
			U"Current container text", conversion::ToUTF32( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true> { this->data_ptr, this->data_size } ),
			U"End iterator container text", conversion::ToUTF32( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true> { end_iterator.container->Data(), end_iterator.container->Size() } ),
			U"End iterator points to index", end_iterator.GetIndex()
		);
		size_t begin_position = begin_iterator.Get() - this->data_ptr;
		size_t end_position = end_iterator.Get() - this->data_ptr;
		size_t length;
		if( end_position < begin_position ) {
			length = 0;
		} else {
			length = end_position - begin_position;
		}

		return { begin_iterator.GetData(), length };
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Text is a fixed length character string similarly to std::string.
/// 
/// @tparam CharacterType
/// Data type of single character.
template<container_bases::TextContainerCharacterType CharacterType>
class BC_CONTAINER_NAME( TextBase ) :
	public container_bases::BC_CONTAINER_NAME( LinearContainerBase )<CharacterType>
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= container_bases::BC_CONTAINER_NAME( LinearContainerBase )<CharacterType>;
	using ContainedValueType				= CharacterType;
	using ContainedCharacterType			= CharacterType;
	static constexpr bool IsDataConst		= false;

	template<container_bases::TextContainerCharacterType OtherValueType>
	using ThisContainerType					= BC_CONTAINER_NAME( TextBase )<OtherValueType>;
	using ThisType							= ThisContainerType<CharacterType>;

	template<container_bases::TextContainerCharacterType OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= BC_CONTAINER_NAME( TextViewBase )<OtherValueType, IsOtherConst>;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<CharacterType, IsOtherConst>;

	template<container_bases::TextContainerCharacterType OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( TextBase )<OtherValueType>;
	using ThisFullType						= ThisContainerFullType<CharacterType>;

	template<bool IsOtherConst>
	using IteratorBase						= container_bases::BC_CONTAINER_NAME( LinearContainerIteratorBase )<CharacterType, IsOtherConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= CharacterType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//template<bool IsOtherConst>
	//friend class IteratorBase;

	friend ConstIterator;
	friend Iterator;

	template<container_bases::TextContainerCharacterType OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( TextViewBase );

	template<container_bases::TextContainerCharacterType OtherValueType>
	friend class BC_CONTAINER_NAME( TextBase );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextBase )() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextBase )(
		const BC_CONTAINER_NAME( TextBase )															&	other
	) BC_CONTAINER_NOEXCEPT
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextBase )(
		BC_CONTAINER_NAME( TextBase )																&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<container_bases::TextContainerCharacterType OtherT>
	constexpr BC_CONTAINER_NAME( TextBase )(
		const std::initializer_list<OtherT>															&	init_list
	) BC_CONTAINER_NOEXCEPT requires( std::is_same_v<CharacterType, OtherT> || std::is_same_v<char, OtherT> )
	{
		this->Append( init_list );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( TextBase )(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst>									other
	) BC_CONTAINER_NOEXCEPT
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr BC_CONTAINER_NAME( TextBase )(
		const CharacterType( &c_string )[ ArraySize ]
	) BC_CONTAINER_NOEXCEPT
	{
		this->Append( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>( c_string, ArraySize ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr BC_CONTAINER_NAME( TextBase )(
		const char( &c_string )[ ArraySize ]
	) BC_CONTAINER_NOEXCEPT requires( !std::is_same_v<CharacterType, char> )
	{
		this->Append( BC_CONTAINER_NAME( TextViewBase )<char, true>( c_string, ArraySize ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextBase )(
		CharacterType																					fill_with_character,
		size_t																							initialize_with_size			= 1
	) BC_CONTAINER_NOEXCEPT
	{
		this->FillBack( fill_with_character, initialize_with_size );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator=(
		const BC_CONTAINER_NAME( TextBase )															&	other
	) BC_CONTAINER_NOEXCEPT
	{
		if( &other == this ) return *this;

		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator=(
		BC_CONTAINER_NAME( TextBase )																&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<container_bases::TextContainerCharacterType OtherT>
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator=(
		const std::initializer_list<OtherT>															&	init_list
	) BC_CONTAINER_NOEXCEPT requires( std::is_same_v<CharacterType, OtherT> || std::is_same_v<char, OtherT> )
	{
		this->Clear();
		this->Append( init_list );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator=(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst>									other
	) BC_CONTAINER_NOEXCEPT
	{
		if( other.Data() >= this->Data() && other.Data() < this->Data() + this->Size() )
		{
			// Other data is a part of this container, we'll need to do a copy first.
			auto other_copy = BC_CONTAINER_NAME( TextBase )<CharacterType> { other };
			*this = std::move( other_copy );
			return *this;
		}

		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator=(
		const CharacterType( &c_string )[ ArraySize ]
	) BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
		this->Append( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>( c_string, ArraySize ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<size_t ArraySize>
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator=(
		const char( &c_string )[ ArraySize ]
	) BC_CONTAINER_NOEXCEPT requires( !std::is_same_v<CharacterType, char> )
	{
		this->Clear();
		this->Append( BC_CONTAINER_NAME( TextViewBase )<char, true>( c_string, ArraySize ) );
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
	constexpr bool																						operator<(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst>									other
	) const BC_CONTAINER_NOEXCEPT
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
	constexpr bool																						operator<(
		const BC_CONTAINER_NAME( TextBase )															&	other
	) const BC_CONTAINER_NOEXCEPT
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
	template<container_bases::ContainerView OtherContainerType>
	[[nodiscard]] constexpr BC_CONTAINER_NAME( TextBase )												operator+(
		const OtherContainerType																	&	other
	) const BC_CONTAINER_NOEXCEPT
	{
		auto result = BC_CONTAINER_NAME( TextBase ) {};
		result.Reserve( this->data_size + other.Size() );
		result = *this;
		result += other;
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
	template<size_t ArraySize>
	[[nodiscard]] constexpr BC_CONTAINER_NAME( TextBase )												operator+(
		const CharacterType( &c_string )[ ArraySize ]
	) const BC_CONTAINER_NOEXCEPT
	{
		auto result = BC_CONTAINER_NAME( TextBase ) {};
		result.Reserve( this->data_size + ArraySize );
		result = *this;
		result += BC_CONTAINER_NAME( TextViewBase )<char, true>( c_string, ArraySize );
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
	template<size_t ArraySize>
	[[nodiscard]] constexpr BC_CONTAINER_NAME( TextBase )												operator+(
		const char( &c_string )[ ArraySize ]
	) const BC_CONTAINER_NOEXCEPT requires( !std::is_same_v<CharacterType, char> )
	{
		auto result = BC_CONTAINER_NAME( TextBase ) {};
		result.Reserve( this->data_size + ArraySize );
		result = *this;
		result += BC_CONTAINER_NAME( TextViewBase )<char, true>( c_string, ArraySize );
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
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator+=(
		const BC_CONTAINER_NAME( TextBase )															&	other
	) BC_CONTAINER_NOEXCEPT
	{
		this->Append( other );
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
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator+=(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst>									other
	) BC_CONTAINER_NOEXCEPT
	{
		this->Append( other );
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
	template<container_bases::TextContainerCharacterType OtherT>
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator+=(
		const std::initializer_list<OtherT>															&	other
	) BC_CONTAINER_NOEXCEPT requires( std::is_same_v<CharacterType, OtherT> || std::is_same_v<char, OtherT> )
	{
		this->Append( other );
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
	template<container_bases::ContainerView OtherContainerType>
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator+=(
		const OtherContainerType																	&	other
	) BC_CONTAINER_NOEXCEPT
	{
		this->Append( other );
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
	template<size_t ArraySize>
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator+=(
		const CharacterType( &c_string )[ ArraySize ]
	) BC_CONTAINER_NOEXCEPT
	{
		this->Append( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>( c_string, ArraySize ) );
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
	template<size_t ArraySize>
	constexpr BC_CONTAINER_NAME( TextBase )															&	operator+=(
		const char( &c_string )[ ArraySize ]
	) BC_CONTAINER_NOEXCEPT requires( !std::is_same_v<CharacterType, char> )
	{
		auto reserve_space = this->data_size + ArraySize;
		this->Append( BC_CONTAINER_NAME( TextViewBase )<char, true>( c_string, ArraySize ) );
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
	constexpr bool																						operator==(
		const BC_CONTAINER_NAME( TextBase )															&	other
	) const noexcept
	{
		if( &other == this ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
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
	constexpr bool																						operator!=(
		const BC_CONTAINER_NAME( TextBase )															&	other
	) const noexcept
	{
		if( &other == this ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
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
	constexpr bool																						operator==(
		const BC_CONTAINER_NAME( TextBase )<char>													&	other
	) const noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		return container_bases::CheckContainerContentsMatch( *this, other );
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
	constexpr bool																						operator!=(
		const BC_CONTAINER_NAME( TextBase )<char>													&	other
	) const noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		return container_bases::CheckContainerContentsDiffer( *this, other );
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
	constexpr bool																						operator==(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst>									other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
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
	constexpr bool																						operator!=(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, IsOtherConst>									other
	) const noexcept
	{
		if( other.Data() == this->Data() && other.Size() == this->Size() ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
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
	constexpr bool																						operator==(
		BC_CONTAINER_NAME( TextViewBase )<char, IsOtherConst>											other
	) const noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		return container_bases::CheckContainerContentsMatch( *this, other );
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
	constexpr bool																						operator!=(
		BC_CONTAINER_NAME( TextViewBase )<char, IsOtherConst>											other
	) const noexcept requires( !std::is_same_v<CharacterType, char> )
	{
		return container_bases::CheckContainerContentsDiffer( *this, other );
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
	template<size_t ArraySize>
	constexpr bool																						operator==(
		const CharacterType( &c_string )[ ArraySize ]
	) const noexcept
	{
		return container_bases::CheckContainerContentsMatch( *this, BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>{ c_string, ArraySize } );
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
	template<size_t ArraySize>
	constexpr bool																						operator!=(
		const CharacterType( &c_string )[ ArraySize ]
	) const noexcept
	{
		return container_bases::CheckContainerContentsDiffer( *this, BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>{ c_string, ArraySize } );
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
	template<size_t ArraySize>
	constexpr bool																						operator==(
		const char( &c_string )[ ArraySize ]
	) const noexcept requires( !std::is_same_v<char, CharacterType> )
	{
		return container_bases::CheckContainerContentsMatch( *this, BC_CONTAINER_NAME( TextViewBase )<char, true>{ c_string, ArraySize } );
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
	template<size_t ArraySize>
	constexpr bool																						operator!=(
		const char( &c_string )[ ArraySize ]
	) const noexcept requires( !std::is_same_v<char, CharacterType> )
	{
		return container_bases::CheckContainerContentsDiffer( *this, BC_CONTAINER_NAME( TextViewBase )<char, true>{ c_string, ArraySize } );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Insert text anywhere in this text.
	///
	///	If inserting in the middle of the text, existing text is split to make space for the new text.
	///
	/// @tparam OtherContainerType
	///	Any other text container type.
	///
	/// @param at
	///	Iterator position where new text should be added. Eg. Consider text "abcd", if inserting "HH" at position <tt>begin() +
	/// 2</tt> then resulting text is "abHHcd".
	/// 
	/// @param other
	///	Container containing text to insert.
	///
	/// @param count
	///	How many times other_text should be inserted. Eg. if "text" was inserted with count 2, it is as if "texttext" was inserted.
	/// 
	/// @return
	/// Iterator to the first original character after insertion. Eg. If inserting "HH" at position <tt>begin() + 2</tt> then
	/// returned iterator points to <tt>begin() + 4</tt>.
	template<container_bases::TextContainerView OtherContainerType>
	constexpr Iterator																					Insert(
		ConstIterator																					at,
		const OtherContainerType																	&	other,
		size_t																							count							= 1
	) BC_CONTAINER_NOEXCEPT requires( std::is_same_v<CharacterType, typename OtherContainerType::ContainedCharacterType> || std::is_same_v<char, typename OtherContainerType::ContainedCharacterType> )
	{
		BC_ContainerAssert( reinterpret_cast<const void*>( at.container ) == reinterpret_cast<const void*>( this ),
			"Cannot insert into container using iterator that doesn't point to the container we're inserting into"
		);

		auto CopyFunc = [this, count, &at](
			auto		other
		) -> Iterator
		{
			size_t start_index			= at.GetIndex();
			size_t other_size			= other.Size();
			size_t total_insert_size	= other_size * count;

			this->ShiftRight( start_index, total_insert_size );

			for( size_t c = 0; c < count; ++c ) {
				auto it = other.begin();
				for( size_t i = 0; i < other_size; ++i ) {
					auto count_start_pos = c * other_size + start_index;
					new( &this->data_ptr[ count_start_pos + i ] ) CharacterType( *it );
					++it;
				}
			}
			return Iterator { this, &this->data_ptr[ start_index + total_insert_size ] };
		};

		if constexpr( container_bases::LinearContainerView<OtherContainerType> ) {
			if( reinterpret_cast<const void*>( other.Data() ) >= reinterpret_cast<const void*>( this->data_ptr ) &&
				reinterpret_cast<const void*>( other.Data() ) < reinterpret_cast<const void*>( this->data_ptr + this->data_size ) ) {
				// Other container data is either full or partial range from within this container, need to make a temporary.
				auto other_copy = BC_CONTAINER_NAME( TextBase )<typename OtherContainerType::ContainedCharacterType> { other };
				return CopyFunc( other_copy );
			}
		}

		return CopyFunc( other );
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
	///	How many times the C-style string should be inserted. Eg. if "text" was inserted with count 2, it is as if "texttext" was
	/// inserted.
	/// 
	/// @return
	/// Iterator to the first original character after insertion. Eg. If inserting "HH" at position <tt>begin() + 2</tt> then
	/// returned iterator points to <tt>begin() + 4</tt>.
	template<size_t ArraySize>
	constexpr Iterator																					Insert(
		ConstIterator																					at,
		const CharacterType( &c_string )[ ArraySize ],
		size_t																							count							= 1
	) BC_CONTAINER_NOEXCEPT
	{
		return this->Insert( at, BC_CONTAINER_NAME( TextViewBase )<CharacterType, true> {c_string, ArraySize }, count );
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
	///	How many times the C-style string should be inserted. Eg. if "text" was inserted with count 2, it is as if "texttext" was
	/// inserted.
	/// 
	/// @return
	/// Iterator to the first original character after insertion. Eg. If inserting "HH" at position <tt>begin() + 2</tt> then
	/// returned iterator points to <tt>begin() + 4</tt>.
	template<size_t ArraySize>
	constexpr Iterator																					Insert(
		ConstIterator																					at,
		const char( &c_string )[ ArraySize ],
		size_t																							count							= 1
	) BC_CONTAINER_NOEXCEPT requires( !std::is_same_v<CharacterType, char> )
	{
		return this->Insert( at, BC_CONTAINER_NAME( TextViewBase )<char, true> {c_string, ArraySize }, count );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<container_bases::TextContainerCharacterType OtherT>
	void																								Append(
		const std::initializer_list<OtherT>															&	init_list,
		size_t																							count							= 1
	) BC_CONTAINER_NOEXCEPT requires( std::is_same_v<CharacterType, OtherT> || std::is_same_v<char, OtherT> )
	{
		// Cannot use this->Base::Append() because other element type is always allowed to be char which is not necessarily
		// the same type as this element type. Base::Append() only handles inputs with same element types as this.
		size_t old_size				= this->Size();
		size_t other_size			= init_list.size();
		size_t total_insert_size	= other_size * count;

		this->ResizeNoConstruct( old_size + total_insert_size, 0 );

		for( size_t c = 0; c < count; ++c ) {
			auto other_it			= init_list.begin();
			auto write_location		= other_size * c + old_size;
			for( size_t i = 0; i < other_size; ++i ) {
				new( &this->data_ptr[ write_location + i ] ) CharacterType( *other_it );
				++other_it;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<container_bases::TextContainerView OtherContainerType>
	void																								Append(
		const OtherContainerType																	&	other,
		size_t																							count							= 1
	) BC_CONTAINER_NOEXCEPT requires( std::is_same_v<CharacterType, typename OtherContainerType::ContainedCharacterType> || std::is_same_v<char, typename OtherContainerType::ContainedCharacterType> )
	{
		// Cannot use this->Base::Append() because other container element type is always allowed to be char which is not
		// necessarily the same type as this element type. Base::Append() only handles inputs with same element types as this.
		size_t old_size				= this->Size();
		size_t other_size			= other.Size();
		size_t total_insert_size	= other_size * count;

		this->ResizeNoConstruct( old_size + total_insert_size, 0 );

		for( size_t c = 0; c < count; ++c ) {
			auto other_it			= other.begin();
			auto write_location		= other_size * c + old_size;
			for( size_t i = 0; i < other_size; ++i ) {
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
	template<size_t ArraySize>
	constexpr void																						Append(
		const CharacterType( &c_string )[ ArraySize ],
		size_t																							count							= 1
	) BC_CONTAINER_NOEXCEPT
	{
		this->Append( BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>{ c_string, ArraySize } );
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
	template<size_t ArraySize>
	constexpr void																						Append(
		const char( &c_string )[ ArraySize ],
		size_t																							count							= 1
	) BC_CONTAINER_NOEXCEPT requires( !std::is_same_v<CharacterType, char> )
	{
		this->Append( BC_CONTAINER_NAME( TextViewBase )<char, true>{ c_string, ArraySize } );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @param character
	///	Character to find and count.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr size_t																		CountCharacters(
		CharacterType																					character,
		size_t																							start_position					= 0,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const noexcept
	{
		return ThisViewType<true>( *this ).CountCharacters( character, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Counts and returns the number of characters found on a range.
	///
	/// @param character
	///	Character to find and count.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr size_t																		CountCharacters(
		CharacterType																					character,
		ConstIterator																					start_position,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const noexcept
	{
		return ThisViewType<true>( *this ).CountCharacters( character, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrance of a character inside this text.
	///
	/// @param character
	///	Character to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator																FindCharacter(
		CharacterType																					character,
		size_t																							start_position					= 0,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const noexcept
	{
		return ThisViewType<true>( *this ).FindCharacter( character, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrance of a character inside this text.
	///
	/// @param character
	///	Character to find.
	///
	/// @param start_position
	///	Position of where to start searching from.
	///
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding a character, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	///
	/// @return
	/// Iterator to where matching character was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator																FindCharacter(
		CharacterType																					character,
		ConstIterator																					start_position,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const noexcept
	{
		return ThisViewType<true>( *this ).FindCharacter( character, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrance of a text inside this text view.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from, default 0.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding text, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator																Find(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>											text_to_find,
		size_t																							start_position					= 0,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const noexcept
	{
		return ThisViewType<true>( *this ).Find( text_to_find, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find the first occurrence of a text inside this text view.
	///
	/// @param text_to_find
	///	Source text to find.
	/// 
	/// @param start_position
	///	Position of where to start searching from.
	/// 
	/// @param search_length
	///	Window of search, up to which point is the text searched to when finding text, default is
	/// <tt>std::numeric_limits<size_t>::max()</tt>.
	/// 
	/// @return
	/// Iterator to where other text matching this was found, iterator points to the end if not found.
	[[nodiscard]] constexpr ConstIterator																Find(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>											text_to_find,
		ConstIterator																					start_position,
		size_t																							search_length					= std::numeric_limits<size_t>::max()
	) const BC_CONTAINER_NOEXCEPT
	{
		return ThisViewType<true>( *this ).Find( text_to_find, start_position, search_length );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																								Replace(
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>											text_to_replace,
		BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>											replace_with,
		size_t																							start_position					= 0,
		size_t																							count							= 1
	) BC_CONTAINER_NOEXCEPT
	{
		auto ReplaceRange = [this, replace_with]( size_t from, size_t replace_text_start, size_t amount )
		{
			for( size_t i = 0; i < amount; ++i ) {
				this->data_ptr[ from + i ] = replace_with[ replace_text_start + i ];
			}
		};

		if( text_to_replace.IsEmpty() ) return;

		auto search_position = start_position;
		for( size_t i = 0; i < count; ++i ) {
			auto it = this->Find( text_to_replace, search_position );
			if( it == this->end() ) {
				return;
			}
			size_t it_index = it.GetIndex();
			int64_t size_diff = int64_t( replace_with.Size() ) - int64_t( text_to_replace.Size() );
			if( size_diff < 0 ) {
				this->Erase( it, it + -size_diff );
				ReplaceRange( it_index, 0, replace_with.Size() );
			} else if( size_diff > 0 ) {
				this->Insert( it, replace_with.SubText( 0, size_diff ) );
				ReplaceRange( it_index + size_diff, size_diff, replace_with.Size() - size_diff );
			} else {
				ReplaceRange( it_index, 0, replace_with.Size() );
			}
			search_position = it_index + replace_with.Size();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get a sub text from this text.
	///
	/// @param start_position
	///	Start copying from this position.
	/// 
	/// @param size
	///	How many characters to copy, only copies up to the end of this string.
	/// 
	/// @return
	/// New text container instance.
	constexpr BC_CONTAINER_NAME( TextBase )																SubText(
		size_t																							start_position,
		size_t																							size							= std::numeric_limits<size_t>::max()
	) const BC_CONTAINER_NOEXCEPT
	{
		if( start_position >= this->data_size ) return {};
		auto length = std::min( this->data_size - start_position, size );
		BC_CONTAINER_NAME( TextBase ) ret;
		ret.Resize( length );
		for( size_t i = 0; i < length; ++i ) {
			ret[ i ] = this->data_ptr[ i + start_position ];
		}

		return ret;
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
	/// New text container instance.
	constexpr BC_CONTAINER_NAME( TextBase )																SubText(
		ConstIterator																					begin_it,
		ConstIterator																					end_it
	)  const BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( reinterpret_cast<void*>( begin_it.container ) == this, "Wrong start iterator used on TextBase::SubText" );
		BC_ContainerAssert( reinterpret_cast<void*>( end_it.container ) == this, "Wrong end iterator used on TextBase::SubText" );
		size_t begin_position = begin_it.GetIndex();
		size_t end_position = end_it.GetIndex();
		size_t length;
		if( end_position < begin_position ) {
			length = 0;
		} else {
			length = end_position - begin_position;
		}

		BC_CONTAINER_NAME( TextBase ) ret;
		ret.Resize( length );
		for( size_t i = 0; i < length; ++i ) {
			ret[ i ] = this->data_ptr[ i + begin_position ];
		}

		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Converts this text into a null terminated C-style character string.
	///
	///	This operation is fast as underlaying container always reserves extra space for the null terminator at the end, however only
	/// this function will add that null character at the end.
	///
	/// @warning
	/// Always use this function to get a C-style string as using Data() will not return a null terminated character string.
	///
	/// @return
	/// C-style, null-terminated character string with same character type as this text.
	constexpr const CharacterType																	*	ToCStr() BC_CONTAINER_NOEXCEPT
	{
		if( !this->data_ptr ) return nullptr;
		this->Reserve( this->data_size + 1 );
		this->data_ptr[ this->data_size ] = '\0';
		return this->data_ptr;
	}

	operator BC_CONTAINER_NAME( TextViewBase )<CharacterType, true>() const noexcept
	{
		return BC_CONTAINER_NAME( TextViewBase )<CharacterType, true> { this->data_ptr, this->data_size };
	}

	operator BC_CONTAINER_NAME( TextViewBase )<CharacterType, false>() noexcept
	{
		return BC_CONTAINER_NAME( TextViewBase )<CharacterType, false> { this->data_ptr, this->data_size };
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using BC_CONTAINER_NAME( Text				) = BC_CONTAINER_NAME( TextBase		)<char>;
using BC_CONTAINER_NAME( Text8				) = BC_CONTAINER_NAME( TextBase		)<char8_t>;
using BC_CONTAINER_NAME( Text16				) = BC_CONTAINER_NAME( TextBase		)<char16_t>;
using BC_CONTAINER_NAME( Text32				) = BC_CONTAINER_NAME( TextBase		)<char32_t>;
using BC_CONTAINER_NAME( TextView			) = BC_CONTAINER_NAME( TextViewBase	)<char, true>; 
using BC_CONTAINER_NAME( TextView8			) = BC_CONTAINER_NAME( TextViewBase	)<char8_t, true>;
using BC_CONTAINER_NAME( TextView16			) = BC_CONTAINER_NAME( TextViewBase	)<char16_t, true>;
using BC_CONTAINER_NAME( TextView32			) = BC_CONTAINER_NAME( TextViewBase	)<char32_t, true>;
using BC_CONTAINER_NAME( EditableTextView	) = BC_CONTAINER_NAME( TextViewBase	)<char, false>;
using BC_CONTAINER_NAME( EditableTextView8	) = BC_CONTAINER_NAME( TextViewBase	)<char8_t, false>;
using BC_CONTAINER_NAME( EditableTextView16	) = BC_CONTAINER_NAME( TextViewBase	)<char16_t, false>;
using BC_CONTAINER_NAME( EditableTextView32	) = BC_CONTAINER_NAME( TextViewBase	)<char32_t, false>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( Text ) ) == 24 );
static_assert( sizeof( BC_CONTAINER_NAME( Text8 ) ) == 24 );
static_assert( sizeof( BC_CONTAINER_NAME( Text16 ) ) == 24 );
static_assert( sizeof( BC_CONTAINER_NAME( Text32 ) ) == 24 );

static_assert( sizeof( BC_CONTAINER_NAME( TextView ) ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( TextView8 ) ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( TextView16 ) ) == 16 );
static_assert( sizeof( BC_CONTAINER_NAME( TextView32 ) ) == 16 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( container_bases::Container<BC_CONTAINER_NAME( Text )> );
static_assert( container_bases::Container<BC_CONTAINER_NAME( Text8 )> );
static_assert( container_bases::Container<BC_CONTAINER_NAME( Text16 )> );
static_assert( container_bases::Container<BC_CONTAINER_NAME( Text32 )> );

static_assert( container_bases::ContainerEditableView<BC_CONTAINER_NAME( EditableTextView )> );
static_assert( container_bases::ContainerEditableView<BC_CONTAINER_NAME( EditableTextView8 )> );
static_assert( container_bases::ContainerEditableView<BC_CONTAINER_NAME( EditableTextView16 )> );
static_assert( container_bases::ContainerEditableView<BC_CONTAINER_NAME( EditableTextView32 )> );

static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( TextView )> );
static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( TextView8 )> );
static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( TextView16 )> );
static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( TextView32 )> );

static_assert( container_bases::LinearContainer<BC_CONTAINER_NAME( Text )> );
static_assert( container_bases::LinearContainer<BC_CONTAINER_NAME( Text8 )> );
static_assert( container_bases::LinearContainer<BC_CONTAINER_NAME( Text16 )> );
static_assert( container_bases::LinearContainer<BC_CONTAINER_NAME( Text32 )> );

static_assert( container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( EditableTextView )> );
static_assert( container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( EditableTextView8 )> );
static_assert( container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( EditableTextView16 )> );
static_assert( container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( EditableTextView32 )> );

static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( TextView )> );
static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( TextView8 )> );
static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( TextView16 )> );
static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( TextView32 )> );

static_assert( container_bases::TextContainer<BC_CONTAINER_NAME( Text )> );
static_assert( container_bases::TextContainer<BC_CONTAINER_NAME( Text8 )> );
static_assert( container_bases::TextContainer<BC_CONTAINER_NAME( Text16 )> );
static_assert( container_bases::TextContainer<BC_CONTAINER_NAME( Text32 )> );

static_assert( container_bases::TextContainerEditableView<BC_CONTAINER_NAME( EditableTextView )> );
static_assert( container_bases::TextContainerEditableView<BC_CONTAINER_NAME( EditableTextView8 )> );
static_assert( container_bases::TextContainerEditableView<BC_CONTAINER_NAME( EditableTextView16 )> );
static_assert( container_bases::TextContainerEditableView<BC_CONTAINER_NAME( EditableTextView32 )> );

static_assert( container_bases::TextContainerView<BC_CONTAINER_NAME( TextView )> );
static_assert( container_bases::TextContainerView<BC_CONTAINER_NAME( TextView8 )> );
static_assert( container_bases::TextContainerView<BC_CONTAINER_NAME( TextView16 )> );
static_assert( container_bases::TextContainerView<BC_CONTAINER_NAME( TextView32 )> );


static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( Text )> );
static_assert( container_bases::ContainerEditableView<BC_CONTAINER_NAME( Text )> );
static_assert( container_bases::Container<BC_CONTAINER_NAME( Text )> );

static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( EditableTextView )> );
static_assert( container_bases::ContainerEditableView<BC_CONTAINER_NAME( EditableTextView )> );
static_assert( !container_bases::Container<BC_CONTAINER_NAME( EditableTextView )> );

static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( TextView )> );
static_assert( !container_bases::ContainerEditableView<BC_CONTAINER_NAME( TextView )> );
static_assert( !container_bases::Container<BC_CONTAINER_NAME( TextView )> );

static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( Text )> );
static_assert( container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( Text )> );
static_assert( container_bases::LinearContainer<BC_CONTAINER_NAME( Text )> );

static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( EditableTextView )> );
static_assert( container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( EditableTextView )> );
static_assert( !container_bases::LinearContainer<BC_CONTAINER_NAME( EditableTextView )> );

static_assert( container_bases::LinearContainerView<BC_CONTAINER_NAME( TextView )> );
static_assert( !container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( TextView )> );
static_assert( !container_bases::LinearContainer<BC_CONTAINER_NAME( TextView )> );

static_assert( container_bases::TextContainerView<BC_CONTAINER_NAME( Text )> );
static_assert( container_bases::TextContainerEditableView<BC_CONTAINER_NAME( Text )> );
static_assert( container_bases::TextContainer<BC_CONTAINER_NAME( Text )> );

static_assert( container_bases::TextContainerView<BC_CONTAINER_NAME( EditableTextView )> );
static_assert( container_bases::TextContainerEditableView<BC_CONTAINER_NAME( EditableTextView )> );
static_assert( !container_bases::TextContainer<BC_CONTAINER_NAME( EditableTextView )> );

static_assert( container_bases::TextContainerView<BC_CONTAINER_NAME( TextView )> );
static_assert( !container_bases::TextContainerEditableView<BC_CONTAINER_NAME( TextView )> );
static_assert( !container_bases::TextContainer<BC_CONTAINER_NAME( TextView )> );



} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
