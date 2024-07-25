
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
	using MyInvokerBase = ::bc::internal_::InvokerBase<ReturnType, ParameterTypes...>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	enum class Type : u8
	{
		NONE		= 0,
		FUNCTION,
		INVOKEABLE_OBJECT,
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	union LocalStorage
	{
		MyInvokerBase												*	heap_alloc_invoker;
		u8																raw[16];
	};

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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Function )() noexcept
	{
		#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
		memset( &storage, 0, sizeof( decltype( storage ) ) );
		#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <utility::CallableWithReturnAndParameters<ReturnType, ParameterTypes...> FunctorType>
	BC_CONTAINER_NAME( Function )(
		FunctorType													&&	callable
	) requires( NOT Function TYPE )
	{
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

		InitInvoker( std::forward<FunctorType>( callable ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BC_CONTAINER_NAME( Function )(
		const BC_CONTAINER_NAME( Function )							&	other
	)
	{
		CopyOther( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BC_CONTAINER_NAME( Function )(
		BC_CONTAINER_NAME( Function )								&&	other
	) noexcept
	{
		MoveOther( std::move( other ) );
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
	template<utility::CallableWithReturnAndParameters<ReturnType, ParameterTypes...> FunctorType>
	BC_CONTAINER_NAME( Function )									&	operator=(
		FunctorType													&&	callable
	) requires( NOT Function TYPE )
	{
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
		InitInvoker( std::forward<FunctorType>( callable ) );

		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ReturnType operator()(
		ParameterTypes...												args
	) const
	{
		BC_ContainerAssert( !IsEmpty(), U"Cannot invoke empty function." );
		return GetInvokerPointer<MyInvokerBase>()->Invoke( std::forward<ParameterTypes>( args )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																Clear() noexcept
	{
		if ( this->type != Type::NONE )
		{
			DestructInvoker();
			DeallocateInvoker( this->is_stored_locally );
			this->is_stored_locally = false;
			this->type = Type::NONE;
		}
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
		if( other.type == Type::NONE ) return;

		this->is_stored_locally = other.is_stored_locally;
		this->type = other.type;

		// TODO: Clone invoker
		//if( other.is_stored_locally )
		//{
		//	this->storage = other.storage;
		//}
		//else
		//{
		//	this->storage.invoker = other.storage.invoker->Clone();
		//}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																MoveOther(
		BC_CONTAINER_NAME( Function )								&&	other
	) noexcept
	{
		std::swap( this->is_stored_locally, other.is_stored_locally );
		std::swap( this->type, other.type );
		std::swap( this->storage, other.storage );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	void																InitInvoker(
		FunctorType													&&	callable
	)
	{
		assert( this->type == Type::NONE && "Function already initialized." );

		using FunctorRawType = std::remove_reference_t<std::remove_pointer_t<FunctorType>>;
		using FunctorTraits = ::bc::utility::CallableTraits<FunctorRawType>;
		constexpr bool is_plain_function = FunctorTraits::IsPlainFunction();

		if constexpr( is_plain_function )
		{
			using InvokerType = ::bc::internal_::FunctionInvoker<LocalStorage, ReturnType, ParameterTypes...>;

			this->is_stored_locally = true;
			this->type = Type::FUNCTION;

			AllocateInvoker<InvokerType, FunctorType>( true );
			ConstructInvoker<InvokerType, FunctorType>( std::forward<FunctorType>( callable ), true );
		}
		else
		{
			using InvokerType = ::bc::internal_::ObjectInvoker<LocalStorage, FunctorType, ReturnType, ParameterTypes...>;


			constexpr auto StorageSize = sizeof( LocalStorage );
			constexpr auto StorageAlignment = alignof( LocalStorage );
			constexpr auto FunctorTypeSize = sizeof( FunctorRawType );
			constexpr auto FunctorTypeAlignment = alignof( FunctorRawType );

			auto trivially_copyable = std::is_trivially_copyable_v<FunctorRawType>;
			auto smaller_or_equal_to_storage = sizeof( FunctorRawType ) <= StorageSize;
			auto aligned = alignof( FunctorRawType ) <= StorageAlignment;
			auto same_alignment = ( StorageAlignment % alignof( FunctorRawType ) == 0 );



			constexpr bool store_locally = ::bc::internal_::IsFunctorStoredLocally<FunctorRawType, LocalStorage>();
			this->is_stored_locally = store_locally;
			this->type = Type::INVOKEABLE_OBJECT;

			AllocateInvoker<InvokerType, FunctorType>( store_locally );
			ConstructInvoker<InvokerType, FunctorType>( std::forward<FunctorType>( callable ), store_locally );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<
		typename InvokerType,
		typename FunctorType
	>
	void																AllocateInvoker(
		bool															store_locally
	)
	{
		if( store_locally ) return;
		// TODO: Replace with our custom memory allocation.
		auto invoker = malloc( sizeof( InvokerType ) );
		assert( invoker == static_cast<MyInvokerBase*>( invoker ) && "static_cast should not alter invoker pointer." );
		SetInvokerPointer( reinterpret_cast<InvokerType*>( invoker ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																DeallocateInvoker(
		bool															store_locally
	)
	{
		if( store_locally ) return;
		// TODO: Replace with our custom memory allocation.
		free( GetInvokerPointer<MyInvokerBase>() );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <
		typename InvokerType,
		typename FunctorType
	>
	void																ConstructInvoker(
		FunctorType													&&	callable,
		bool															store_locally
	)
	{
		auto * invoker = GetInvokerPointer<InvokerType>();
		::new( invoker ) InvokerType( std::forward<FunctorType>( callable ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																DestructInvoker()
	{
		GetInvokerPointer<MyInvokerBase>()->~InvokerBase();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename InvokerType>
	void																SetInvokerPointer(
		InvokerType													*	invoker
	)
	{
		assert( invoker != nullptr && "Invoker pointer must not be null." );
		assert( this->is_stored_locally == false && "Invoker pointer must not be set when local storage is used." );

		this->storage.heap_alloc_invoker = invoker;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename InvokerType>
	InvokerType														*	GetInvokerPointer()
	{
		if( this->is_stored_locally ) return reinterpret_cast<InvokerType*>( storage.raw );
		return static_cast<InvokerType*>( storage.heap_alloc_invoker );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename InvokerType>
	const InvokerType												*	GetInvokerPointer() const
	{
		if( this->is_stored_locally ) return reinterpret_cast<const InvokerType*>( storage.raw );
		return static_cast<InvokerType*>( storage.heap_alloc_invoker );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool																is_stored_locally		= false;
	Type																type					= Type::NONE;
	alignas( 8 ) LocalStorage											storage;
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
// Check if text containers fulfill size requirements.
// static_assert( sizeof( BC_CONTAINER_NAME( Function )<void()> ) == 32 );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



BC_CONTAINER_NAMESPACE_END;
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
