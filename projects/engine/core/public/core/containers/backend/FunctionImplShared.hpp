#pragma once

// The purpose of this file is to add common parts of to both simple and normal versions of the Function.



namespace bc {
namespace internal_ {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename FunctorType,
	typename FunctionLocalStorageType
>
consteval bool IsFunctorStoredLocally()
{
	constexpr auto StorageSize = sizeof( FunctionLocalStorageType );
	constexpr auto StorageAlignment = alignof( FunctionLocalStorageType );
	return
		std::is_trivially_copyable_v<FunctorType> &&
		sizeof( FunctorType ) <= StorageSize &&
		alignof( FunctorType ) <= StorageAlignment &&
		( StorageAlignment % alignof( FunctorType ) == 0 );
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
	virtual ReturnType Invoke( ParameterTypes... args ) const = 0;
	virtual InvokerBase * Clone() const = 0;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Template derived class for specific callable objects
template<
	typename FunctionLocalStorageType,
	typename ReturnType,
	typename ...ParameterTypes
>
class FunctionInvoker : public InvokerBase<ReturnType, ParameterTypes...>
{
public:

	using Signature = ReturnType(ParameterTypes...);

	static constexpr bool StoredLocally = true;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FunctionInvoker(
		Signature															*	f
	) :
		functor( f )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~FunctionInvoker() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ReturnType Invoke(
		ParameterTypes...														args
	) const override
	{
		return functor( std::forward<ParameterTypes>( args )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	InvokerBase<ReturnType, ParameterTypes...>			*	Clone() const override
	{
		auto new_invoker = new FunctionInvoker( functor );
		return static_cast<InvokerBase<ReturnType, ParameterTypes...>*>( new_invoker );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Signature																*	functor;
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
public:

	using Signature = ReturnType(ParameterTypes...);

	static constexpr bool StoredLocally = IsFunctorStoredLocally<FunctorType, FunctionLocalStorageType>();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ObjectInvoker(
		const FunctorType													&	f
	) :
		functor( f )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~ObjectInvoker() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ReturnType Invoke(
		ParameterTypes...														args
	) const override
	{
		return functor( std::forward<ParameterTypes>( args )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	InvokerBase<ReturnType, ParameterTypes...>			*	Clone() const override
	{
		auto new_invoker = new ObjectInvoker<FunctionLocalStorageType, FunctorType, ReturnType, ParameterTypes...>( functor );
		return static_cast<InvokerBase<ReturnType, ParameterTypes...>*>( new_invoker );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const FunctorType															functor;
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



} // namespace internal_
} // namespace bc
