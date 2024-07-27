#pragma once

// The purpose of this file is to add common parts of to both simple and normal versions of the Function.

#include <core/memory/MemoryBlockInfo.hpp>



namespace bc {
namespace internal_ {
namespace container {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <
	typename InvokerType,
	typename FunctorType
>
void																ConstructInvoker(
	InvokerType													*	invoker,
	FunctorType													&&	callable
)
{
	assert( invoker != nullptr && "Invoker pointer must not be null." );
	::new( invoker ) InvokerType( std::forward<FunctorType>( callable ) );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvokerType>
void																DestructInvoker(
	InvokerType													*	invoker
)
{
	assert( invoker != nullptr && "Invoker pointer must not be null." );
	invoker->~InvokerBase();
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename InvokerType,
	typename FunctorType,
	typename FunctionLocalStorageType
>
consteval bool IsInvokerStoredLocally()
{
	constexpr auto StorageSize = sizeof( FunctionLocalStorageType );
	constexpr auto StorageAlignment = alignof( FunctionLocalStorageType );
	return
		std::is_trivially_copyable_v<FunctorType> &&
		sizeof( InvokerType ) <= StorageSize &&
		alignof( InvokerType ) <= StorageAlignment &&
		( StorageAlignment % alignof( InvokerType ) == 0 );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Abstract base class for callable objects
template<
	typename ReturnType,
	typename ...ParameterTypes
>
class InvokerBase
{
public:
	virtual ~InvokerBase() = default;
	virtual memory::MemoryBlockInfo GetMemoryBlockInfo() const = 0;
	virtual ReturnType Invoke( ParameterTypes... args ) const = 0;
	virtual void CloneInto( InvokerBase * destination ) const = 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Template derived class for specific callable objects
template<
	typename FunctionLocalStorageType,
	typename FunctorType,
	typename ReturnType,
	typename ...ParameterTypes
>
class ObjectInvoker : public InvokerBase<ReturnType, ParameterTypes...>
{
	using MyInvokerBase = InvokerBase<ReturnType, ParameterTypes...>;

public:

	using Signature = ReturnType(ParameterTypes...);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorTypeIn>
	ObjectInvoker(
		FunctorTypeIn														&&	f
	) :
		functor( std::forward<FunctorTypeIn>( f ) )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ObjectInvoker(
		const ObjectInvoker													&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~ObjectInvoker() = default;

	virtual memory::MemoryBlockInfo GetMemoryBlockInfo() const override
	{
		return memory::MemoryBlockInfo{
			.size		= sizeof( decltype( *this ) ),
			.alignment	= alignof( decltype( *this ) )
		};
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ReturnType Invoke(
		ParameterTypes...														args
	) const override
	{
		return functor( std::forward<ParameterTypes>( args )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																		CloneInto(
		MyInvokerBase														*	destination
	) const override
	{
		ConstructInvoker( static_cast<ObjectInvoker*>( destination ), functor );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FunctorType																	functor;
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( InvokerBase<void> ) == 8 );
static_assert( sizeof( InvokerBase<void, int> ) == 8 );
static_assert( sizeof( InvokerBase<void, int, int> ) == 8 );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // namespace container
} // namespace internal_
} // namespace bc
