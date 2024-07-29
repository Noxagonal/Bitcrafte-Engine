
#include <core/containers/backend/ContainerBase.hpp>
#include <core/utility/template/CallableTraits.hpp>
#include <core/utility/concepts/CallableConcepts.hpp>
#include "FunctionImplShared.hpp"

#if BC_CONTAINER_IMPLEMENTATION_NORMAL
#include <core/diagnostic/assertion/Assert.hpp>

#elif BC_CONTAINER_IMPLEMENTATION_SIMPLE
#include <core/diagnostic/assertion/HardAssert.hpp>

#else
#error "Container implementation type not given"
#endif

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>



namespace bc {



BC_CONTAINER_NAMESPACE_START;



template<typename Signature>
class BC_CONTAINER_NAME( Function );

template <typename ReturnType, typename ...ParameterTypes>
class BC_CONTAINER_NAME( Function )<ReturnType( ParameterTypes... )>
{
	using MyFunction = ReturnType( ParameterTypes... );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Signature = ReturnType( ParameterTypes... );
	using Traits = ::bc::utility::CallableTraits<Signature>;

	//using ReturnType = typename Traits::ReturnType;
	//using CallParameterTypeList = typename Traits::ParameterTypeList;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= Signature;
	static constexpr bool IsDataConst		= false;

	template<typename OtherSignature>
	using ThisContainerType					= BC_CONTAINER_NAME( Function )<OtherSignature>;
	using ThisType							= ThisContainerType<Signature>;

	template<typename OtherSignature, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<typename OtherSignature>
	using ThisContainerFullType				= BC_CONTAINER_NAME( Function )<OtherSignature>;
	using ThisFullType						= ThisContainerFullType<Signature>;

