
#include <core/containers/backend/ContainerBase.hpp>

#if BI_CONTAINER_IMPLEMENTATION_NORMAL
#include <core/containers/backend/PairImplNormal.hpp>
#elif BI_CONTAINER_IMPLEMENTATION_SIMPLE
#include <core/containers/backend/PairImplSimple.hpp>
#else
#error "Container implementation type not given"
#endif

#include <algorithm>
#include <assert.h>

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>



// TODO: Make Map more data-oriented, make a continuous array of keys and values, Requires Deque.



namespace bc {



template<BC_CONTAINER_VALUE_TYPENAME KeyType, BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( Map );


namespace container_bases {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BC_CONTAINER_VALUE_TYPENAME KeyType, BC_CONTAINER_VALUE_TYPENAME ValueType>
struct BC_CONTAINER_NAME( MapNode )
{
	BC_CONTAINER_NAME( MapNode )																	*	parent;
	BC_CONTAINER_NAME( MapNode )																	*	left;
	BC_CONTAINER_NAME( MapNode )																	*	right;
	int64_t																								height;
	BC_CONTAINER_NAME( Pair )<KeyType, ValueType>														data;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<BC_CONTAINER_VALUE_TYPENAME KeyType, BC_CONTAINER_VALUE_TYPENAME ValueType, bool IsConst>
class BC_CONTAINER_NAME( MapIteratorBase )
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedKeyType					= KeyType;
	using ContainedValueType				= ValueType;
	using ContainedPairType					= BC_CONTAINER_NAME( Pair )<KeyType, ValueType>;
	using value_type						= ContainedPairType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<BC_CONTAINER_VALUE_TYPENAME OtherKeyType, BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( MapIteratorBase );

	friend class BC_CONTAINER_NAME( MapIteratorBase )<KeyType, ValueType, true>;
	friend class BC_CONTAINER_NAME( MapIteratorBase )<KeyType, ValueType, false>;

	using Container				= BC_CONTAINER_NAME( Map )<KeyType, ValueType>;
	using Node					= container_bases::BC_CONTAINER_NAME( MapNode )<KeyType, ValueType>;

public:

