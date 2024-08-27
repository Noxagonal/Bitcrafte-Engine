#pragma once

#include <core/utility/concepts/TypeTraitConcepts.hpp>
#include <core/containers/backend/ContainerBase.hpp>
#include <core/utility/template/InvocableTraits.hpp>
#include <core/diagnostic/assertion/Assert.hpp>

#include <cstring>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Container for storing functions, invocable objects and lambdas so that they can be invoked later via this container.
///
/// We'll refer to functions, invocable objects and lambdas as "functors".
///
/// This container is similar to std::function, it stores a functor and allows it to be invoked later. This is especially useful
/// when storing lambdas to be invoked later.
///
/// @note
/// There is a small performance cost when using this container, especially from invocable objects like lambdas, mostly from CPU
/// cache misses. Try to avoid using this in hot loops.
///
/// Usage example:
/// @code
/// auto my_function = bc::Function(
///		[]( int a, float b )
/// 	{
/// 		return a * b;
/// 	};
///	);
/// auto result = my_function( 2, 3.0f );
/// @endcode
///
/// @tparam Signature
/// Signature of the contained function. Eg. <tt>int(int, float)</tt>
template<typename Signature>
class Function;

template <typename ReturnType, typename ...ParameterTypes>
class Function<ReturnType( ParameterTypes... )>
{
	using MyFunction = ReturnType( ParameterTypes... );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Signature = ReturnType( ParameterTypes... );
	using Traits = ::bc::utility::InvocableTraits<Signature>;

	//using ReturnType = typename Traits::ReturnType;
	//using CallParameterTypeList = typename Traits::ParameterTypeList;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= Signature;
	static constexpr bool IsDataConst		= false;

	template<typename OtherSignature>
	using ThisContainerType					= Function<OtherSignature>;
	using ThisType							= ThisContainerType<Signature>;