	using value_type						= Signature;	// for stl compatibility.
	using result_type						= ReturnType;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class Type : u8
	{
		NONE		= 0,
		FUNCTION,
		INVOKEABLE_OBJECT,
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	union alignas( 8 ) LocalStorage
	{
		MyFunction													*	function_pointer;
		void														*	heap_functor;
		u8																raw[16];
	};
	static_assert( sizeof( LocalStorage ) == 16 );

	using MyFunctorManagerBase = ::bc::internal_::container::FunctorManagerBase<LocalStorage, ReturnType, ParameterTypes...>;
	using FunctorManagerStorage = std::aligned_storage_t<sizeof( MyFunctorManagerBase ), alignof( MyFunctorManagerBase )>;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Function )() noexcept
	{
		#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
		memset( &storage, 0, sizeof( decltype( storage ) ) );
		#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BC_CONTAINER_NAME( Function )(
		BC_CONTAINER_NAME( Function )								&	other
	)
	{
		#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
		memset( &storage, 0, sizeof( decltype( storage ) ) );
		#endif

		Copy( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BC_CONTAINER_NAME( Function )(
		BC_CONTAINER_NAME( Function )								&&	other
	) noexcept
	{
		#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
		memset( &storage, 0, sizeof( decltype( storage ) ) );
		#endif

		Swap( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename FunctorType>
	BC_CONTAINER_NAME( Function )(
		FunctorType													&&	functor
	) requires(
		utility::CallableWithReturnAndParameters<FunctorType, ReturnType, ParameterTypes...> &&
		!std::is_same_v<std::decay_t<FunctorType>, BC_CONTAINER_NAME(Function)>
	)
	{
		using FunctorBaseType = std::remove_reference_t<std::remove_pointer_t<std::decay_t<FunctorType>>>;
		using FunctorTraits = ::bc::utility::CallableTraits<FunctorBaseType>;

		static_assert( !std::is_pointer_v<FunctorBaseType>, "FunctorBaseType must not be a pointer." );
		static_assert( !std::is_reference_v<FunctorBaseType>, "FunctorBaseType must not be a reference." );

		static_assert(
			std::is_copy_constructible_v<std::decay_t<FunctorType>>,
			"FunctorType must be copy constructible"
		);
		static_assert(
			std::is_constructible_v<std::decay_t<FunctorType>, FunctorType>,
			"Function must be constructible from the FunctorType"
		);

		#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
		memset( &storage, 0, sizeof( decltype( storage ) ) );
		#endif

		Store( std::forward<FunctorType>( functor ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~BC_CONTAINER_NAME( Function )()
	{
		Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BC_CONTAINER_NAME( Function )									&	operator=(
		BC_CONTAINER_NAME( Function )								&	other
	)
	{
		if( std::addressof( other ) != this )
		{
			BC_CONTAINER_NAME( Function )( other ).Swap( *this );
		}
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BC_CONTAINER_NAME( Function )									&	operator=(
		BC_CONTAINER_NAME( Function )								&&	other
	) noexcept
	{
		if( std::addressof( other ) != this )
		{
			Clear();
			Swap( other );

			// Could also do this, but we'll save a swap by doing it directly as swapping cannot throw,
			// we don't need to worry about it. The compiler might be able to optimize this anyhow.
			//BC_CONTAINER_NAME( Function )( std::move( other ) ).Swap( *this );
		}
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	BC_CONTAINER_NAME( Function )									&	operator=(
		FunctorType													&&	functor
	) requires(
		utility::CallableWithReturnAndParameters<FunctorType, ReturnType, ParameterTypes...> &&
		!std::is_same_v<std::decay_t<FunctorType>, BC_CONTAINER_NAME(Function)>
	)
	{
		using FunctorBaseType = std::remove_reference_t<std::remove_pointer_t<std::decay_t<FunctorType>>>;
		using FunctorTraits = ::bc::utility::CallableTraits<FunctorBaseType>;

		static_assert( !std::is_pointer_v<FunctorBaseType>, "FunctorBaseType must not be a pointer." );
		static_assert( !std::is_reference_v<FunctorBaseType>, "FunctorBaseType must not be a reference." );

		static_assert(
			std::is_copy_constructible_v<std::decay_t<FunctorType>>,
			"Functor must be copy constructible"
		);
		static_assert(
			std::is_constructible_v<std::decay_t<FunctorType>, FunctorType>,
			"bc::Function must be constructible from the FunctorType"
		);

		Clear();
		Store( std::forward<FunctorType>( functor ) );

		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ReturnType operator()(
		ParameterTypes...												args
	)
	{
		BC_ContainerAssert( !IsEmpty(), U"Cannot invoke empty function." );

		if( this->type == Type::INVOKEABLE_OBJECT )
		{
			auto manager = reinterpret_cast<MyFunctorManagerBase*>( &this->functor_manager );
			return manager->Invoke( this->is_stored_locally, this->storage, std::forward<ParameterTypes>( args )... );
		}
		else
		{
			auto function_ptr = storage.function_pointer;
			return function_ptr( std::forward<ParameterTypes>( args )... );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																Clear() noexcept
	{
		if ( this->type == Type::INVOKEABLE_OBJECT )
		{
			auto manager = reinterpret_cast<MyFunctorManagerBase*>( &this->functor_manager );
			manager->DestructFunctor( this->is_stored_locally, this->storage );
			manager->~MyFunctorManagerBase();
			bc::internal_::container::FreeFunctor<LocalStorage>( this->is_stored_locally, this->storage );
		}
		this->is_stored_locally = false;
		this->type = Type::NONE;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool																IsStoredLocally() const noexcept
	{
		BC_ContainerAssert( !IsEmpty(), U"Cannot check empty function stack locality, results would be meaningless." );
		return this->is_stored_locally;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool																IsEmpty() const noexcept
	{
		return this->type == Type::NONE;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	explicit operator bool() const noexcept
	{
		return !IsEmpty();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																Copy(
		BC_CONTAINER_NAME( Function )								&	other
	)
	{
		assert( this->type == Type::NONE && "Function already initialized." );

		if( other.type == Type::NONE ) return;

		this->is_stored_locally = other.is_stored_locally;
		this->type = other.type;

		if( other.type == Type::INVOKEABLE_OBJECT )
		{
			auto my_manager = reinterpret_cast<MyFunctorManagerBase*>( &this->functor_manager );
			auto other_manager = reinterpret_cast<MyFunctorManagerBase*>( &other.functor_manager );
			other_manager->Clone( other.is_stored_locally, *my_manager, this->storage, other.storage );
		}
		else
		{
			this->storage.function_pointer = other.storage.function_pointer;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																Swap(
		BC_CONTAINER_NAME( Function )								&	other
	) noexcept
	{
		std::swap( this->is_stored_locally, other.is_stored_locally );
		std::swap( this->type, other.type );
		std::swap( this->functor_manager, other.functor_manager );
		std::swap( this->storage, other.storage );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	void																Store(
		FunctorType													&&	functor
	)
	{
		assert( this->type == Type::NONE && "Function already initialized." );

		using FunctorBaseType = std::remove_reference_t<std::remove_pointer_t<FunctorType>>;
		using FunctorTraits = ::bc::utility::CallableTraits<FunctorBaseType>;
		constexpr bool is_plain_function = FunctorTraits::IsPlainFunction();

		if constexpr( is_plain_function )
		{
			this->is_stored_locally = true;
			this->type = Type::FUNCTION;

			storage.function_pointer = functor;
		}
		else
		{
			using FunctorManagerType = ::bc::internal_::container::FunctorManager<FunctorBaseType, LocalStorage, ReturnType, ParameterTypes...>;
			static_assert( sizeof( FunctorManagerType ) == 8, "FunctorManager size is not 8 bytes." );
			static_assert( alignof( FunctorManagerType ) == alignof( MyFunctorManagerBase ), "FunctorManager alignment is not equal to MyFunctorManagerBase." );

			constexpr bool store_locally = ::bc::internal_::container::IsFunctorStoredLocally<FunctorBaseType, LocalStorage>();

			this->is_stored_locally = store_locally;
			this->type = Type::INVOKEABLE_OBJECT;

			auto functor_pointer = ::bc::internal_::container::AllocateFunctor<FunctorBaseType, LocalStorage>( store_locally, storage );
			::new( functor_pointer ) FunctorBaseType( std::forward<FunctorType>( functor ) );

			::new( &this->functor_manager ) FunctorManagerType();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool																is_stored_locally		= false;
	Type																type					= Type::NONE;
	FunctorManagerStorage												functor_manager;
	LocalStorage														storage;
};



// Deduction guides.
template <typename ReturnType, typename... ParameterTypes>
BC_CONTAINER_NAME( Function )(ReturnType (*)(ParameterTypes...))
	-> BC_CONTAINER_NAME( Function )<ReturnType(ParameterTypes...)>;

template<
	typename FunctorType,
	typename FunctionSignature = typename utility::CallableTraits<FunctorType>::Signature
>
BC_CONTAINER_NAME( Function )(FunctorType)
	-> BC_CONTAINER_NAME( Function )<FunctionSignature>;



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if function container fulfills size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( Function )<void()> ) == 32 );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



BC_CONTAINER_NAMESPACE_END;
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