	constexpr BC_CONTAINER_NAME( MapIteratorBase )() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr BC_CONTAINER_NAME( MapIteratorBase )(
		const BC_CONTAINER_NAME( MapIteratorBase )<KeyType, ValueType, IsOtherConst>				&	other
	) noexcept requires( container_bases::IsConstConvertible<IsConst, IsOtherConst> ) :
		container( other.GetContainer() ),
		node( const_cast<Node*>( other.GetData() ) )
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( MapIteratorBase )(
		const Container																				*	container,
		const Node																					*	node
	) noexcept :
		container( container ),
		node( const_cast<Node*>( node ) )
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ContainedPairType																&	operator*() BC_CONTAINER_NOEXCEPT const
	{
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ContainedPairType																		&	operator*() BC_CONTAINER_NOEXCEPT requires( IsConst == false )
	{
		return *this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ContainedPairType																*	operator->() BC_CONTAINER_NOEXCEPT const
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ContainedPairType																		*	operator->() BC_CONTAINER_NOEXCEPT requires( IsConst == false )
	{
		return this->Get();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator==(
		BC_CONTAINER_NAME( MapIteratorBase )<KeyType, ValueType, IsOtherConst>							other
	) const noexcept
	{
		return this->node == other.GetData();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<bool IsOtherConst>
	constexpr bool																						operator!=(
		BC_CONTAINER_NAME( MapIteratorBase )<KeyType, ValueType, IsOtherConst>							other
	) const noexcept
	{
		return this->node != other.GetData();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( MapIteratorBase )													&	operator++() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->node,
			U"Tried to increment iterator past end",
			U"Container size", this->container->Size()
		);
		if( this->node->right ) {
			this->node = TraverseDownLeftmost( this->node->right );
		} else {
			if( this->node->parent ) {
				this->node = TraverseUpUntilComingFromLeft( this->node );
			} else {
				this->node = nullptr;
			}
		}
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( MapIteratorBase )													&	operator--() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->node,
			U"Tried to decrement iterator past begin",
			"Container size", this->container->Size()
		);
		if( this->node == nullptr ) {
			// End. Go to last actual.
			this->node = this->container->FindRightmostFrom( this->container->root_node );
		} else {
			if( this->node->left )
				this->node = TraverseDownRightmost( this->node->left );
			else {
				this->node = TraverseUpUntilComingFromRight( this->node );
			}
		}
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( MapIteratorBase )													&	operator+=(
		size_t																							value
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		for( size_t i = 0; i < value; ++i ) {
			++( *this );
		}
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( MapIteratorBase )													&	operator-=(
		size_t																							value
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		for( size_t i = 0; i < value; ++i ) {
			--( *this );
		}
		return *this;
	}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( MapIteratorBase )														operator+(
		size_t																							value
	) BC_CONTAINER_NOEXCEPT const
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_CONTAINER_NAME( MapIteratorBase ) ret = *this;
		ret += value;
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( MapIteratorBase )														operator-(
		size_t																							value
	) BC_CONTAINER_NOEXCEPT const
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_CONTAINER_NAME( MapIteratorBase ) ret = *this;
		ret -= value;
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const ContainedPairType																*	Get() BC_CONTAINER_NOEXCEPT const
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->node,
			U"Iterator out of range",
			"Container size", this->container->Size()
		);
		return std::addressof( this->node->data );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ContainedPairType																		*	Get() BC_CONTAINER_NOEXCEPT requires( IsConst == false )
	{
		BC_ContainerAssert( this->container, U"Tried using iterator that points to nothing" );
		BC_ContainerAssert( !this->container->IsEmpty(), U"Container is empty, cannot iterate over nothing" );
		BC_ContainerAssert( this->node,
			U"Iterator out of range",
			"Container size", this->container->Size()
		);
		return std::addressof( this->node->data );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const Node																			*	GetData() const noexcept
	{
		return this->node;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Node																					*	GetData() noexcept requires( IsConst == false )
	{
		return this->node;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr const Container																		*	GetContainer() const noexcept
	{
		return this->container;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Checks that the iterator points to valid data.
	/// 
	/// @return
	/// true if this iterator can be used to get a value, false if this iterator should not be used.
	constexpr bool																						IsValid() const noexcept
	{
		return this->container && this->node && !this->container->IsEmpty();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static constexpr Node																			*	TraverseDownLeftmost(
		Node																						*	from_node
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( from_node, U"Empty node" );
		while( from_node->left ) from_node = from_node->left;
		return from_node;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Node																					*	TraverseDownRightmost(
		Node																						*	from_node
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( from_node, U"Empty node" );
		while( from_node->right ) from_node = from_node->right;
		return from_node;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Node																					*	TraverseUpUntilComingFromRight(
		Node																						*	from_node
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( from_node, U"Empty node" );
		BC_ContainerAssert(
			( from_node->parent && from_node->parent->left == from_node ) || ( from_node->parent && from_node->parent->right == from_node ),
			U"Node parent does not recognize child."
		);
		while( from_node->parent && from_node->parent->left == from_node ) {
			from_node = from_node->parent;
		}
		return from_node->parent;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Node																					*	TraverseUpUntilComingFromLeft(
		Node																						*	from_node
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( from_node, U"Empty node" );
		BC_ContainerAssert(
			( from_node->parent && from_node->parent->left == from_node ) || ( from_node->parent && from_node->parent->right == from_node ),
			U"Node parent does not recognize child."
		);
		while( from_node->parent && from_node->parent->right == from_node ) {
			from_node = from_node->parent;
		}
		return from_node->parent;
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const Container																					*	container				= nullptr;
	Node																							*	node					= nullptr;
};



} // container_bases



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Map containing a key - value pair where the key is used to access elements.
///
/// @note
/// Keys must be unique.
///
/// @warning
/// Keys must not be modified manually or incorrect elements are returned. To modify a key, you must erase it first and then
/// insert a new one.
/// 
///	Map is very close to how std::map works and if you ever used it, this should be pretty familiar.
/// 
///	This map is build on an AVL binary search tree where the key is used to access stored elements. This means that all keys
/// must always be unique and elements are always ordered from smallest to largest. 
/// 
/// @tparam KeyType
///	Key type.
/// 
/// @tparam ValueType
///	value Type
template<BC_CONTAINER_VALUE_TYPENAME KeyType, BC_CONTAINER_VALUE_TYPENAME ValueType>
class BC_CONTAINER_NAME( Map ) :
	protected container_bases::ContainerResource
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedKeyType					= KeyType;
	using ContainedValueType				= ValueType;
	using ContainedPairType					= BC_CONTAINER_NAME( Pair )<KeyType, ValueType>;
	static constexpr bool IsDataConst		= false;

	template<BC_CONTAINER_VALUE_TYPENAME OtherKeyType, BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerType					= BC_CONTAINER_NAME( Map )<OtherKeyType, OtherValueType>;
	using ThisType							= ThisContainerType<KeyType, ValueType>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherKeyType, BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= ThisContainerViewType<KeyType, ValueType, IsOtherConst>;

	template<BC_CONTAINER_VALUE_TYPENAME OtherKeyType, BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( Map )<OtherKeyType, OtherValueType>;
	using ThisFullType						= ThisContainerFullType<KeyType, ValueType>;

	template<bool IsConst>
	using IteratorBase						= container_bases::BC_CONTAINER_NAME( MapIteratorBase )<KeyType, ValueType, IsConst>;
	using ConstIterator						= IteratorBase<true>;
	using Iterator							= IteratorBase<false>;

	using value_type						= ContainedPairType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<BC_CONTAINER_VALUE_TYPENAME OtherKeyType, BC_CONTAINER_VALUE_TYPENAME OtherValueType, bool IsOtherConst>
	friend class BC_CONTAINER_NAME( MapIteratorBase );

	template<BC_CONTAINER_VALUE_TYPENAME OtherKeyType, BC_CONTAINER_VALUE_TYPENAME OtherValueType>
	friend class BC_CONTAINER_NAME( Map );

	friend ConstIterator;
	friend Iterator;

protected:

	using Node					= container_bases::BC_CONTAINER_NAME( MapNode )<KeyType, ValueType>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	size_t						size				= 0;
	Node					*	root_node			= nullptr;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct FindNodeResult
	{
		Node				*	closest_node;	///< Closest node found, either the node itself or closest parent.
		bool					found;			///< true if exact match found, false if not in which case closest_node points to it's parent.
		bool					left_of_parent;	///< Place where the key would be inserted if node was not found.
	};

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Map )() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Map )(
		const std::initializer_list<ContainedPairType>												&	init_list
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> && std::is_copy_assignable_v<ValueType> )
	{
		this->Append( init_list );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Map )(
		const BC_CONTAINER_NAME( Map )																&	other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> )
	{
		this->Append( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Map )(
		BC_CONTAINER_NAME( Map )																	&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~BC_CONTAINER_NAME( Map )() BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Map )																&	operator=(
		const std::initializer_list<ContainedPairType>												&	other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> )
	{
		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Map )																&	operator=(
		const BC_CONTAINER_NAME( Map )																&	other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> )
	{
		if( &other == this ) return *this;

		this->Clear();
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Map )																&	operator=(
		BC_CONTAINER_NAME( Map )																	&&	other
	) noexcept
	{
		this->SwapOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Extends this map with elements from an initializer list.
	///
	/// @note
	/// If any key already exists in this map, then only the value is updated from the initializer list.
	/// 
	/// @param other
	///	Another map to add to this map.
	/// 
	/// @return
	/// Reference to this.
	constexpr BC_CONTAINER_NAME( Map )																&	operator+=(
		const std::initializer_list<ContainedPairType>												&	init_list
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> && std::is_copy_assignable_v<ValueType> )
	{
		this->Append( init_list );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if contents of this map matches the contents of another.
	/// 
	/// @param other
	///	Other map to compare contents with.
	/// 
	/// @return
	/// true if contents match, false if contents do not match.
	constexpr bool																						operator==(
		const BC_CONTAINER_NAME( Map )																&	other
	) const noexcept
	{
		if( other.root_node == this->root_node && other.Size() == this->Size() ) return true;

		return container_bases::CheckContainerContentsMatch( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if contents of this map differ the contents of another.
	/// 
	/// @param other
	///	Other map to compare contents with.
	/// 
	/// @return
	/// true if contents do not match, false if contents match.
	constexpr bool																						operator!=(
		const BC_CONTAINER_NAME( Map )																&	other
	) const noexcept
	{
		if( other.root_node == this->root_node && other.Size() == this->Size() ) return true;

		return container_bases::CheckContainerContentsDiffer( *this, other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Extends this map with another.
	///
	/// @note
	/// If any key already exists in this map, then only the value is updated from the other map.
	/// 
	/// @param other
	///	Another map to add to this map.
	/// 
	/// @return
	/// Reference to this.
	constexpr BC_CONTAINER_NAME( Map )																&	operator+=(
		const BC_CONTAINER_NAME( Map )																&	other
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> && std::is_copy_assignable_v<ValueType> )
	{
		this->Append( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Find or add an element by key name.
	///
	///	If key exists, get the value mapped to this key. If key does not exist, construct a new element with given key and return
	/// its value.
	/// 
	/// @param key
	///	Key of the element we wish to find or create.
	/// 
	/// @return
	/// Value paired with with given key.
	constexpr ValueType																				&	operator[](
		const KeyType																				&	key
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> && std::is_copy_assignable_v<ValueType> && std::is_default_constructible_v<ValueType> )
	{
		auto find_node_result = this->FindNode( key );
		if( find_node_result.found ) {
			return find_node_result.closest_node->data.second;
		} else {
			auto new_node = this->AllocateNode();
			this->AddNodeToTree( find_node_result, new_node );
			this->ConstructNode( new_node, key, {} );
			return new_node->data.second;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the smallest key/value pair.
	///
	/// @return
	/// Pair with the smallest key.
	constexpr const ContainedPairType																&	Front() BC_CONTAINER_NOEXCEPT const
	{
		BC_ContainerAssert( this->root_node, U"Cannot get container front, container is empty" );
		return this->FindLeftmostFrom( this->root_node )->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the smallest key/value pair.
	///
	/// @return
	/// Pair with the smallest key.
	constexpr ContainedPairType																		&	Front() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->root_node, U"Cannot get container front, container is empty" );
		return this->FindLeftmostFrom( this->root_node )->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the largest key/value pair.
	///
	/// @return
	/// Pair with the largest key.
	constexpr const ContainedPairType																&	Back() BC_CONTAINER_NOEXCEPT const
	{
		BC_ContainerAssert( this->root_node, U"Cannot get container back, container is empty" );
		return this->FindRightmostFrom( this->root_node )->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get the largest key/value pair.
	///
	/// @return
	/// Pair with the largest key.
	constexpr ContainedPairType																		&	Back() BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->root_node, U"Cannot get container back, container is empty" );
		return this->FindRightmostFrom( this->root_node )->data;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Finds an element with specific key.
	/// 
	/// @param key
	///	Key value of an element we want to find.
	/// 
	/// @return
	/// Iterator to the element if found, returns end iterator if not found.
	constexpr Iterator																					Find(
		const KeyType																				&	key
	) noexcept
	{
		auto result = this->FindNode( key );
		if( result.found ) return Iterator { this, result.closest_node };
		else return Iterator { this, nullptr };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Finds an element with specific key.
	/// 
	/// @param key
	///	Key value of an element we want to find.
	/// 
	/// @return
	/// ConstIterator to the element if found, returns end iterator if not found.
	constexpr ConstIterator																				Find(
		const KeyType																				&	key
	) const noexcept
	{
		auto result = this->FindNode( key );
		if( result.found ) return ConstIterator { this, result.closest_node };
		else return ConstIterator { this, nullptr };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check each member to see if any key match the parameter.
	///
	/// @note
	/// This is exactly the same as <tt>container.Find( <key> ) != container.end()</tt>
	/// 
	/// @param member
	///	Key value we're looking for.
	/// 
	/// @return
	/// True if this container has member which key equals what we're searching for, false if not found.
	constexpr bool																						HasMember(
		const KeyType																				&	key
	) const noexcept
	{
		return this->Find( key ) != this->end();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append elements from another container at the end of this.
	///
	///	Other container may be any iteratable Bitcrafte container.
	///
	/// @tparam OtherContainerType
	///	Type of another container which elements are added to the end of this list.
	///
	/// @param other
	///	Other list of which elements are appended to this.
	///
	/// @param count
	///	How many times the other elements are added to this list.
	template<container_bases::ContainerView OtherContainerType>
	constexpr void																						Append(
		const OtherContainerType																	&	other,
		size_t																							count					= 1
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> && std::is_same_v<ContainedPairType, typename OtherContainerType::ContainedPairType> )
	{
		if constexpr( std::is_same_v<OtherContainerType, ThisType> ||
			std::is_same_v<OtherContainerType, ThisViewType<true>> ||
			std::is_same_v<OtherContainerType, ThisViewType<false>> )
		{
			if( reinterpret_cast<const void*>( &other ) == reinterpret_cast<const void*>( this ) ||
				( other.root_node == this->root_node ) )
			{
				return;
			}
		}

		auto old_size				= this->Size();
		size_t other_size			= other.Size();
		size_t total_insert_size	= other_size * count;

		for( size_t c = 0; c < count; ++c )
		{
			auto other_it			= other.begin();
			for( size_t i = 0; i < other_size; ++i )
			{
				this->Insert( *other_it );
				++other_it;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Append elements from another container at the end of this.
	///
	///	Other container may be any iteratable Bitcrafte container.
	///
	/// @tparam OtherContainerType
	///	Type of another container which elements are added to the end of this list.
	///
	/// @param other
	///	Other list of which elements are appended to this.
	///
	/// @param count
	///	How many times the other elements are added to this list.
	constexpr void																						Append(
		const std::initializer_list<ContainedPairType>												&	init_list,
		size_t																							count					= 1
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> )
	{
		auto old_size				= this->Size();
		size_t other_size			= init_list.size();
		size_t total_insert_size	= other_size * count;

		for( size_t c = 0; c < count; ++c ) {
			auto other_it			= init_list.begin();
			for( size_t i = 0; i < other_size; ++i ) {
				this->Insert( *other_it );
				++other_it;
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Inserts a new key/value pair.
	///
	///	@note
	/// If key already exists, then the existing element value is only updated.
	/// 
	/// @param pair
	///	Pair to insert.
	/// 
	/// @return
	/// Iterator to inserted element.
	constexpr Iterator																					Insert(
		const ContainedPairType																		&	pair
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> && std::is_copy_assignable_v<ValueType> )
	{
		auto find_node_result = this->FindNode( pair.first );
		if( find_node_result.found ) {
			find_node_result.closest_node->data.second = pair.second;
			return Iterator { this, find_node_result.closest_node };
		} else {
			auto new_node = this->AllocateNode();
			this->AddNodeToTree( find_node_result, new_node );
			this->CopyConstructNode( new_node, pair );
			return Iterator { this, new_node };
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Inserts a new key/value pair.
	///
	///	@note
	/// If key already exists, then the existing element value is only updated.
	/// 
	/// @param pair
	///	Pair to insert.
	/// 
	/// @return
	/// Iterator to inserted element.
	constexpr Iterator																					Insert(
		ContainedPairType																			&&	pair
	) BC_CONTAINER_NOEXCEPT requires( std::is_move_constructible_v<ContainedPairType> && ( std::is_move_assignable_v<ValueType> || std::is_copy_assignable_v<ValueType> ) )
	{
		auto find_node_result = this->FindNode( pair.first );
		if( find_node_result.found ) {
			if constexpr( std::is_move_assignable_v<ValueType> ) {
				find_node_result.closest_node->data.second = std::move( pair.second );
			} else {
				find_node_result.closest_node->data.second = pair.second;
			}
			return Iterator { this, find_node_result.closest_node };
		} else {
			auto new_node = this->AllocateNode();
			this->AddNodeToTree( find_node_result, new_node );
			this->MoveConstructNode( new_node, std::move( pair ) );
			return Iterator { this, new_node };
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Emplace a key/value pair directly in place.
	///
	///	@note
	/// If key already exists, then the existing element value is only updated.
	/// 
	///	This is largely the same as insert except Pair data is constructed in place.
	/// 
	/// @param key
	///	Key used to find or create an element.
	/// 
	/// @param value
	///	Element value to set.
	/// 
	/// @return
	/// Iterator to created or updated element location.
	constexpr Iterator																					Emplace(
		const KeyType																				&	key,
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> && std::is_copy_assignable_v<ValueType> )
	{
		auto find_node_result = this->FindNode( key );
		if( find_node_result.found ) {
			find_node_result.closest_node->data.second = value;
			return Iterator { this, find_node_result.closest_node };
		} else {
			auto new_node = this->AllocateNode();
			this->AddNodeToTree( find_node_result, new_node );
			this->ConstructNode( new_node, key, value );
			return Iterator { this, new_node };
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Emplace a key/value pair directly in place.
	///
	///	@note
	/// If key already exists, then the existing element value is only updated.
	/// 
	///	This is largely the same as insert except key and value are outside of a Pair data and the Pair data is constructed in
	/// place.
	/// 
	/// @param key
	///	Key used to find or create an element.
	/// 
	/// @param value
	///	Element value to set.
	/// 
	/// @return
	/// Iterator to created or updated element location.
	constexpr Iterator																					Emplace(
		KeyType																						&&	key,
		ValueType																					&&	value
	) BC_CONTAINER_NOEXCEPT
	{
		auto find_node_result = this->FindNode( key );
		if( find_node_result.found ) {
			if constexpr( std::is_move_assignable_v<ValueType> ) {
				std::swap( find_node_result.closest_node->data.second, value );
			} else {
				find_node_result.closest_node->data.second = value;
			}
			return Iterator { this, find_node_result.closest_node };
		} else {
			auto new_node = this->AllocateNode();
			this->AddNodeToTree( find_node_result, new_node );
			if constexpr( std::is_move_constructible_v<KeyType> && std::is_move_constructible_v<ValueType> ) {
				this->ConstructNode( new_node, std::move( key ), std::move( value ) );
			} else if constexpr( std::is_move_constructible_v<KeyType> ) {
				this->ConstructNode( new_node, std::move( key ), value );
			} else if constexpr( std::is_move_constructible_v<ValueType> ) {
				this->ConstructNode( new_node, key, std::move( value ) );
			} else {
				this->ConstructNode( new_node, key, value );
			}
			return Iterator { this, new_node };
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase an element based on key.
	///
	/// @note
	/// If key does not exist, does nothing.
	/// 
	/// @param key
	///	Key of an element to erase.
	/// 
	/// @return
	/// Iterator to the next element which replaced this. Iterator end is returned if the erased element was the last one or key
	/// was not found.
	constexpr Iterator																					Erase(
		const KeyType																				&	key
	) BC_CONTAINER_NOEXCEPT
	{
		auto find_results = this->FindNode( key );
		if( find_results.found ) {
			auto node = find_results.closest_node;
			auto ret = Iterator { this, node };
			++ret;
			this->DestructNode( node );
			this->RemoveNodeFromTree( node );
			this->DeallocateNode( node );
			return ret;
		}
		return Iterator { this, nullptr };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase an element at iterator location.
	///
	/// @param at
	///	Iterator to element location.
	/// 
	/// @return
	/// Iterator to the next element which replaced this. Iterator end is returned if the erased element was the last one.
	constexpr Iterator																					Erase(
		ConstIterator																					at
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->size, U"Cannot erase from container, container is already empty" );
		BC_ContainerAssert( at.GetContainer() == this, U"Cannot erase from container using iterator that doesn't point to the container we're erasing from" );
		BC_ContainerAssert( at.GetData(), U"Cannot erase from container, iterator was at the end");
		auto node = const_cast<Node*>( at.GetData() );
		auto ret = Iterator { this, node };
		++ret;
		this->DestructNode( node );
		this->RemoveNodeFromTree( node );
		this->DeallocateNode( node );
		return ret;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Erase multiple elements at range.
	/// 
	/// @param from
	///	Iterator to first erased element location.
	/// 
	/// @param to
	///	Iterator to position up to which all elements are erased. This is the first element not erased.
	/// 
	/// @return
	/// Iterator position to the first element not erased.
	constexpr Iterator																					Erase(
		ConstIterator																					from,
		ConstIterator																					to
	) BC_CONTAINER_NOEXCEPT
	{
		BC_ContainerAssert( this->size, U"Cannot erase from container, container is already empty" );
		BC_ContainerAssert( from.GetContainer() == this, U"Cannot erase from container using iterator that doesn't point to the container we're erasing from" );
		BC_ContainerAssert( to.GetContainer() == this, U"Cannot erase from container using iterator that doesn't point to the container we're erasing from" );
		BC_ContainerAssert( from.GetData(), U"Cannot erase from container, iterator was at the end" );
		auto it = Iterator { this, from.GetData() };
		auto end = Iterator { this, to.GetData() };
		while( it != end ) {
			it = this->Erase( it );
		}
		return end;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Clear entire map of all contents.
	constexpr void																						Clear() BC_CONTAINER_NOEXCEPT
	{
		if( this->size ) {
			auto temp_node_list_size = this->size;
			Node ** temp_node_list = this->AllocateMemory<Node*>( temp_node_list_size );
			auto it = this->begin();
			for( size_t i = 0; i < this->size; ++i ) {
				temp_node_list[ i ] = it.GetData();
				++it;
			}
			for( size_t i = 0; i < this->size; ++i ) {
				Node * node = temp_node_list[ i ];
				this->DestructNode( node );
				this->DeallocateNode( node );
			}
			this->FreeMemory( temp_node_list, temp_node_list_size );
			this->root_node	= nullptr;
			this->size		= 0;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Get size of the map.
	/// 
	/// @return
	/// Current number of elements stored inside this map.
	constexpr size_t																					Size() const noexcept
	{
		return this->size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Check if this Map has no elements stored.
	/// 
	/// @return
	/// true if Size == 0, false otherwise.
	constexpr bool																						IsEmpty() const noexcept
	{
		return !this->size;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Iterator																					begin() noexcept
	{
		return Iterator { this, this->FindLeftmostFrom( this->root_node ) };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Iterator																					end() noexcept
	{
		return Iterator { this, nullptr };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ConstIterator																				begin() const noexcept
	{
		return ConstIterator { this, this->FindLeftmostFrom( this->root_node ) };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ConstIterator																				end() const noexcept
	{
		return ConstIterator { this, nullptr };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ConstIterator																				cbegin() const noexcept
	{
		return this->begin();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ConstIterator																				cend() const noexcept
	{
		return this->end();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						SwapOther(
		BC_CONTAINER_NAME( Map )																	&&	other
	) noexcept
	{
		std::swap( this->size, other.size );
		std::swap( this->root_node, other.root_node );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr FindNodeResult																			FindNode(
		const KeyType																				&	key
	) const noexcept
	{
		Node * node = this->root_node;
		while( node )
		{
			if( key == node->data.first )
			{
				return { node, true };
			}
			if( key < node->data.first )
			{
				if( node->left )
				{
					node = node->left;
				}
				else
				{
					return { node, false, true };
				}
			}
			else
			{
				if( node->right )
				{
					node = node->right;
				}
				else
				{
					return { node, false, false };
				}
			}
		}
		return { node };
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr static Node																			*	FindLeftmostFrom(
		Node																						*	node
	) noexcept
	{
		if( node == nullptr ) return nullptr;
		while( node->left )
		{
			node = node->left;
		}
		return node;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr static Node																			*	FindRightmostFrom(
		Node																						*	node
	) noexcept
	{
		if( node == nullptr ) return nullptr;
		while( node->right )
		{
			node = node->right;
		}
		return node;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr static Node																			*	FindSuccessor(
		Node																						*	node
	) noexcept
	{
		assert( node->right );
		return FindLeftmostFrom( node->right );
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						AddNodeToTree(
		FindNodeResult																					find_node_result,
		Node																						*	node
	) BC_CONTAINER_NOEXCEPT
	{
		assert( !find_node_result.found );
		if( find_node_result.closest_node ) {
			if( find_node_result.left_of_parent ) {
				find_node_result.closest_node->left = node;
				node->parent = find_node_result.closest_node;
			} else {
				find_node_result.closest_node->right = node;
				node->parent = find_node_result.closest_node;
			}
		} else {
			this->root_node = node;
		}

		++this->size;
		if( this->size > 2 ) this->BalanceTree( node );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						RemoveNodeFromTree(
		Node																						*	node
	) BC_CONTAINER_NOEXCEPT
	{
		auto node_parent = node->parent;
		auto recalculate_weight_from = node_parent;
		assert( !node_parent || ( node == node_parent->left || node == node_parent->right ) );
		if( !node->left && !node->right ) {
			// No children, just remove it from the tree.
			if( node_parent ) {
				if( node_parent->left == node ) node_parent->left = nullptr;
				else if( node_parent->right == node ) node_parent->right = nullptr;
			} else {
				// This was a root node, and there's nothing remaining after deleting this.
				this->root_node = nullptr;
			}
		}
		else if( ( !!node->left ) != ( !!node->right ) ) { // XOR
			// This node has one child which will take over this position.
			Node * child_node = ( !!node->left ) ? node->left : node->right;
			if( node_parent ) {
				if( node_parent->left == node ) {
					node_parent->left = child_node;
					child_node->parent = node_parent;
				}
				else if( node_parent->right == node ) {
					node_parent->right = child_node;
					child_node->parent = node_parent;
				}
			} else {
				// This was root node, replace root by child.
				this->root_node			= child_node;
				this->root_node->parent	= nullptr;
			}
		} else {
			// This node has 2 children, we need to find a successor.
			auto successor = this->FindSuccessor( node );
			auto successor_parent = successor->parent;

			assert( successor );
			assert( successor->parent );
			assert( successor->left == nullptr );

			auto successor_left = successor->left;
			auto successor_right = successor->right;
			auto node_left = node->left;
			auto node_right = node->right;

			bool replace_root = node == this->root_node;
			bool successor_is_child = successor->parent == node;
			bool replace_node_parent_left_side = ( node_parent ) ? ( node_parent->left == node ) : false;

			if( successor_is_child ) {
				recalculate_weight_from = successor;

				if( node_parent ) {
					if( replace_node_parent_left_side ) node_parent->left = successor;
					else node_parent->right = successor;
				}
				successor->parent = node_parent;

				successor->left = node_left;
				node_left->parent = successor;
			} else {
				recalculate_weight_from = successor_parent;

				// Link successor's right child to successor parent's left child.
				successor_parent->left = nullptr;
				if( successor_right ) {
					successor_parent->left = successor_right;
					successor_right->parent = successor_parent;
				}
				// Replace current node with successor node.
				if( node_parent ) {
					if( replace_node_parent_left_side ) node_parent->left = successor;
					else node_parent->right = successor;
				}
				successor->parent = node_parent;

				successor->left = node_left;
				node_left->parent = successor;

				successor->right = node_right;
				node_right->parent = successor;
			}
			if( replace_root ) {
				this->root_node = successor;
			}
		}

		--this->size;
		if( this->size > 2 ) this->BalanceTree( recalculate_weight_from );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						ConstructNode(
		Node																						*	node,
		const KeyType																				&	key,
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<KeyType> && std::is_copy_constructible_v<ValueType> )
	{
		this->ConstructRange( &node->data, 1, key, value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						ConstructNode(
		Node																						*	node,
		const KeyType																				&	key,
		ValueType																					&&	value
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<KeyType> || std::is_move_constructible_v<ValueType> )
	{
		this->ConstructRange( &node->data, 1, key, std::move( value ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						ConstructNode(
		Node																						*	node,
		KeyType																						&&	key,
		const ValueType																				&	value
	) BC_CONTAINER_NOEXCEPT requires( std::is_move_constructible_v<KeyType> && std::is_copy_constructible_v<ValueType> )
	{
		this->ConstructRange( &node->data, 1, std::move( key ), value );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						ConstructNode(
		Node																						*	node,
		KeyType																						&&	key,
		ValueType																					&&	value
	) BC_CONTAINER_NOEXCEPT requires( std::is_move_constructible_v<KeyType> && std::is_move_constructible_v<ValueType> )
	{
		this->ConstructRange( &node->data, 1, std::move( key ), std::move( value ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						CopyConstructNode(
		Node																						*	node,
		const ContainedPairType																		&	pair
	) BC_CONTAINER_NOEXCEPT requires( std::is_copy_constructible_v<ContainedPairType> )
	{
		this->CopyConstructRange( &node->data, &pair, 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						MoveConstructNode(
		Node																						*	node,
		ContainedPairType																			&&	pair
	) noexcept requires( std::is_move_constructible_v<ContainedPairType> )
	{
		this->MoveConstructRange( &node->data, &pair, 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						DestructNode(
		Node																						*	node
	) BC_CONTAINER_NOEXCEPT
	{
		this->DestructRange( &node->data, 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Node																					*	AllocateNode() BC_CONTAINER_NOEXCEPT
	{
		auto new_node = this->AllocateMemory<Node>( 1 );
		new_node->parent		= nullptr;
		new_node->left			= nullptr;
		new_node->right			= nullptr;
		new_node->height		= 0;
		return new_node;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						DeallocateNode(
		Node																						*	node
	) BC_CONTAINER_NOEXCEPT
	{
		this->FreeMemory( node, 1 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						BalanceTree(
		Node																						*	starting_from
	) noexcept
	{
		assert( starting_from );
		Node * node = starting_from;
		while( node ) {
			bool was_root = node == this->root_node;
			this->RecalculateHeight( node );
			int64_t balance = this->GetNodeBalance( node );

			if( balance > +1 ) {
				if( this->GetNodeBalance( node->left ) < 0 ) {
					node->left = this->LeftRotate( node->left );
				}
				node = this->RightRotate( node );
			} else if( balance < -1 ) {
				if( this->GetNodeBalance( node->right ) > 0 ) {
					node->right = this->RightRotate( node->right );
				}
				node = this->LeftRotate( node );
			}
			if( was_root ) {
				this->root_node = node;
				this->root_node->parent = nullptr;
			}
			assert( std::abs( this->GetNodeBalance( node ) ) < 2 );
			node = node->parent;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						RecalculateHeight(
		Node																						*	node
	) noexcept
	{
		int64_t left_height = node->left ? ( node->left->height ) : int64_t( 0 );
		int64_t right_height = node->right ? ( node->right->height ) : int64_t( 0 );
		node->height = std::max( left_height, right_height ) + 1;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr int64_t																					GetNodeBalance(
		Node																						*	node
	) const noexcept
	{
		int64_t left_height = node->left ? ( node->left->height ) : int64_t( 0 );
		int64_t right_height = node->right ? ( node->right->height ) : int64_t( 0 );
		return left_height - right_height;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Node																					*	RightRotate(
		Node																						*	old_root
	) noexcept
	{
		assert( old_root );
		Node * old_root_parent = old_root->parent;
		bool old_root_was_left_side = false;
		if( old_root_parent ) {
			old_root_was_left_side = old_root_parent->left == old_root;
		}

		Node * new_root = old_root->left;
		assert( new_root );
		Node * new_root_other_side = new_root->right;

		new_root->right = old_root;
		old_root->left = new_root_other_side;
		if( old_root->left ) {
			old_root->left->parent = old_root;
		}
		new_root->parent = old_root->parent;
		old_root->parent = new_root;

		if( old_root_parent ) {
			if( old_root_was_left_side ) {
				old_root_parent->left = new_root;
			} else {
				old_root_parent->right = new_root;
			}
		}

		old_root->height	= std::max( old_root->left ? old_root->left->height : 0, old_root->right ? old_root->right->height : 0 ) + 1;
		new_root->height	= std::max( new_root->left ? new_root->left->height : 0, new_root->right ? new_root->right->height : 0 ) + 1;

		assert( new_root->parent != old_root->parent );
		assert( new_root->right == old_root || new_root->left == old_root && "Parent does not recognize new child node." );

		return new_root;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr Node																					*	LeftRotate(
		Node																						*	old_root
	) noexcept
	{
		assert( old_root );
		Node * old_root_parent = old_root->parent;
		bool old_root_was_left_side = false;
		if( old_root_parent ) {
			old_root_was_left_side = old_root_parent->left == old_root;
		}

		Node * new_root = old_root->right;
		assert( new_root );
		Node * new_root_other_side = new_root->left;

		new_root->left = old_root;
		old_root->right = new_root_other_side;
		if( old_root->right ) {
			old_root->right->parent = old_root;
		}
		new_root->parent = old_root->parent;
		old_root->parent = new_root;

		if( old_root_parent ) {
			if( old_root_was_left_side ) {
				old_root_parent->left = new_root;
			} else {
				old_root_parent->right = new_root;
			}
		}

		old_root->height	= std::max( old_root->left ? old_root->left->height : 0, old_root->right ? old_root->right->height : 0 ) + 1;
		new_root->height	= std::max( new_root->left ? new_root->left->height : 0, new_root->right ? new_root->right->height : 0 ) + 1;

		return new_root;
	}

	#ifdef BITCRAFTE_ENABLE_MAP_DEBUG
public:
	void ScanInvalidPointers( Node * node = nullptr, Node * previous = nullptr ) const
	{
		try {
			if( node == nullptr ) node = this->root_node;
			if( node->left ) {
				if( node->left == reinterpret_cast<Node*>( 0xdddddddddddddddd ) ) throw previous;
				if( node->left == reinterpret_cast<Node*>( 0xcccccccccccccccc ) ) throw previous;
				if( node->left == reinterpret_cast<Node*>( 0xcdcdcdcdcdcdcdcd ) ) throw previous;
				ScanInvalidPointers( node->left, node );
			}
			if( node->right ) {
				if( node->right == reinterpret_cast<Node*>( 0xdddddddddddddddd ) ) throw previous;
				if( node->right == reinterpret_cast<Node*>( 0xcccccccccccccccc ) ) throw previous;
				if( node->right == reinterpret_cast<Node*>( 0xcdcdcdcdcdcdcdcd ) ) throw previous;
				ScanInvalidPointers( node->right, node );
			}
		} catch( Node * threw_node ) {
			assert( 0 && "Node pointer invalid" );
		}
	}
	int64_t CheckMaxImbalance( Node * node = nullptr ) const
	{
		int64_t result = 0;
		int64_t left_height = 0;
		int64_t right_height = 0;
		if( node == nullptr ) node = this->root_node;
		if( node->left ) {
			result = std::max( result, CheckMaxImbalance( node->left ) );
		}
		if( node->right ) {
			result = std::max( result, CheckMaxImbalance( node->right ) );
		}
		result = std::max( result, std::abs( GetNodeBalance( node ) ) );
		return result;
	}

	void DebugPrintBinaryTree() const
	{
		Node * node = this->root_node;
		std::cout << "MAP BINARY TREE DEBUG PRINT:\n<root>: ";
		RecursivePrint( node );
		std::cout << "\nMAP BINARY TREE DEBUG PRINT END\n";
	}

private:
	void RecursivePrint( Node * node, size_t inlining = 0 ) const
	{
		auto AddSpaces = [inlining]()
		{
			for( size_t i = 0; i < inlining; ++i ) {
				std::cout << "  ";
			}
		};
		if( node ) {
			std::cout << "[" << GetNodeBalance( node ) << "]: " << node->data.first << "\n";
			AddSpaces(); std::cout << "  Left: "; RecursivePrint( node->left, inlining + 1 );
			AddSpaces(); std::cout << "  Right: "; RecursivePrint( node->right, inlining + 1 );
		} else {
			std::cout << "NULL\n";
		}
	}
	#endif
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if map container fulfills size requirements.
static_assert( sizeof( container_bases::BC_CONTAINER_NAME( MapIteratorBase )<uint32_t, uint32_t, true> ) == 16 );
static_assert( sizeof( container_bases::BC_CONTAINER_NAME( MapIteratorBase )<uint32_t, uint32_t, false> ) == 16 );

static_assert( sizeof( BC_CONTAINER_NAME( Map )<uint32_t, uint32_t> ) == 16 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if map container fulfills concept requirements.
static_assert( container_bases::ContainerView<BC_CONTAINER_NAME( Map )<uint32_t, uint32_t>> );
static_assert( container_bases::ContainerEditableView<BC_CONTAINER_NAME( Map )<uint32_t, uint32_t>> );
static_assert( container_bases::Container<BC_CONTAINER_NAME( Map )<uint32_t, uint32_t>> );
static_assert( !container_bases::LinearContainerView<BC_CONTAINER_NAME( Map )<uint32_t, uint32_t>> );
static_assert( !container_bases::LinearContainerEditableView<BC_CONTAINER_NAME( Map )<uint32_t, uint32_t>> );
static_assert( !container_bases::LinearContainer<BC_CONTAINER_NAME( Map )<uint32_t, uint32_t>> );











#ifdef BITCRAFTE_ENABLE_MAP_DEBUG
inline void RunTests_Containers_Map()
{
	{
	   // DEBUGGING ONLY!!!
		std::map<int, int> m;
		m.insert( std::pair( 0, 0 ) );
		m.insert( std::pair( 1, 0 ) );
		m.insert( std::pair( 2, 0 ) );
		m.insert( std::pair( 3, 0 ) );
		m.insert( std::pair( 4, 0 ) );
		m.insert( std::pair( 5, 0 ) );
		m.insert( std::pair( 6, 0 ) );

		bc::Map<int, int> map;
		map.Insert( bc::Pair( 0, 0 ) );
		map.Insert( bc::Pair( 1, 0 ) );
		map.Insert( bc::Pair( 2, 0 ) );
		map.Insert( bc::Pair( 3, 0 ) );
		map.Insert( bc::Pair( 4, 0 ) );
		map.Insert( bc::Pair( 5, 0 ) );
		map.Insert( bc::Pair( 6, 0 ) );
		map.DebugPrintBinaryTree();
		map.Erase( 1 );
		map.Erase( 5 );
		map.Erase( 3 );
		map.DebugPrintBinaryTree();
		assert( map.CheckMaxImbalance() < 2 );
	}
	int test_count = 10000;
	{
		bc::Map<int, int> map;

		// acending insert
		for( int i = 0; i < test_count; ++i ) {
			map.Insert( bc::Pair( i, 0 ) );
		}

		// Iterate forward
		{
			auto it = map.begin();
			for( int i = 0; i < test_count; ++i ) {
				assert( it->first == i );
				++it;
			}
		}
		// Iterate backward
		{
			auto it = map.end();
			for( int i = 0; i < test_count; ++i ) {
				--it;
				assert( it->first == ( test_count - 1 ) - i );
			}
		}
		assert( map.CheckMaxImbalance() < 2 );
	}
	{
		bc::Map<int, int> map;

		// decending insert
		for( int i = 0; i < test_count; ++i ) {
			map.Insert( bc::Pair( ( test_count - 1 ) - i, 0 ) );
		}

		// Iterate forward
		{
			auto it = map.begin();
			for( int i = 0; i < test_count; ++i ) {
				assert( it->first == i );
				++it;
			}
		}
		// Iterate backward
		{
			auto it = map.end();
			for( int i = 0; i < test_count; ++i ) {
				--it;
				assert( it->first == ( test_count - 1 ) - i );
			}
		}
		assert( map.CheckMaxImbalance() < 2 );
	}
	{
		// Random insert
		bc::Map<int, int> map;
		std::random_device rd;
		std::default_random_engine gen( rd() );
		std::uniform_int_distribution<int> distribution( 0, test_count - 1 );
		for( int i = 0; i < test_count; ++i ) {
			auto dist = distribution( gen );
			map.Insert( bc::Pair( dist, 0 ) );
		}
		// map.DebugPrintBinaryTree();

		// Iterate forward
		{
			auto it = map.begin();
			int previous = -1;
			while( it != map.end() ) {
				int current = it->first;
				assert( current >= previous );
				previous = current;
				++it;
			}
		}
		// Iterate backward
		{
			auto it = map.end();
			int previous = INT32_MAX;
			while( it != map.begin() ) {
				--it;
				int current = it->first;
				assert( current <= previous );
				previous = current;
			}
		}
		assert( map.CheckMaxImbalance() < 2 );
	}
	// Test erase
	{
		bc::Map<int, int> map;
		std::vector<int> baseline( test_count );

		std::random_device rd;
		std::default_random_engine gen( rd() );
		std::uniform_int_distribution<int> distribution( 0, test_count - 1 );

		// decending insert
		for( int i = 0; i < test_count; ++i ) {
			map.Insert( bc::Pair( i, 0 ) );
			baseline[ i ] = i;
		}

		for( int i = 0; i < ( test_count / 4 ); ++i ) {
			auto dist = distribution( gen );
			map.Erase( dist );
			baseline[ dist ] = -1;
		}
		{
			for( int i = 0; i < test_count; ++i ) {
				if( baseline[ i ] < 0 ) {
					// Should not exist
					assert( map.Find( i ) == map.end() );
				} else {
					// Should exist
					assert( map.Find( i ) != map.end() );
				}
			}
		}
		assert( map.CheckMaxImbalance() < 2 );
	}

	// Test random insert and erase while iterating.
	{
		bc::Map<int, int> map;
		std::map<int, int> baseline;

		std::random_device rd;
		std::default_random_engine gen( rd() );
		std::uniform_int_distribution<int> selection( 0, 3 );
		std::uniform_int_distribution<int> distribution( 0, test_count - 1 );

		for( int i = 0; i < test_count; ++i ) {
			auto dist = distribution( gen );
			auto erase_this = selection( gen ) == 0;
			if( erase_this ) {
				map.Erase( dist );
				baseline.erase( dist );
			} else {
				map.Insert( bc::Pair( dist, 0 ) );
				baseline.insert( std::pair( dist, 0 ) );
			}
		}
		assert( baseline.size() == map.Size() );
		assert( map.CheckMaxImbalance() < 2 );
		{
			auto it = map.begin();
			auto b_it = baseline.begin();
			while( b_it != baseline.end() && it != map.end() ) {
				assert( it->first == b_it->first );
				++it;
				++b_it;
			}
		}
	}
}

#endif






}



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
