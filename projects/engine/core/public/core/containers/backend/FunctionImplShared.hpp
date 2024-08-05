#pragma once

// The purpose of this file is to add common parts of to both simple and normal versions of the Function.

#include <core/memory/MemoryBlockInfo.hpp>
#include <string.h>
#include <core/memory/raw/RawMemory.hpp>



namespace bc {
namespace internal_ {
namespace container {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename FunctorType,
	typename FunctionLocalStorageType
>
FunctorType														*	AllocateFunctor(
	bool															is_stored_locally,
	FunctionLocalStorageType									&	storage
) noexcept
{
	if( is_stored_locally )
	{
		assert( sizeof( FunctorType ) <= sizeof( FunctionLocalStorageType ) && "Functor type is too big." );
		assert( alignof( FunctorType ) <= alignof( FunctionLocalStorageType ) && "Functor alignment does not fit in storage." );
		return reinterpret_cast<FunctorType*>( storage.raw );
	}
	else
	{
		storage.heap_functor = memory::AllocateMemory<FunctorType>( 1, alignof( FunctorType ) );
		return static_cast<FunctorType*>( storage.heap_functor );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename FunctorType,
	typename FunctionLocalStorageType
>
void																FreeFunctor(
	bool															is_stored_locally,
	FunctionLocalStorageType									&	storage
) noexcept
{
	if( is_stored_locally ) return;

	memory::FreeMemory<FunctorType>( reinterpret_cast<FunctorType*>( storage.heap_functor ), 1 );

	#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
	memset( &storage, 0, sizeof( FunctionLocalStorageType ) );
	#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename FunctorType,
	typename FunctionLocalStorageType
>
const FunctorType												*	GetFunctorPointer(
	bool															is_stored_locally,
	const FunctionLocalStorageType								&	storage
) noexcept
{
	if( is_stored_locally ) return reinterpret_cast<const FunctorType*>( storage.raw );
	return static_cast<const FunctorType*>( storage.heap_functor );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename FunctorType,
	typename FunctionLocalStorageType
>
FunctorType														*	GetFunctorPointer(
	bool															is_stored_locally,
	FunctionLocalStorageType									&	storage
) noexcept
{
	if( is_stored_locally ) return reinterpret_cast<FunctorType*>( storage.raw );
	return static_cast<FunctorType*>( storage.heap_functor );
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename FunctorType,
	typename FunctionLocalStorageType
>
consteval bool IsFunctorStoredLocally() noexcept
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
template<
	typename FunctionLocalStorageType,
	typename ReturnType,
	typename ...ParameterTypes
>
class FunctorManagerBase
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~FunctorManagerBase() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ReturnType							Invoke( 
		bool									is_stored_locally,
		FunctionLocalStorageType			&	storage,
		ParameterTypes...						args
	) = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void								Clone(
		bool									is_stored_locally,
		FunctorManagerBase					&	destination_manager,
		FunctionLocalStorageType			&	destination,
		const FunctionLocalStorageType		&	source
	) const = 0;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void								ClearFunctor(
		bool									is_stored_locally,
		FunctionLocalStorageType			&	storage
	) noexcept = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	typename FunctorType,
	typename FunctionLocalStorageType,
	typename ReturnType,
	typename ...ParameterTypes
>
class FunctorManager : public FunctorManagerBase<FunctionLocalStorageType, ReturnType, ParameterTypes...>
{
public:

	using MyFunctorManagerBase = FunctorManagerBase<FunctionLocalStorageType, ReturnType, ParameterTypes...>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	FunctorManager() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ReturnType							Invoke(
		bool									is_stored_locally,
		FunctionLocalStorageType			&	storage,
		ParameterTypes...						args
	) override
	{
		auto functor_pointer = GetFunctorPointer<FunctorType, FunctionLocalStorageType>( is_stored_locally, storage );
		return ( *functor_pointer )( std::forward<ParameterTypes>( args )... );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void								Clone(
		bool									is_stored_locally,
		MyFunctorManagerBase				&	destination_manager,
		FunctionLocalStorageType			&	destination,
		const FunctionLocalStorageType		&	source
	) const override
	{
		// Allocate space for the functor in destination storage.
		auto functor_pointer = ::bc::internal_::container::AllocateFunctor<FunctorType, FunctionLocalStorageType>(
			is_stored_locally,
			destination
		);

		// Copy the functor from source to destination by invoking the copy constructor.
		::new( functor_pointer ) FunctorType( *GetFunctorPointer<FunctorType, FunctionLocalStorageType>( is_stored_locally, source ) );

		::new( &destination_manager ) FunctorManager();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual void								ClearFunctor(
		bool									is_stored_locally,
		FunctionLocalStorageType			&	storage
	) noexcept override
	{
		auto functor_pointer = GetFunctorPointer<FunctorType, FunctionLocalStorageType>( is_stored_locally, storage );
		functor_pointer->~FunctorType();
		FreeFunctor<FunctorType, FunctionLocalStorageType>( is_stored_locally, storage );
	}
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // namespace container
} // namespace internal_
} // namespace bc