	template<typename OtherSignature, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<typename OtherSignature>
	using ThisContainerFullType				= Function<OtherSignature>;
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
		MyFunction*		function_pointer;
		void*			heap_functor;
		u8				raw[ 16 ];
	};
	static_assert( sizeof( LocalStorage ) == 16 );

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	class FunctorManagerBase
	{
		// TODO: Currently we're using this manager as a virtual base class. This is not ideal for performance because of vtable
		//       indirection. We can substitude this with a single pointer to function instead. This function would be a template
		//       function that gets instantiated by Store() function, and pointer to it would be reinterpret_casted to a function
		//       signature of common type. Just need to make sure that the function signature matches. We only need one function for
		//       every operation as we can just pass an op-code to it. A switch statement inside the function would probably be
		//       faster than a vtable indirection. On the other hand, reinterpret_cast may prevent some optimizations so when we do
		//       this, performance needs to be measured. Also, make sure that we can do this safely.

	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual ~FunctorManagerBase() noexcept = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual auto Invoke(
			bool				is_stored_locally,
			LocalStorage&		storage,
			ParameterTypes...	args
		) -> ReturnType = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void Clone(
			bool				is_stored_locally,
			FunctorManagerBase&	destination_manager,
			LocalStorage&		destination,
			const LocalStorage&	source
		) const = 0;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void ClearFunctor(
			bool				is_stored_locally,
			LocalStorage&		storage
		) noexcept = 0;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	class FunctorManager : public FunctorManagerBase
	{
	public:

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		FunctorManager() = default;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual auto Invoke(
			bool				is_stored_locally,
			LocalStorage&		storage,
			ParameterTypes...	args
		) -> ReturnType override
		{
			auto functor_pointer = GetFunctorPointer<FunctorType>( is_stored_locally, storage );
			return ( *functor_pointer )( std::forward<ParameterTypes>( args )... );
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void Clone(
			bool				is_stored_locally,
			FunctorManagerBase&	destination_manager,
			LocalStorage&		destination,
			const LocalStorage&	source
		) const override
		{
			// Allocate space for the functor in destination storage.
			auto functor_pointer = AllocateFunctor<FunctorType>(
				is_stored_locally,
				destination
			);

			// Copy the functor from source to destination by invoking the copy constructor.
			::new( functor_pointer ) FunctorType( *GetFunctorPointer<FunctorType>( is_stored_locally, source ) );

			::new( &destination_manager ) FunctorManager();
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		virtual void ClearFunctor(
			bool			is_stored_locally,
			LocalStorage&	storage
		) noexcept override
		{
			auto functor_pointer = GetFunctorPointer<FunctorType>( is_stored_locally, storage );
			functor_pointer->~FunctorType();
			FreeFunctor<FunctorType>( is_stored_locally, storage );
		}
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using FunctorManagerStorage = std::aligned_storage_t<sizeof( FunctorManagerBase ), alignof( FunctorManagerBase )>;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Default constructor.
	constexpr Function() noexcept
	{
		DebugClearStorage();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Copy constructor.
	///
	/// @param other
	/// Function to copy.
	Function( const Function& other )
	{
		DebugClearStorage();

		Copy( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Move constructor.
	///
	/// @param other
	/// Function to move.
	Function( Function&& other ) noexcept
	{
		DebugClearStorage();

		Swap( other );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Construct from a functor type.
	///
	/// @tparam FunctorType
	/// Type of the functor.
	///
	/// @param functor
	/// Functor to store.
	template<typename FunctorType>
	Function( FunctorType&& functor )
		requires(
	!std::is_same_v<std::remove_reference_t<std::remove_pointer_t<std::decay_t<FunctorType>>>, Function> &&
		std::is_copy_constructible_v<FunctorType> &&
		std::is_nothrow_destructible_v<FunctorType> &&
		utility::InvocableWithReturn<FunctorType, ReturnType, ParameterTypes...>
		)
	{
		using FunctorBaseType = std::remove_reference_t<std::remove_pointer_t<std::decay_t<FunctorType>>>;
		using FunctorTraits = ::bc::utility::InvocableTraits<FunctorBaseType>;

		#if BC_CONTAINER_IMPLEMENTATION_SIMPLE
		// This might be too restrictive, but simple implementation should always be noexcept, this is one way to enforce it.
		static_assert(
			std::is_nothrow_invocable_v<FunctorType, ParameterTypes...>,
			"FunctorType must be noexcept invocable when simple implementation is used."
			);
		#endif

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

		DebugClearStorage();

		Store( std::forward<FunctorType>( functor ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~Function() noexcept
	{
		Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Copy assignment operator.
	///
	/// @param other
	/// Function to copy.
	///
	/// @returns
	/// Reference to this.
	auto operator=( const Function& other ) -> Function&
	{
		if( std::addressof( other ) != this )
		{
			Function( other ).Swap( *this );
		}
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Move assignment operator.
	///
	/// @param other
	/// Function to move.
	///
	/// @returns
	/// Reference to this.
	auto operator=( Function&& other ) noexcept -> Function&
	{
		if( std::addressof( other ) != this )
		{
			Clear();
			Swap( other );

			// Could also do this, but we'll save a swap by doing it directly as swapping cannot throw,
			// we don't need to worry about it. The compiler might be able to optimize this anyhow.
			//Function( std::move( other ) ).Swap( *this );
		}
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Functor assignment operator.
	///
	/// @tparam FunctorType
	/// Type of the functor to assign.
	///
	/// @param functor
	/// The functor to assign.
	///
	/// @return
	/// A reference to this object.
	template<typename FunctorType>
	auto operator=( FunctorType&& functor )->Function&
		requires(
	!std::is_same_v<std::remove_reference_t<std::remove_pointer_t<std::decay_t<FunctorType>>>, Function> &&
		std::is_copy_constructible_v<FunctorType> &&
		std::is_nothrow_destructible_v<FunctorType> &&
		utility::InvocableWithReturn<FunctorType, ReturnType, ParameterTypes...>
		)
	{
		Function( std::forward<FunctorType>( functor ) ).Swap( *this );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Invokes the stored functor.
	///
	/// @note
	/// If this Function is empty, an exception will be thrown in debug builds, and program will crash in release builds.
	///
	/// @param args
	/// Arguments to pass to the functor.
	///
	/// @return
	/// The return value of the functor.
	auto operator()( ParameterTypes... args ) -> ReturnType
	{
		BAssert( !IsEmpty(), U"Cannot invoke empty function." );

		if( this->type == Type::INVOKEABLE_OBJECT )
		{
			auto manager = reinterpret_cast<FunctorManagerBase*>( &this->functor_manager );
			return manager->Invoke( this->is_stored_locally, this->storage, std::forward<ParameterTypes>( args )... );
		}
		else
		{
			auto function_ptr = storage.function_pointer;
			return function_ptr( std::forward<ParameterTypes>( args )... );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Clears the Function, making it empty.
	void Clear() noexcept
	{
		if( this->type == Type::INVOKEABLE_OBJECT )
		{
			auto manager = reinterpret_cast<FunctorManagerBase*>( &this->functor_manager );
			manager->ClearFunctor( this->is_stored_locally, this->storage );
			manager->~FunctorManagerBase();
		}
		this->is_stored_locally = false;
		this->type = Type::NONE;
		DebugClearStorage();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Checks if the function is stored in the stack.
	///
	/// This is useful mostly for debugging purposes. Typically you don't need this, unless you are collecting statistics.
	///
	/// @return
	/// true if the function is stored in the stack, false when the function is stored in the heap.
	auto IsStoredLocally() const noexcept -> bool
	{
		BAssert( !IsEmpty(), U"Cannot check empty function stack locality, results would be meaningless." );
		return this->is_stored_locally;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Checks if the function is empty.
	///
	/// @return
	/// true if the function is empty and cannot be invoked, false otherwise.
	auto IsEmpty() const noexcept -> bool
	{
		return this->type == Type::NONE;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Checks if the function stores a functor.
	///
	/// @return
	/// true if the function is not empty and can be invoked, false otherwise.
	explicit operator bool() const noexcept
	{
		return !IsEmpty();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Copy( const Function& other )
	{
		assert( this->type == Type::NONE && "Function already initialized." );

		if( other.type == Type::NONE ) return;

		this->is_stored_locally = other.is_stored_locally;
		this->type = other.type;

		if( other.type == Type::INVOKEABLE_OBJECT )
		{
			auto my_manager = reinterpret_cast<FunctorManagerBase*>( &this->functor_manager );
			auto other_manager = reinterpret_cast<const FunctorManagerBase*>( &other.functor_manager );
			other_manager->Clone( other.is_stored_locally, *my_manager, this->storage, other.storage );
		}
		else
		{
			this->storage.function_pointer = other.storage.function_pointer;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Swap( Function& other ) noexcept
	{
		std::swap( this->is_stored_locally, other.is_stored_locally );
		std::swap( this->type, other.type );
		std::swap( this->functor_manager, other.functor_manager );
		std::swap( this->storage, other.storage );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	void Store( FunctorType&& functor )
	{
		assert( this->type == Type::NONE && "Function already initialized." );

		using FunctorBaseType = std::remove_reference_t<std::remove_pointer_t<FunctorType>>;
		using FunctorTraits = ::bc::utility::InvocableTraits<FunctorBaseType>;
		constexpr bool is_plain_function = FunctorTraits::IsPlainFunction();

		if constexpr( is_plain_function )
		{
			this->is_stored_locally = true;
			this->type = Type::FUNCTION;

			storage.function_pointer = functor;
		}
		else
		{
			using NewFunctorManagerType = FunctorManager<FunctorBaseType>;
			static_assert( sizeof( NewFunctorManagerType ) == 8, "FunctorManager size is not 8 bytes." );
			static_assert( alignof( NewFunctorManagerType ) == alignof( FunctorManagerBase ), "NewFunctorManagerType alignment is not equal to FunctorManagerBase." );

			constexpr bool store_locally = TestFunctorCanBeStoredLocally<FunctorBaseType>();

			this->is_stored_locally = store_locally;
			this->type = Type::INVOKEABLE_OBJECT;

			auto functor_pointer = AllocateFunctor<FunctorBaseType>( store_locally, storage );
			::new( functor_pointer ) FunctorBaseType( std::forward<FunctorType>( functor ) );

			::new( &this->functor_manager ) NewFunctorManagerType();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	static auto AllocateFunctor(
		bool			is_stored_locally,
		LocalStorage&	storage
	) noexcept -> FunctorType*
	{
		if( is_stored_locally )
		{
			assert( sizeof( FunctorType ) <= sizeof( LocalStorage ) && "Functor type is too big." );
			assert( alignof( FunctorType ) <= alignof( LocalStorage ) && "Functor alignment does not fit in storage." );
			return reinterpret_cast<FunctorType*>( storage.raw );
		}
		else
		{
			storage.heap_functor = memory::AllocateMemory<FunctorType>( 1, alignof( FunctorType ) );
			return static_cast<FunctorType*>( storage.heap_functor );
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	static void FreeFunctor(
		bool			is_stored_locally,
		LocalStorage&	storage
	) noexcept
	{
		if( is_stored_locally ) return;

		memory::FreeMemory<FunctorType>( reinterpret_cast<FunctorType*>( storage.heap_functor ), 1 );

		#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
		std::memset( &storage, 0, sizeof( LocalStorage ) );
		#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	static auto GetFunctorPointer(
		bool				is_stored_locally,
		const LocalStorage&	storage
	) noexcept -> const FunctorType*
	{
		if( is_stored_locally ) return reinterpret_cast<const FunctorType*>( storage.raw );
		return static_cast<const FunctorType*>( storage.heap_functor );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	static auto GetFunctorPointer(
		bool			is_stored_locally,
		LocalStorage&	storage
	) noexcept -> FunctorType*
	{
		if( is_stored_locally ) return reinterpret_cast<FunctorType*>( storage.raw );
		return static_cast<FunctorType*>( storage.heap_functor );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename FunctorType>
	static consteval auto TestFunctorCanBeStoredLocally() noexcept -> bool
	{
		constexpr auto StorageSize = sizeof( LocalStorage );
		constexpr auto StorageAlignment = alignof( LocalStorage );
		return
			std::is_trivially_copyable_v<FunctorType> &&
			sizeof( FunctorType ) <= StorageSize &&
			alignof( FunctorType ) <= StorageAlignment &&
			( StorageAlignment % alignof( FunctorType ) == 0 );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void DebugClearStorage() noexcept
	{
		#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
		std::memset( &storage, 0, sizeof( decltype( storage ) ) );
		#endif
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool					is_stored_locally		= false;
	Type					type					= Type::NONE;
	FunctorManagerStorage	functor_manager;
	LocalStorage			storage;
};



// Deduction guide for functions.
template <typename ReturnType, typename... ParameterTypes>
Function( ReturnType( * )( ParameterTypes... ) )
-> Function<ReturnType( ParameterTypes... )>;

// Deduction guide for invokeable objects.
template<typename FunctorType>
	requires( !std::is_pointer_v<FunctorType> && !std::is_reference_v<FunctorType> && !std::is_same_v<FunctorType, void> && !std::is_lvalue_reference_v<FunctorType> && !std::is_rvalue_reference_v<FunctorType> && utility::InvocableObject<FunctorType> )
Function( FunctorType )
->Function<typename utility::InvocableTraits<FunctorType>::Signature>;



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if function container fulfills size requirements.
static_assert( sizeof( Function<void()> ) == 32 );

// Check that the function satisfies basic copy and move constraints.
static_assert( std::is_copy_constructible_v<Function<void()>> );
static_assert( std::is_move_constructible_v<Function<void()>> );
static_assert( std::is_nothrow_move_constructible_v<Function<void()>> );

static_assert( std::is_copy_assignable_v<Function<void()>> );
static_assert( std::is_move_assignable_v<Function<void()>> );
static_assert( std::is_nothrow_move_assignable_v<Function<void()>> );

} // namespace tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // bc
