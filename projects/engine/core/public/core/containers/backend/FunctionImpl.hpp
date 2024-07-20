
#include <core/containers/backend/ContainerBase.hpp>

#include <type_traits>
#include <core/utility/template/TypeList.hpp>
#include <core/utility/template/CallableTraits.hpp>

#if BC_CONTAINER_IMPLEMENTATION_NORMAL
#elif BC_CONTAINER_IMPLEMENTATION_SIMPLE
#else
#error "Container implementation type not given"
#endif

#include <core/containers/backend/ContainerImplAddDefinitions.hpp>



namespace bc {
namespace internal_ {

} // internal_

BC_CONTAINER_NAMESPACE_START;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: Working on a mechanism to get parameter pack displayed to the user via the IDE.
template<typename ...CallOperatorParameterTypeList>
class OperatorCallerImpl
{
	ReturnType operator()( CallOperatorParameterTypeList ...args )
	{
		// TODO: Call function object.
	}
};

template<typename CallableType>
using OperatorCallerParamTypeList = typename utility::CallableTraits<CallableType>::ParameterTypeList;

template<typename CallableType>
using OperatorCallerBase = OperatorCallerParamTypeList<CallableType>::template ApplyTo<OperatorCallerImpl>;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Wrapper for callable types.
///
/// Not as efficient as regular function pointers, but can be used for storing lambda functions. This is useful when it is
/// unknown if a function will be a regular function or a lambda function.
///
/// @tparam CallableType
/// Type of the callable object. Must be a function, lambda, or other callable object.
template<typename CallableType>
class BC_CONTAINER_NAME( Function ) : private container_bases::ContainerResource, OperatorCallerBase<CallableType>
{
	using CallableTraits = utility::CallableTraits<CallableType>;

	using ReturnType = typename CallableTraits::ReturnType;
	using CallParameterTypeList = typename CallableTraits::ParameterTypeList;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Class for storing the regular function pointer.
	class ImplRegularFunction
	{
		// TODO
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Class for storing a member function pointer.
	class ImplMemberFunction
	{
		// TODO
	};
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Class for storing the actual callable object. This allocates memory for the object if required.
	class ImplCallableObject
	{
		// TODO
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Class for storing the actual callable object. This does not allocate memory for the object.
	class ImplCallableObjectNoAlloc
	{
	public:
		ImplCallableObjectNoAlloc( CallableType & function )
		{
			// TODO: Copy construct function object.
		}

		ImplCallableObjectNoAlloc( CallableType && function )
		{
			// TODO: Move construct function object.
		}

		~ImplCallableObjectNoAlloc()
		{

		}

		template<typename... CallParameterTypePack>
		ReturnType DoCall( CallParameterTypePack... )
		{
			#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
			static_assert( std::is_same_v<utility::TypeList<CallParameterTypePack...>, CallParameterTypeList> );
			#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD

			// TODO: Get function pointer.
			// TODO; // Figure out what types of functions are allowed in std::function. If signature is void(), this Function class should
			// accpet plain functions, member functions, lambdas and callable objects with similar signature. The actual type is resolved at
			// runtime. A void() function does not need storage, just the pointer, same as member functions. Lambda functions and any
			// callable object will need storage for local variables of the callable object, or lambda capture list.
			// Play around with std::function to see what types it accepts.

			// Regular function pointer, lambda, or other callable object signature can be just Function<void()> as an example. A member
			// function signature must be given when creating the Function type. Eg. Function<void( ClassType*, int )>; where ClassType is
			// the type of the class that has the member function. This could also work with member callable objects. Eg.
			// ClassType::MemberCallable. Where ClassType is the type of the class that has the member function and MemberCallable is
			// the type of the member class which is callable.
		}

		union
		{
			u8 												storage[ 32 ];
		};
		// TODO
	};

	enum class ImplType : u8
	{
		EMPTY								= 0,
		REGULAR,
		MEMBER,
		OBJECT,
		OBJECT_NO_ALLOC,
	};

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	using Base								= void;
	using ContainedValueType				= CallableType;
	static constexpr bool IsDataConst		= false;

	template<typename OtherCallableType>
	using ThisContainerType					= BC_CONTAINER_NAME( Function )<OtherCallableType>;
	using ThisType							= ThisContainerType<CallableType>;

	template<typename OtherCallableType, bool IsOtherConst>
	using ThisContainerViewType				= void;

	template<bool IsOtherConst>
	using ThisViewType						= void;

	template<typename OtherCallableType>
	using ThisContainerFullType				= BC_CONTAINER_NAME( Function )<OtherCallableType>;
	using ThisFullType						= ThisContainerFullType<CallableType>;

