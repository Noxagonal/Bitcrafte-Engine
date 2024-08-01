#pragma once

#include "List.hpp"
#include "Text.hpp"
#include <core/diagnostic/assertion/Assert.hpp>

/*
* TODO: Everything here
* 

namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Path;



namespace container_bases {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<bool IsConst>
class PathIteratorBase
{
	friend class Path;

public:

	template<bool IsOtherConst>
	using ContainerIteratorType		= PathIteratorBase<IsOtherConst>;
	using ThisIteratorType			= IteratorBaseType<IsConst>;

	using ContainerType			= Path<CharacterT, RootType, BackstepType, SeparatorCharacters...>;
	using ValueType				= TextBase<CharacterT>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PathIteratorBase( const PathIteratorBase & other ) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr PathIteratorBase(
		const IteratorBaseType<IsOtherConst>			&	other
	) requires( utility::IsConstConvertible<IsConst, IsOtherConst> )
		:
		container( const_cast<ContainerType*>( other.container ) ),
		data( const_cast<ValueType*>( other.data ) )
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr PathIteratorBase(
		const ContainerType										*	container,
		const TextBase<CharacterT>								*	data
	) :
		container( const_cast<ContainerType*>( container ) ),
		data( const_cast<ValueType*>( data ) )
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType									&	operator*() const
	{
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType											&	operator*() requires( IsConst == false )
	{
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType									*	operator->() const
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType											*	operator->() requires( IsConst == false )
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool													operator==(
		const IteratorBaseType<IsOtherConst>			&	other
	) const
	{
		return reinterpret_cast<void*>( this->container ) == reinterpret_cast<void*>( other.container ) && this->data == other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool													operator!=(
		const IteratorBaseType<IsOtherConst>			&	other
	) const
	{
		return reinterpret_cast<void*>( this->container ) != reinterpret_cast<void*>( other.container ) || this->data != other.data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr IteratorSelfType									&	operator++()
	{
		BAssert( this->container, U"Tried using iterator that points to nothing" );
		BAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BAssert( this->data + 1 <= this->container->Data() + this->container->Size(),
			diagnostic::MakePrintRecord_AssertText( U"Tried to increment iterator past end",
				U"Container size", this->container->Size(),
				U"Iterator index", this->GetIndex_NoCheck() + 1
			)
		);
		++this->data;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr IteratorSelfType									&	operator--()
	{
		BAssert( this->container, U"Tried using iterator that points to nothing" );
		BAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BAssert( this->data - 1 >= this->container->Data(),
			diagnostic::MakePrintRecord_AssertText( U"Tried to decrement iterator past begin",
				U"Container size", this->container->Size(),
				U"Iterator index", this->GetIndex_NoCheck() - 1
			)
		);
		--this->data;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr IteratorSelfType										operator+(
		u64															amount
	) const
	{
		BAssert( this->container, U"Tried using iterator that points to nothing" );
		BAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BAssert( this->data + amount <= this->container->Data() + this->container->Size(),
			diagnostic::MakePrintRecord_AssertText( U"Tried to increment iterator past end",
				U"Container size", this->container->Size(),
				U"Iterator index", this->GetIndex_NoCheck() + amount
			)
		);
		IteratorSelfType ret = *this;
		ret.data += amount;
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr IteratorSelfType										operator-(
		u64															amount
	) const
	{
		BAssert( this->container, U"Tried using iterator that points to nothing" );
		BAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BAssert( this->data - amount >= this->container->Data(),
			diagnostic::MakePrintRecord_AssertText( U"Tried to decrement iterator past begin",
				U"Container size", this->container->Size(),
				U"Iterator index", this->GetIndex_NoCheck() - amount
			)
		);
		IteratorSelfType ret = *this;
		ret.data -= amount;
		return ret;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr IteratorSelfType									&	operator+=(
		u64															amount
	)
	{
		BAssert( this->container, U"Tried using iterator that points to nothing" );
		BAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BAssert( this->data + amount <= this->container->Data() + this->container->Size(),
			diagnostic::MakePrintRecord_AssertText( U"Tried to increment iterator past end",
				U"Container size", this->container->Size(),
				U"Iterator index", this->GetIndex_NoCheck() + amount
			)
		);
		this->data += amount;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr IteratorSelfType									&	operator-=(
		u64															amount
	)
	{
		BAssert( this->container, U"Tried using iterator that points to nothing" );
		BAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BAssert( this->data - amount >= this->container->Data(),
			diagnostic::MakePrintRecord_AssertText( U"Tried to decrement iterator past begin",
				U"Container size", this->container->Size(),
				U"Iterator index", this->GetIndex_NoCheck() - amount
			)
		);
		this->data -= amount;
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ValueType											*	Get() requires( IsConst == false )
	{
		BAssert( this->container, "Tried using iterator that points to nothing" );
		BAssert( this->data >= this->container->Data() && this->data < ( this->container->Data() + this->container->Size() ),
			diagnostic::MakePrintRecord_AssertText( U"Iterator is out of range",
				U"Container size", this->container->Size(),
				U"Iterator index", this->GetIndex_NoCheck()
			)
		);
		return data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ValueType									*	Get() const
	{
		BAssert( this->container, "Tried using iterator that points to nothing" );
		BAssert( this->data >= this->container->Data() && this->data < ( this->container->Data() + this->container->Size() ),
			diagnostic::MakePrintRecord_AssertText( U"Iterator is out of range",
				U"Container size", this->container->Size(),
				U"Iterator index", this->GetIndex_NoCheck()
			)
		);
		return data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr i64													GetIndex() const
	{
		BAssert( this->container, "Tried using iterator that points to nothing" );
		BAssert( this->data >= this->container->Data() && this->data < ( this->container->Data() + this->container->Size() ),
			diagnostic::MakePrintRecord_AssertText( U"Iterator is out of range",
				U"Container size", this->container->Size(),
				U"Iterator index", this->GetIndex_NoCheck()
			)
		);
		return GetIndex_NoCheck();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr i64												GetIndex_NoCheck() const
	{
		return ( i64( this->data ) - i64( this->container->Data() ) ) / sizeof( ValueType );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ContainerType			*	container;
	ValueType				*	data;
};



} // container_bases



// TODO: Path object should be simplified a lot.
// - Remove from containers, this is more of an utility than a container.



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Provides text manipulation aimed to help with paths.
class Path
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Path() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Path(
		const Path												&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Path(
		Path													&&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Path(
		TextViewBase<char32_t, true>								from_text
	)
	{
		SetFromText( from_text );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 ArraySize>
	constexpr Path(
		const char32_t( &c_string )[ ArraySize ]
	)
	{
		SetFromText( TextViewBase<CharacterT, true>( c_string, ArraySize ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const TextBase<char32_t>							&	operator[](
		u64															section_index
	) const
	{
		BAssert( section_index < Size(),
			diagnostic::MakePrintRecord_AssertText( U"Index out of range",
				U"Container size", Size(),
				U"Iterator Index", section_index
			)
		);
		return GetSection( section_index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextBase<char32_t>								&	operator[](
		u64															section_index
	)
	{
		BAssert( section_index < Size(),
			diagnostic::MakePrintRecord_AssertText( U"Index out of range",
				U"Container size", Size(),
				U"Iterator Index", section_index
			)
		);
		return GetSection( section_index );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Path												&	operator=(
		const Path												&	new_path
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Path												&	operator=(
		Path													&&	new_path
		) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Path												&	operator=(
		TextViewBase<char32_t, true>								new_path
		)
	{
		SetFromText( new_path );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 ArraySize>
	constexpr Path												&	operator=(
		const char32_t( &new_path )[ ArraySize ]
		)
	{
		SetFromText( TextViewBase<CharacterT, true>( new_path, ArraySize ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Path												&	operator/=(
		TextViewBase<char32_t, true>								section_name
		)
	{
		PushBack( section_name );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Path													operator/(
		TextViewBase<char32_t, true>								section_name
		) const
	{
		Path ret = *this;
		ret /= section_name;
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 ArraySize>
	constexpr bool													operator==(
		const char32_t( &c_string )[ ArraySize ]
		) const
	{
		auto as_path = Path { TextViewBase<CharacterT, true>{ c_string, ArraySize } };
		return container_bases::CheckContainerContentsMatch( *this, as_path );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													operator==(
		TextViewBase<char32_t, true>								other
		) const
	{
		auto as_path = Path { other };
		return container_bases::CheckContainerContentsMatch( *this, as_path );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													operator==(
		const Path												&	other
		) const
	{
		if( &other == this ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<u64 ArraySize>
	constexpr bool													operator!=(
		const char32_t( &c_string )[ ArraySize ]
		) const
	{
		auto as_path = Path { TextViewBase<char32_t, true>{ c_string, ArraySize } };
		return container_bases::CheckContainerContentsDiffer( *this, as_path );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													operator!=(
		TextViewBase<char32_t, true>								other
		) const
	{
		auto as_path = Path { other };
		return container_bases::CheckContainerContentsDiffer( *this, as_path );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													operator!=(
		const Path												&	other
		) const
	{
		if( &other == this ) return false;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void													PushFront(
		TextViewBase<char32_t, true>								section_name
	)
	{
		sections.PushFront( section_name );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void													PushBack(
		TextViewBase<char32_t, true>								section_name
	)
	{
		sections.PushBack( section_name );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void													Clear()
	{
		sections.Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													SetFromText(
		TextViewBase<char32_t, true>								new_path
	)
	{
		return ParseFromText( new_path );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void													Erase(
		u64															section_index
	)
	{
		auto it = sections.begin() + section_index;
		if( it != sections.end() )
		{
			sections.Erase( it );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void													Erase(
		TextViewBase<char32_t, true>								section_name
	)
	{
		auto it = sections.begin();
		while( it != sections.end() )
		{
			if( *it == section_name )
			{
				sections.Erase( it );
				return;
			}
			++it;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void													Replace(
		u64															section_index,
		TextViewBase<char32_t, true>								new_name
	)
	{
		sections[ section_index ] = new_name;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void													Replace(
		TextViewBase<char32_t, true>								section_name,
		TextViewBase<char32_t, true>								new_name
	)
	{
		auto it = sections.begin();
		while( it != sections.end() )
		{
			if( *it == section_name )
			{
				*it = new_name;
				return;
			}
			++it;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get first section.
	///
	/// @note
	/// A section is a single name in a path, eg. a folder or file name.
	/// 
	/// @return
	/// Text view containing the first section text.
	constexpr const TextBase<char32_t>							&	Front() const
	{
		BAssert( !sections.IsEmpty(), U"Cannot get container front element, container is empty." );
		return sections[ 0 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get first section.
	///
	/// @note
	/// A section is a single name in a path, eg. a folder or file name.
	/// 
	/// @return
	/// New text containing the first section text.
	constexpr TextBase<char32_t>								&	Front()
	{
		BAssert( !sections.IsEmpty(), U"Cannot get container front element, container is empty." );
		return sections[ 0 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get last section.
	///
	/// @note
	/// A section is a single name in a path, eg. a folder or file name.
	/// 
	/// @return
	/// Text view containing the last section text.
	constexpr const TextBase<char32_t>							&	Back() const
	{
		BAssert( !sections.IsEmpty(), U"Cannot get container back element, container is empty." );
		return sections[ sections.Size() - 1 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get last section.
	///
	/// @note
	/// A section is a single name in a path, eg. a folder or file name.
	/// 
	/// @return
	/// New text containing the last section text.
	constexpr TextBase<char32_t>								&	Back()
	{
		BAssert( !sections.IsEmpty(), U"Cannot get container back element, container is empty." );
		return sections[ sections.Size() - 1 ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get common parent between this and another path object.
	///
	/// @param with
	///	Search a common parent 'with' another.
	/// 
	/// @return
	/// New path containing the common parent.
	constexpr Path													GetCommonParent(
		const Path												&	with
	) const
	{
		u64 max_sections = std::min( sections.Size(), with.sections.Size() );
		Path ret;
		ret.sections.Reserve( max_sections );
		for( u64 i = 0; i < max_sections; ++i )
		{
			if( sections[ i ] == with.sections[ i ] )
			{
				ret.sections.PushBack( sections[ i ] );
			}
			else
			{
				break;
			}
		}
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get relative path to another.
	///
	///	For example:
	/// 
	///		Path p1 = "f1/f2/f3";
	///		Path p2 = "f1/f2/f4";
	///		Path p3 = p1.GetRelativePath( p2 );
	///		// p3 will now contain "../f4"
	/// 
	/// @param to
	///	Path 'to' reach another.
	/// 
	/// @return
	/// New relative path.
	constexpr Path													GetRelativePath(
		const Path												&	to
	) const
	{
		static_assert( BackstepType < container_bases::PathBackstepType::COUNT, "Please add test for each BackstepType type" );

		auto common_parent = GetCommonParent( to );
		u64 backstep_count = Size() - common_parent.Size();
		Path ret;
		for( u64 i = 0; i < backstep_count; ++i )
		{
			if constexpr( BackstepType == container_bases::PathBackstepType::DOUBLE_DOTS )
			{
				ret.sections.EmplaceBack( ".." );
			}
			else if constexpr( BackstepType == container_bases::PathBackstepType::LESS_THAN )
			{
				ret.sections.EmplaceBack( "<" );
			}
		}
		for( u64 i = common_parent.Size(); i < to.Size(); ++i )
		{
			ret.PushBack( to[ i ] );
		}
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get a single section from the path.
	///
	/// @note
	/// A section is a single name in a path, eg. a folder or file name.
	/// 
	/// @param section_index
	///	Index count to section to return.
	/// 
	/// @return
	/// TextView object containing the name of the section.
	constexpr const TextBase<char32_t>							&	GetSection(
		u64															section_index
	) const
	{
		BAssert( section_index < Size(),
			diagnostic::MakePrintRecord_AssertText( U"Index out of range",
				U"Container size", Size(),
				U"Iterator Index", section_index
			)
		);
		return sections[ section_index ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get a single section from the path.
	///
	/// @note
	/// A section is a single name in a path, eg. a folder or file name.
	/// 
	/// @param section_index
	///	Index count to section to return.
	/// 
	/// @return
	/// New text object containing the name of the section.
	constexpr TextBase<char32_t>								&	GetSection(
		u64															section_index
	)
	{
		BAssert( section_index < Size(),
			diagnostic::MakePrintRecord_AssertText( U"Index out of range",
				U"Container size", Size(),
				U"Iterator Index", section_index
			)
		);
		return sections[ section_index ];
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get file name without path nor extension.
	/// 
	///	For example: <tt>"relative/path/image.png"</tt> will return <tt>"image"</tt>.
	/// 
	/// @return
	/// Text view to the last section text before the dot.
	constexpr TextViewBase<char32_t, true>							GetStem() const
	{
		if( IsEmpty() ) return {};

		auto & last = Back();
		if( last.IsEmpty() ) return {};

		auto it = last.begin() + 1;
		while( it != last.end() )
		{
			if( *it == '.' )
			{
				return TextViewBase<char32_t, true>( last.Data(), it.GetIndex() );
			}
			++it;
		}
		return last;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get file name, including extension.
	///
	///	For example: <tt>"relative/path/image.png"</tt> will return <tt>"image.png"</tt>.
	/// 
	/// @return
	/// Text view to the last section text before the dot.
	constexpr const TextBase<char32_t>							&	GetFileName() const
	{
		return Back();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get extension from a file.
	///
	///	For example: <tt>"relative/path/image.png"</tt> will return <tt>".png"</tt>.
	/// 
	/// @return
	/// Text view to the last section text from the last dot to the end of the file name.
	constexpr TextViewBase<char32_t, true>							GetExtension() const
	{
		if( IsEmpty() ) return {};

		auto & last = Back();
		if( last.IsEmpty() ) return {};

		auto it = last.begin() + 1;
		while( it != last.end() )
		{
			if( *it == '.' )
			{
				return TextViewBase<char32_t, true>( last.Data() + it.GetIndex(), last.Size() - it.GetIndex() );
			}
			++it;
		}
		return {};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Replaces stem name, file name exluding the extension.
	/// 
	///	For example:
	///
	///		Path p = "relative/path/image.png";
	///		p.ReplaceStem( "picture" );
	///		// p is now "relative/path/picture.png"
	///
	/// @param new_stem_name
	///	New name.
	constexpr void													ReplaceStem(
		TextViewBase<char32_t, true>								new_stem_name
	)
	{
		auto extension = GetExtension();
		auto & last = Back();
		last = new_stem_name;
		last.Append( extension );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Replaces file name including the extension.
	///
	///	For example:
	/// 
	///		Path p = "relative/path/image.png";
	///		p.ReplaceFileName( "picture.tga" );
	///		// p is now "relative/path/picture.tga"
	///
	/// @param new_file_name
	///	New name.
	constexpr void													ReplaceFileName(
		TextViewBase<char32_t, true>								new_file_name
	)
	{
		auto & last = Back();
		last = new_file_name;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Replace file extension name.
	/// 
	///	For example:
	/// 
	///		Path p = "relative/path/image.png";
	///		p.ReplaceExtension( "tga" );
	///		// p is now "relative/path/image.tga"
	///
	/// @param new_extension_name
	///	New name.
	constexpr void													ReplaceExtension(
		TextViewBase<char32_t, true>								new_extension_name
	)
	{
		auto stem = GetStem();
		auto & last = Back();
		last = stem;

		u64 append_extension_from_position;
		for( append_extension_from_position = 0; append_extension_from_position < new_extension_name.Size(); ++append_extension_from_position )
		{
			if( new_extension_name[ append_extension_from_position ] != '.' ) break;
		}
		u64 extension_append_size = append_extension_from_position - new_extension_name.Size();

		last.Append( TextViewBase<char32_t, true>( &new_extension_name[ append_extension_from_position ], extension_append_size ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Remove stem name, file name without extension.
	/// 
	///	For example:
	/// 
	///		Path p = "relative/path/image.png";
	///		p.RemoveStem();
	///		// p is now "relative/path/.png"
	///		p.RemoveStem();
	///		// p is now "relative/path"
	constexpr void													RemoveStem()
	{
		auto extension = GetExtension();
		auto & last = Back();
		last = extension;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Remove file name including the extension.
	///
	///	For example:
	/// 
	///				Path p = "relative/path/image.png";
	///				p.RemoveFileName();
	///				// p is now "relative/path"
	constexpr void													RemoveFileName()
	{
		sections.PopBack();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Remove file extension.
	///
	///	For example:
	/// 
	///				Path p = "relative/path/image.png";
	///				p.RemoveExtension();
	///				// p is now "relative/path/image"
	constexpr void													RemoveExtension()
	{
		auto stem = GetStem();
		auto & last = Back();
		last = stem;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Gets the number of sections.
	///
	/// @note
	/// A section is a single name in a path, eg. a folder or file name.
	/// 
	/// @return
	/// Number of sections.
	constexpr u64													Size() const
	{
		return sections.Size();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const TextBase<char32_t>							*	Data() const
	{
		return sections.Data();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr TextBase<char32_t>								*	Data()
	{
		return sections.Data();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													IsEmpty() const
	{
		return !Size();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if this path is relative or absolute.
	///
	///	Relative path starts with a regular folder or file name. Eg. <tt>"folder/sub"</tt> is relative while <tt>/folder/sub</tt>
	/// and <tt>C:/folder/sub</tt> are absolute.
	/// 
	/// @return
	/// true if relative, false if absolute.
	constexpr bool													IsRelative() const
	{
		if( IsEmpty() ) return true;
		if constexpr( RootType == container_bases::PathRootType::UNIX )
		{
			return !sections[ 0 ].IsEmpty();
		}
		else if constexpr( RootType == container_bases::PathRootType::MICROSOFT_WINDOWS )
		{
			if( sections[ 0 ].Size() > 1 )
			{
				return sections[ 0 ][ 1 ] != ':';
			}
			return true;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the path as text container.
	/// 
	/// @return
	/// Path as text.
	constexpr TextBase<char32_t>									ToText() const
	{
		if( sections.IsEmpty() ) return {};
		TextBase<char32_t>ret;
		for( u64 i = 0; i < sections.Size() - 1; ++i )
		{
			ret.Append( sections[ i ] );
			ret.PushBack( GetPrimarySeparatorCharacter() );
		}
		ret.Append( sections.Back() );
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr Iterator												begin()
	{
		return Iterator { this, this->sections.Data() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ConstIterator											begin() const
	{
		return ConstIterator { this, this->sections.Data() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ConstIterator											cbegin() const
	{
		return this->begin();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr Iterator												end()
	{
		return Iterator { this, this->sections.Data() + this->sections.Size() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ConstIterator											end() const
	{
		return ConstIterator { this, this->sections.Data() + this->sections.Size() };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	[[nodiscard]]
	constexpr ConstIterator											cend() const
	{
		return this->end();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													ParseFromText(
		TextViewBase<char32_t, true>								from_text
	)
	{
		sections.Clear();
		if( from_text.IsEmpty() ) return true;

		sections.EmplaceBack();
		auto * current_section = &sections.Back();
		bool previous_character_was_separator = false;

		for( u64 i = 0; i < from_text.Size(); ++i )
		{
			if( IsSeparatorCharacter( from_text[ i ] ) )
			{
				if( !previous_character_was_separator )
				{
					sections.EmplaceBack();
					current_section = &sections.Back();
				}
				previous_character_was_separator = true;
			}
			else
			{
				current_section->PushBack( from_text[ i ] );
				previous_character_was_separator = false;
			}
		}
		if( sections.Back().IsEmpty() ) sections.PopBack();
		ResolveBacksteps();

		return true;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void													ResolveBacksteps()
	{
		i64 current = 0;
		while( current < i64( sections.Size() ) )
		{
			if( IsBackstepSection( current ) )
			{
				if( current > 0 && !IsBackstepSection( current - 1 ) )
				{
					sections.Erase( sections.begin() + current );
					sections.Erase( sections.begin() + current - 1 );
					current -= 2;
				}
			}
			++current;
		}
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													IsBackstepSection(
		TextViewBase<char32_t, true>								section_text
	) const
	{
		if( section_text.Size() == 2 )
		{
			if( section_text[ 0 ] == char32_t( '.' ) && section_text[ 1 ] == char32_t( '.' ) )
			{
				return true;
			}
		}
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													IsBackstepSection(
		u64															index
	) const
	{
		return IsBackstepSection( GetSection( index ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool													IsSeparatorCharacter(
		char32_t													character
	) const
	{
		return character == U'/' || character == U'\\';
	}

	List<TextBase<char32_t>>										sections;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if list container fulfills size requirements.
static_assert( sizeof( Path ) == 24 );



} // bc

*/
