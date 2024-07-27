
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
	using MyInvokerBase = ::bc::internal_::container::InvokerBase<ReturnType, ParameterTypes...>;

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
		MyInvokerBase												*	heap_invoker;
		typename std::aligned_storage<sizeof( MyInvokerBase ), alignof( MyInvokerBase )>::type
																		local_invoker;
		u8																raw[24];
	};
	static_assert( sizeof( LocalStorage ) == 24 );

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
		const BC_CONTAINER_NAME( Function )							&	other
	)
	{
		#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
		memset( &storage, 0, sizeof( decltype( storage ) ) );
		#endif

		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BC_CONTAINER_NAME( Function )(
		BC_CONTAINER_NAME( Function )								&&	other
	) noexcept
	{
		#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
		memset( &storage, 0, sizeof( decltype( storage ) ) );
		#endif

		MoveOther( std::move( other ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename FunctorType>
	BC_CONTAINER_NAME( Function )(
		FunctorType													&&	callable
	) requires(
		utility::CallableWithReturnAndParameters<FunctorType, ReturnType, ParameterTypes...> &&
		!std::is_same_v<std::decay_t<FunctorType>, BC_CONTAINER_NAME(Function)>
	)
	{
		// TODO: Make sure that this function is not called for another Function object.
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

		StoreInvokeable( std::forward<FunctorType>( callable ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~BC_CONTAINER_NAME( Function )()
	{
		Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BC_CONTAINER_NAME( Function )									&	operator=(
		const BC_CONTAINER_NAME( Function )							&	other
	)
	{
		if( std::addressof( other ) != this )
		{
			Clear();
			CopyOther( other );
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
			MoveOther( std::move( other ) );
		}
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	BC_CONTAINER_NAME( Function )									&	operator=(
		FunctorType													&&	callable
	) requires(
		utility::CallableWithReturnAndParameters<FunctorType, ReturnType, ParameterTypes...> &&
		!std::is_same_v<std::decay_t<FunctorType>, BC_CONTAINER_NAME(Function)>
	)
	{
		// TODO: Make sure that this function is not called for another Function object.

		using FunctorRawType = std::remove_reference_t<std::remove_pointer_t<FunctorType>>;
		using FunctorTraits = ::bc::utility::CallableTraits<FunctorRawType>;

		static_assert(
			std::is_copy_constructible_v<std::decay_t<FunctorType>>,
			"FunctorType must be copy constructible"
		);
		static_assert(
			std::is_constructible_v<std::decay_t<FunctorType>, FunctorType>,
			"Function must be constructible from the FunctorType"
		);

		Clear();
		StoreInvokeable( std::forward<FunctorType>( callable ) );

		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ReturnType operator()(
		ParameterTypes...												args
	) const
	{
		BC_ContainerAssert( !IsEmpty(), U"Cannot invoke empty function." );
		if( this->type == Type::INVOKEABLE_OBJECT ) {
			auto invoker = GetInvokerPointer<MyInvokerBase>();
			return invoker->Invoke( std::forward<ParameterTypes>( args )... );
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
			auto invoker = GetInvokerPointer<MyInvokerBase>();
			::bc::internal_::container::DestructInvoker( invoker );
			DeallocateInvoker( invoker, this->is_stored_locally );
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
	void																CopyOther(
		const BC_CONTAINER_NAME( Function )							&	other
	)
	{
		assert( this->type == Type::NONE && "Function already initialized." );

		if( other.type == Type::NONE ) return;

		this->is_stored_locally = other.is_stored_locally;
		this->type = other.type;

		if( other.type == Type::INVOKEABLE_OBJECT )
		{
			auto other_invoker = other.GetInvokerPointer<MyInvokerBase>();
			auto other_memory_block_info = other_invoker->GetMemoryBlockInfo();
			AllocateAndSetInvokerPointer(
				other.is_stored_locally,
				other_memory_block_info.size,
				other_memory_block_info.alignment
			);
			other_invoker->CloneInto( GetInvokerPointer<MyInvokerBase>() );
		}
		else
		{
			this->storage = other.storage;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																MoveOther(
		BC_CONTAINER_NAME( Function )								&&	other
	) noexcept
	{
		assert( this->type == Type::NONE && "Function already initialized." );

		std::swap( this->is_stored_locally, other.is_stored_locally );
		std::swap( this->type, other.type );
		std::swap( this->storage, other.storage );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	void																StoreInvokeable(
		FunctorType													&&	callable
	)
	{
		assert( this->type == Type::NONE && "Function already initialized." );

		using FunctorRawType = std::remove_reference_t<std::remove_pointer_t<FunctorType>>;
		using FunctorTraits = ::bc::utility::CallableTraits<FunctorRawType>;
		constexpr bool is_plain_function = FunctorTraits::IsPlainFunction();

		if constexpr( is_plain_function )
		{
			this->is_stored_locally = true;
			this->type = Type::FUNCTION;

			storage.function_pointer = callable;
		}
		else
		{
			using InvokerType = ::bc::internal_::container::ObjectInvoker<LocalStorage, FunctorType, ReturnType, ParameterTypes...>;

			constexpr bool store_locally = ::bc::internal_::container::IsInvokerStoredLocally<InvokerType, FunctorRawType, LocalStorage>();
			this->is_stored_locally = store_locally;
			this->type = Type::INVOKEABLE_OBJECT;

			auto invoker = AllocateAndSetInvokerPointer<InvokerType, FunctorType>( store_locally );
			::bc::internal_::container::ConstructInvoker<InvokerType, FunctorType>(
				invoker,
				std::forward<FunctorType>( callable )
			);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MyInvokerBase													*	AllocateAndSetInvokerPointer(
		bool															store_locally,
		i64																size,
		i64																alignment
	)
	{
		if( store_locally ) return reinterpret_cast<MyInvokerBase*>( &storage.local_invoker );

		// TODO: Replace with our custom memory allocation.
		auto invoker = reinterpret_cast<MyInvokerBase*>( malloc( size ) );

		SetInvokerHeapPointer( invoker );
		return invoker;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<
		typename InvokerType,
		typename FunctorType
	>
	InvokerType														*	AllocateAndSetInvokerPointer(
		bool															store_locally
	)
	{
		auto invoker = static_cast<InvokerType*>(
			AllocateAndSetInvokerPointer(
				store_locally,
				sizeof( InvokerType ),
				alignof( InvokerType )
			)
		);

		// This test is needed because we will lose type information later.
		assert(
			invoker == static_cast<MyInvokerBase*>( invoker ) &&
			"static_cast must not alter invoker pointer, multiple inheritance is not allowed."
		);

		return invoker;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																DeallocateInvoker(
		MyInvokerBase												*	invoker,
		bool															store_locally
	)
	{
		if( store_locally ) return;
		// TODO: Replace with our custom memory allocation.
		free( invoker );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename InvokerType>
	void																SetInvokerHeapPointer(
		InvokerType													*	invoker
	)
	{
		assert( invoker != nullptr && "Invoker pointer must not be nullptr." );
		assert( this->is_stored_locally == false && "Invoker pointer must not be set when local storage is used." );

		this->storage.heap_invoker = invoker;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename InvokerType>
	InvokerType														*	GetInvokerPointer()
	{
		if( this->is_stored_locally ) return reinterpret_cast<InvokerType*>( &storage.local_invoker );
		return static_cast<InvokerType*>( storage.heap_invoker );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename InvokerType>
	const InvokerType												*	GetInvokerPointer() const
	{
		if( this->is_stored_locally ) return reinterpret_cast<const InvokerType*>( &storage.local_invoker );
		return static_cast<InvokerType*>( storage.heap_invoker );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool																is_stored_locally		= false;
	Type																type					= Type::NONE;
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
