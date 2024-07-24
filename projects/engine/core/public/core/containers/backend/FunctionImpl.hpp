
#include <core/containers/backend/ContainerBase.hpp>
#include <core/utility/template/CallableTraits.hpp>
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

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	union LocalStorage
	{
		::bc::internal_::InvokerBase<ReturnType, ParameterTypes...> 	*	invoker;
		u8																	storage[16];
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

	#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Function )() noexcept
	{
		memset( &storage, 0, sizeof( storage ) );
	}
	#else
	constexpr BC_CONTAINER_NAME( Function )() noexcept = default;
	#endif

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~BC_CONTAINER_NAME( Function )()
	{
		Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename FunctorType>
	BC_CONTAINER_NAME( Function )(
		FunctorType									&&	callable
	)
	{
		static_assert(
			std::is_copy_constructible_v<std::decay_t<FunctorType>>,
			"FunctorType must be copy constructible"
		);
		static_assert(
			std::is_constructible_v<std::decay_t<FunctorType>, FunctorType>,
			"Function must be constructible from the FunctorType"
		);

		using FunctorTraits = ::bc::utility::CallableTraits<std::remove_reference_t<std::remove_pointer_t<FunctorType>>>;
		if constexpr( FunctorTraits::IsPlainFunction() )
		{
			storage.invoker = new ::bc::internal_::FunctionInvoker<LocalStorage, ReturnType, ParameterTypes...>( callable );
		}
		else
		{
			storage.invoker = new ::bc::internal_::ObjectInvoker<LocalStorage, FunctorType, ReturnType, ParameterTypes...>( std::forward<FunctorType>( callable ) );
		}
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
	ReturnType operator()(
		ParameterTypes...												args
	) const
	{
		BC_ContainerAssert( !IsEmpty(), U"Cannot invoke empty function." );
		return static_cast<::bc::internal_::InvokerBase<ReturnType, ParameterTypes...>*>( storage.invoker )->Invoke( std::forward<ParameterTypes>( args )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																Clear() noexcept
	{
		if ( storage.invoker )
		{
			delete static_cast<::bc::internal_::InvokerBase<ReturnType, ParameterTypes...>*>( storage.invoker );
			storage.invoker = nullptr;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool																IsEmpty() const noexcept
	{
		return !storage.invoker;
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
		if( other.storage.invoker )
		{
			storage.invoker = static_cast<::bc::internal_::InvokerBase<ReturnType, ParameterTypes...>*>( other.storage.invoker )->Clone();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																MoveOther(
		BC_CONTAINER_NAME( Function )								&&	other
	) noexcept
	{
		std::swap( storage.invoker, other.storage.invoker );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	LocalStorage														storage = {};
	//union
	//{
	//	::bc::internal_::detail::InvokerBase<ReturnType, ParameterTypes...>* invoker;
	//};

	//::bc::internal_::detail::BC_CONTAINER_NAME( InvokerBase )<ReturnType, ParameterTypes...>*
	//																	invoker			= nullptr;
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
static_assert( sizeof( BC_CONTAINER_NAME( Function )<void()> ) == 16 );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



BC_CONTAINER_NAMESPACE_END;
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