	using value_type						= CallableType;	// for stl compatibility.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherCallableType>
	friend class BC_CONTAINER_NAME( Function );

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Function )() noexcept = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr explicit BC_CONTAINER_NAME( Function )( nullptr_t ) noexcept {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Function )(
		const BC_CONTAINER_NAME( Function )															&	other
	) BC_CONTAINER_NOEXCEPT
	{
		// TODO: Copy construct function object.
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Function )(
		BC_CONTAINER_NAME( Function )																&&	other
	) noexcept
	{
		// TODO: Move construct function object.
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherCallableType>
	constexpr BC_CONTAINER_NAME( Function )(
		OtherCallableType																			&	callable
	) BC_CONTAINER_NOEXCEPT
	{
		// TODO: Copy construct function object.
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherCallableType>
	constexpr BC_CONTAINER_NAME( Function )(
		OtherCallableType																			&&	callable
	) noexcept
	{
		// TODO: Move construct function object.
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr ~BC_CONTAINER_NAME( Function )( ) BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Function )															&	operator=(
		nullptr_t
	) BC_CONTAINER_NOEXCEPT
	{
		this->Clear();
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Function )															&	operator=(
		const BC_CONTAINER_NAME( Function )															&	other
	) BC_CONTAINER_NOEXCEPT
	{
		this->CopyOther( other );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr BC_CONTAINER_NAME( Function )															&	operator=(
		BC_CONTAINER_NAME( Function )																&&	other
	) noexcept
	{
		this->MoveOther( std::move( other ) );
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherCallableType>
	constexpr BC_CONTAINER_NAME( Function )															&	operator=(
		OtherCallableType																			&	callable
	) BC_CONTAINER_NOEXCEPT
	{
		// TODO: Copy function object.
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename OtherCallableType>
	constexpr BC_CONTAINER_NAME( Function )															&	operator=(
		OtherCallableType																			&&	callable
	) BC_CONTAINER_NOEXCEPT
	{
		// TODO: Move function object.
		return *this;
	}

	//template<typename ...CallParameterTypeList>
	//constexpr ReturnType																				operator()()
	//{
	//	// TODO: Call function object.
	//}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr void																						Clear() BC_CONTAINER_NOEXCEPT
	{
		if( this->IsEmpty() ) return;
		// TODO: Destruct function object.
		this->impl_type = ImplType::EMPTY;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constexpr bool																						IsEmpty() const noexcept
	{
		return this->impl_type == ImplType::EMPTY;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	explicit constexpr operator bool() const noexcept
	{
		return this->IsEmpty();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																								CopyOther(
		const BC_CONTAINER_NAME( Function )															&	other
	) noexcept requires( BC_CONTAINER_IS_COPY_ASSIGNABLE<CallableType> )
	{
		if( std::addressof( other ) == this ) return;
		this->Clear();
		// TODO: Copy function object.
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void																								MoveOther(
		BC_CONTAINER_NAME( Function )																&&	other
	) noexcept requires( BC_CONTAINER_IS_MOVE_ASSIGNABLE<CallableType> )
	{
		if( std::addressof( other ) == this ) return;
		// TODO: Move function object.
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ImplType																							impl_type					= ImplType::EMPTY;

	union
	{
		u8																								dummy						= {};
		ImplRegularFunction																				impl_regular_function;
		ImplMemberFunction																				impl_member_function;
		ImplCallableObject																				impl_callable_object;
		ImplCallableObjectNoAlloc																		impl_callable_object_no_alloc;
	};
};



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD && 0 // TODO: Add tests once the function class is fully implemented.
namespace tests {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill size requirements.
static_assert( sizeof( BC_CONTAINER_NAME( Function )<u8> ) == 2 );
static_assert( sizeof( BC_CONTAINER_NAME( Function )<u16> ) == 4 );
static_assert( sizeof( BC_CONTAINER_NAME( Function )<u32> ) == 8 );
static_assert( sizeof( BC_CONTAINER_NAME( Function )<u64> ) == 16 );



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Check if text containers fulfill concept requirements.
static_assert( !utility::ContainerView<BC_CONTAINER_NAME( Function )<u32>> );
static_assert( !utility::ContainerEditableView<BC_CONTAINER_NAME( Function )<u32>> );
static_assert( !utility::Container<BC_CONTAINER_NAME( Function )<u32>> );

static_assert( !utility::LinearContainerView<BC_CONTAINER_NAME( Function )<u32>> );
static_assert( !utility::LinearContainerEditableView<BC_CONTAINER_NAME( Function )<u32>> );
static_assert( !utility::LinearContainer<BC_CONTAINER_NAME( Function )<u32>> );

static_assert( !utility::TextContainerView<BC_CONTAINER_NAME( Function )<c32>> );
static_assert( !utility::TextContainerEditableView<BC_CONTAINER_NAME( Function )<c32>> );
static_assert( !utility::TextContainer<BC_CONTAINER_NAME( Function )<c32>> );

} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



BC_CONTAINER_NAMESPACE_END;
} // bc



#include <core/containers/backend/ContainerImplRemoveDefinitions.hpp>
