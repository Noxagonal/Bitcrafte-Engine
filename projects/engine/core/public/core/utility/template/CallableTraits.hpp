#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/data_types/FundamentalTypes.hpp>
#include <core/utility/concepts/CallableConcepts.hpp>
#include <core/utility/template/TypeList.hpp>



namespace bc {
namespace utility {
namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	bool IsPlainFunctionCallable,
	bool IsMemberCallable,
	bool IsConstCallable,
	bool IsVolatileCallable,
	bool IsLValueOnlyCallable,
	bool IsRValueOnlyCallable,
	bool IsNoexceptCallable,
	typename CallableReturnType,
	typename ...CallableParameterTypePack
>
class CallableTraitsBase
{
public:
	using Signature				= CallableReturnType(CallableParameterTypePack...);
	using ReturnType			= CallableReturnType;
	using ParameterTypeList		= TypeList<CallableParameterTypePack...>;

	static consteval u64 ParameterCount() noexcept { return ParameterTypeList::Size(); }

	static consteval bool IsPlainFunction() { return IsPlainFunctionCallable; }
	static consteval bool IsMemberFunction() { return IsMemberCallable; }
	static consteval bool IsConst() { return IsConstCallable; }
	static consteval bool IsVolatile() { return IsVolatileCallable; }
	static consteval bool IsLValueOnly() { return IsLValueOnlyCallable; }
	static consteval bool IsRValueOnly() { return IsRValueOnlyCallable; }
	static consteval bool IsNoexcept() { return IsNoexceptCallable; }
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Helper class to resolve the traits of a callable object.
///
/// Specializations below are used to resolve the traits of each callable object operator().
///
/// @tparam Type
/// Unused.
template<typename Type>
class CallableObjectResolver
{
	// Disabled default error condition because it may be useful to use this class as a test in SFINAE, eg. in deduction guides.
	//static constexpr bool is_callable = []<bool b = false>(){
	//	static_assert( b, "Cannot get a callable traits from a non callable type." );
	//	return b;
	//}();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... )>
	: public CallableTraitsBase<false, false, false, false, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const>
	: public CallableTraitsBase<false, false, true, false, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) volatile>
	: public CallableTraitsBase<false, false, false, true, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const volatile>
	: public CallableTraitsBase<false, false, true, true, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) &>
	: public CallableTraitsBase<false, false, false, false, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const &>
	: public CallableTraitsBase<false, false, true, false, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) volatile &>
	: public CallableTraitsBase<false, false, false, true, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const volatile &>
	: public CallableTraitsBase<false, false, true, true, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) &&>
	: public CallableTraitsBase<false, false, false, false, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const &&>
	: public CallableTraitsBase<false, false, true, false, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) volatile &&>
	: public CallableTraitsBase<false, false, false, true, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const volatile &&>
	: public CallableTraitsBase<false, false, true, true, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) noexcept>
	: public CallableTraitsBase<false, false, false, false, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const noexcept>
	: public CallableTraitsBase<false, false, true, false, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) volatile noexcept>
	: public CallableTraitsBase<false, false, false, true, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const volatile noexcept>
	: public CallableTraitsBase<false, false, true, true, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) & noexcept>
	: public CallableTraitsBase<false, false, false, false, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const & noexcept>
	: public CallableTraitsBase<false, false, true, false, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) volatile & noexcept>
	: public CallableTraitsBase<false, false, false, true, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const volatile & noexcept>
	: public CallableTraitsBase<false, false, true, true, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) && noexcept>
	: public CallableTraitsBase<false, false, false, false, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const && noexcept>
	: public CallableTraitsBase<false, false, true, false, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) volatile && noexcept>
	: public CallableTraitsBase<false, false, false, true, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableObjectResolver<CallableReturnType( CallableClassType::* )( CallableParameterTypePack... ) const volatile && noexcept>
	: public CallableTraitsBase<false, false, true, true, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Collects information about a callable object.
///
/// This class is useful when trying to extract information from a callable object.
///
/// @tparam CallableType
/// Callable object type.
template<typename CallableType>
class CallableTraits
{
	// Disabled default error condition because it may be useful to use this class as a test in SFINAE, eg. in deduction guides.
	//static constexpr bool is_callable = []<bool b = false>(){
	//	static_assert( b, "Not a callable type" );
	//	return b;
	//}();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<CallableObject CallableType>
class CallableTraits<CallableType> :
	public internal_::CallableObjectResolver<decltype( &CallableType::operator() )>
{};



// Plain functions.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...)> :
	public internal_::CallableTraitsBase<true, false, false, false, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const> :
	public internal_::CallableTraitsBase<true, false, true, false, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) volatile> :
	public internal_::CallableTraitsBase<true, false, false, true, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const volatile> :
	public internal_::CallableTraitsBase<true, false, true, true, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) &> :
	public internal_::CallableTraitsBase<true, false, false, false, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const &> :
	public internal_::CallableTraitsBase<true, false, true, false, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) volatile &> :
	public internal_::CallableTraitsBase<true, false, false, true, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const volatile &> :
	public internal_::CallableTraitsBase<true, false, true, true, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) &&> :
	public internal_::CallableTraitsBase<true, false, false, false, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const &&> :
	public internal_::CallableTraitsBase<true, false, true, false, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) volatile &&> :
	public internal_::CallableTraitsBase<true, false, false, true, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const volatile &&> :
	public internal_::CallableTraitsBase<true, false, true, true, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) noexcept> :
	public internal_::CallableTraitsBase<true, false, false, false, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const noexcept> :
	public internal_::CallableTraitsBase<true, false, true, false, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) volatile noexcept> :
	public internal_::CallableTraitsBase<true, false, false, true, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const volatile noexcept> :
	public internal_::CallableTraitsBase<true, false, true, true, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) & noexcept> :
	public internal_::CallableTraitsBase<true, false, false, false, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const & noexcept> :
	public internal_::CallableTraitsBase<true, false, true, false, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) volatile & noexcept> :
	public internal_::CallableTraitsBase<true, false, false, true, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const volatile & noexcept> :
	public internal_::CallableTraitsBase<true, false, true, true, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) && noexcept> :
	public internal_::CallableTraitsBase<true, false, false, false, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const && noexcept> :
	public internal_::CallableTraitsBase<true, false, true, false, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) volatile && noexcept> :
	public internal_::CallableTraitsBase<true, false, false, true, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType(CallableParameterTypePack...) const volatile && noexcept> :
	public internal_::CallableTraitsBase<true, false, true, true, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};



// Member functions.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... )> :
	public internal_::CallableTraitsBase<true, true, false, false, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const> :
	public internal_::CallableTraitsBase<true, true, true, false, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) volatile> :
	public internal_::CallableTraitsBase<true, true, false, true, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const volatile> :
	public internal_::CallableTraitsBase<true, true, true, true, false, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) &> :
	public internal_::CallableTraitsBase<true, true, false, false, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const &> :
	public internal_::CallableTraitsBase<true, true, true, false, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) volatile &> :
	public internal_::CallableTraitsBase<true, true, false, true, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const volatile&> :
	public internal_::CallableTraitsBase<true, true, true, true, true, false, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) &&> :
	public internal_::CallableTraitsBase<true, true, false, false, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const &&> :
	public internal_::CallableTraitsBase<true, true, true, false, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) volatile &&> :
	public internal_::CallableTraitsBase<true, true, false, true, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const volatile &&> :
	public internal_::CallableTraitsBase<true, true, true, true, false, true, false, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) noexcept> :
	public internal_::CallableTraitsBase<true, true, false, false, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const noexcept> :
	public internal_::CallableTraitsBase<true, true, true, false, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) volatile noexcept> :
	public internal_::CallableTraitsBase<true, true, false, true, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const volatile noexcept> :
	public internal_::CallableTraitsBase<true, true, true, true, false, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) & noexcept> :
	public internal_::CallableTraitsBase<true, true, false, false, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const & noexcept> :
	public internal_::CallableTraitsBase<true, true, true, false, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) volatile & noexcept> :
	public internal_::CallableTraitsBase<true, true, false, true, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const volatile & noexcept> :
	public internal_::CallableTraitsBase<true, true, true, true, true, false, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) && noexcept> :
	public internal_::CallableTraitsBase<true, true, false, false, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const && noexcept> :
	public internal_::CallableTraitsBase<true, true, true, false, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) volatile && noexcept> :
	public internal_::CallableTraitsBase<true, true, false, true, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename CallableReturnType, typename ...CallableParameterTypePack>
class CallableTraits<CallableReturnType( ClassType::* )( CallableParameterTypePack... ) const volatile && noexcept> :
	public internal_::CallableTraitsBase<true, true, true, true, false, true, true, CallableReturnType, CallableParameterTypePack...>
{};



#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

// Basic functions.
static_assert( sizeof( CallableTraits<void()> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double )> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) volatile> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const volatile> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) &> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const &> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) volatile &> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const volatile &> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) &&> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const &&> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) volatile &&> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const volatile &&> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) volatile noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const volatile noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) & noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const & noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) volatile & noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const volatile & noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) && noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const && noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) volatile && noexcept> ) == 1 );
static_assert( sizeof( CallableTraits<void( int, float, double ) const volatile && noexcept> ) == 1 );

static_assert( std::is_same_v<CallableTraits<void()>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<int()>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<int( int, float, double )>::ReturnType, int> );

static_assert( std::is_same_v<CallableTraits<void()>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) volatile>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const volatile>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) &>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const &>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) volatile &>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const volatile &>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) &&>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const &&>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) volatile &&>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const volatile &&>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) volatile noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const volatile noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) & noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const & noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) volatile & noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const volatile & noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) && noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const && noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) volatile && noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<void( int, float, double ) const volatile && noexcept>::ParameterTypeList, TypeList<int, float, double>> );


static_assert( CallableTraits<void()>::ParameterCount() == 0 );
static_assert( CallableTraits<void( int, float, double )>::ParameterCount() == 3 );

static_assert( CallableTraits<void()>::IsPlainFunction() == true );
static_assert( CallableTraits<void()>::IsMemberFunction() == false );
static_assert( CallableTraits<void()>::IsConst() == false );
static_assert( CallableTraits<void()>::IsVolatile() == false );
static_assert( CallableTraits<void()>::IsLValueOnly() == false );
static_assert( CallableTraits<void()>::IsRValueOnly() == false );
static_assert( CallableTraits<void()>::IsNoexcept() == false );

static_assert( CallableTraits<void() const>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const>::IsConst() == true );
static_assert( CallableTraits<void() const>::IsVolatile() == false );
static_assert( CallableTraits<void() const>::IsLValueOnly() == false );
static_assert( CallableTraits<void() const>::IsRValueOnly() == false );
static_assert( CallableTraits<void() const>::IsNoexcept() == false );

static_assert( CallableTraits<void() volatile>::IsPlainFunction() == true );
static_assert( CallableTraits<void() volatile>::IsMemberFunction() == false );
static_assert( CallableTraits<void() volatile>::IsConst() == false );
static_assert( CallableTraits<void() volatile>::IsVolatile() == true );
static_assert( CallableTraits<void() volatile>::IsLValueOnly() == false );
static_assert( CallableTraits<void() volatile>::IsRValueOnly() == false );
static_assert( CallableTraits<void() volatile>::IsNoexcept() == false );

static_assert( CallableTraits<void() const volatile>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const volatile>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const volatile>::IsConst() == true );
static_assert( CallableTraits<void() const volatile>::IsVolatile() == true );
static_assert( CallableTraits<void() const volatile>::IsLValueOnly() == false );
static_assert( CallableTraits<void() const volatile>::IsRValueOnly() == false );
static_assert( CallableTraits<void() const volatile>::IsNoexcept() == false );

static_assert( CallableTraits<void() &>::IsPlainFunction() == true );
static_assert( CallableTraits<void() &>::IsMemberFunction() == false );
static_assert( CallableTraits<void() &>::IsConst() == false );
static_assert( CallableTraits<void() &>::IsVolatile() == false );
static_assert( CallableTraits<void() &>::IsLValueOnly() == true );
static_assert( CallableTraits<void() &>::IsRValueOnly() == false );
static_assert( CallableTraits<void() &>::IsNoexcept() == false );

static_assert( CallableTraits<void() const &>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const &>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const &>::IsConst() == true );
static_assert( CallableTraits<void() const &>::IsVolatile() == false );
static_assert( CallableTraits<void() const &>::IsLValueOnly() == true );
static_assert( CallableTraits<void() const &>::IsRValueOnly() == false );
static_assert( CallableTraits<void() const &>::IsNoexcept() == false );

static_assert( CallableTraits<void() volatile &>::IsPlainFunction() == true );
static_assert( CallableTraits<void() volatile &>::IsMemberFunction() == false );
static_assert( CallableTraits<void() volatile &>::IsConst() == false );
static_assert( CallableTraits<void() volatile &>::IsVolatile() == true );
static_assert( CallableTraits<void() volatile &>::IsLValueOnly() == true );
static_assert( CallableTraits<void() volatile &>::IsRValueOnly() == false );
static_assert( CallableTraits<void() volatile &>::IsNoexcept() == false );

static_assert( CallableTraits<void() const volatile &>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const volatile &>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const volatile &>::IsConst() == true );
static_assert( CallableTraits<void() const volatile &>::IsVolatile() == true );
static_assert( CallableTraits<void() const volatile &>::IsLValueOnly() == true );
static_assert( CallableTraits<void() const volatile &>::IsRValueOnly() == false );
static_assert( CallableTraits<void() const volatile &>::IsNoexcept() == false );

static_assert( CallableTraits<void() &&>::IsPlainFunction() == true );
static_assert( CallableTraits<void() &&>::IsMemberFunction() == false );
static_assert( CallableTraits<void() &&>::IsConst() == false );
static_assert( CallableTraits<void() &&>::IsVolatile() == false );
static_assert( CallableTraits<void() &&>::IsLValueOnly() == false );
static_assert( CallableTraits<void() &&>::IsRValueOnly() == true );
static_assert( CallableTraits<void() &&>::IsNoexcept() == false );

static_assert( CallableTraits<void() const &&>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const &&>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const &&>::IsConst() == true );
static_assert( CallableTraits<void() const &&>::IsVolatile() == false );
static_assert( CallableTraits<void() const &&>::IsLValueOnly() == false );
static_assert( CallableTraits<void() const &&>::IsRValueOnly() == true );
static_assert( CallableTraits<void() const &&>::IsNoexcept() == false );

static_assert( CallableTraits<void() volatile &&>::IsPlainFunction() == true );
static_assert( CallableTraits<void() volatile &&>::IsMemberFunction() == false );
static_assert( CallableTraits<void() volatile &&>::IsConst() == false );
static_assert( CallableTraits<void() volatile &&>::IsVolatile() == true );
static_assert( CallableTraits<void() volatile &&>::IsLValueOnly() == false );
static_assert( CallableTraits<void() volatile &&>::IsRValueOnly() == true );
static_assert( CallableTraits<void() volatile &&>::IsNoexcept() == false );

static_assert( CallableTraits<void() const volatile &&>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const volatile &&>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const volatile &&>::IsConst() == true );
static_assert( CallableTraits<void() const volatile &&>::IsVolatile() == true );
static_assert( CallableTraits<void() const volatile &&>::IsLValueOnly() == false );
static_assert( CallableTraits<void() const volatile &&>::IsRValueOnly() == true );
static_assert( CallableTraits<void() const volatile &&>::IsNoexcept() == false );

static_assert( CallableTraits<void() noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() noexcept>::IsConst() == false );
static_assert( CallableTraits<void() noexcept>::IsVolatile() == false );
static_assert( CallableTraits<void() noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<void() noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<void() noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() const noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const noexcept>::IsConst() == true );
static_assert( CallableTraits<void() const noexcept>::IsVolatile() == false );
static_assert( CallableTraits<void() const noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<void() const noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<void() const noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() volatile noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() volatile noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() volatile noexcept>::IsConst() == false );
static_assert( CallableTraits<void() volatile noexcept>::IsVolatile() == true );
static_assert( CallableTraits<void() volatile noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<void() volatile noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<void() volatile noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() const volatile noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const volatile noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const volatile noexcept>::IsConst() == true );
static_assert( CallableTraits<void() const volatile noexcept>::IsVolatile() == true );
static_assert( CallableTraits<void() const volatile noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<void() const volatile noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<void() const volatile noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() & noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() & noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() & noexcept>::IsConst() == false );
static_assert( CallableTraits<void() & noexcept>::IsVolatile() == false );
static_assert( CallableTraits<void() & noexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<void() & noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<void() & noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() const & noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const & noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const & noexcept>::IsConst() == true );
static_assert( CallableTraits<void() const & noexcept>::IsVolatile() == false );
static_assert( CallableTraits<void() const & noexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<void() const & noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<void() const & noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() volatile & noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() volatile & noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() volatile & noexcept>::IsConst() == false );
static_assert( CallableTraits<void() volatile & noexcept>::IsVolatile() == true );
static_assert( CallableTraits<void() volatile & noexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<void() volatile & noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<void() volatile & noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() const volatile & noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const volatile & noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const volatile & noexcept>::IsConst() == true );
static_assert( CallableTraits<void() const volatile & noexcept>::IsVolatile() == true );
static_assert( CallableTraits<void() const volatile & noexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<void() const volatile & noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<void() const volatile & noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() && noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() && noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() && noexcept>::IsConst() == false );
static_assert( CallableTraits<void() && noexcept>::IsVolatile() == false );
static_assert( CallableTraits<void() && noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<void() && noexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<void() && noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() const && noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const && noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const && noexcept>::IsConst() == true );
static_assert( CallableTraits<void() const && noexcept>::IsVolatile() == false );
static_assert( CallableTraits<void() const && noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<void() const && noexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<void() const && noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() volatile && noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() volatile && noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() volatile && noexcept>::IsConst() == false );
static_assert( CallableTraits<void() volatile && noexcept>::IsVolatile() == true );
static_assert( CallableTraits<void() volatile && noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<void() volatile && noexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<void() volatile && noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<void() const volatile && noexcept>::IsPlainFunction() == true );
static_assert( CallableTraits<void() const volatile && noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<void() const volatile && noexcept>::IsConst() == true );
static_assert( CallableTraits<void() const volatile && noexcept>::IsVolatile() == true );
static_assert( CallableTraits<void() const volatile && noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<void() const volatile && noexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<void() const volatile && noexcept>::IsNoexcept() == true );


// Member functions.
struct CallableMemberTest
{
	void ReturningVoid() {}
	int ReturningInt() { return 0; }
	void ReturningVoidWithParameter( int, float, double ) {}
	int ReturningIntWithParameter( int, float, double ) { return 0; }
};

struct CallableMemberTestConst
{
	void ReturningVoid() const {}
	int ReturningInt() const { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const {}
	int ReturningIntWithParameter( int, float, double ) const { return 0; }
};

struct CallableMemberTestVolatile
{
	void ReturningVoid() volatile {}
	int ReturningInt() volatile { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile {}
	int ReturningIntWithParameter( int, float, double ) volatile { return 0; }
};

struct CallableMemberTestConstVolatile
{
	void ReturningVoid() const volatile {}
	int ReturningInt() const volatile { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile {}
	int ReturningIntWithParameter( int, float, double ) const volatile { return 0; }
};

struct CallableMemberTestLValueReference
{
	void ReturningVoid() & {}
	int ReturningInt() & { return 0; }
	void ReturningVoidWithParameter( int, float, double ) & {}
	int ReturningIntWithParameter( int, float, double ) & { return 0; }
};

struct CallableMemberTestConstLValueReference
{
	void ReturningVoid() const & {}
	int ReturningInt() const & { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const & {}
	int ReturningIntWithParameter( int, float, double ) const & { return 0; }
};

struct CallableMemberTestVolatileLValueReference
{
	void ReturningVoid() volatile & {}
	int ReturningInt() volatile & { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile & {}
	int ReturningIntWithParameter( int, float, double ) volatile & { return 0; }
};

struct CallableMemberTestConstVolatileLValueReference
{
	void ReturningVoid() const volatile & {}
	int ReturningInt() const volatile & { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile & {}
	int ReturningIntWithParameter( int, float, double ) const volatile & { return 0; }
};

struct CallableMemberTestRValueReference
{
	void ReturningVoid() && {}
	int ReturningInt() && { return 0; }
	void ReturningVoidWithParameter( int, float, double ) && {}
	int ReturningIntWithParameter( int, float, double ) && { return 0; }
};

struct CallableMemberTestConstRValueReference
{
	void ReturningVoid() const && {}
	int ReturningInt() const && { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const && {}
	int ReturningIntWithParameter( int, float, double ) const && { return 0; }
};

struct CallableMemberTestVolatileRValueReference
{
	void ReturningVoid() volatile && {}
	int ReturningInt() volatile && { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile && {}
	int ReturningIntWithParameter( int, float, double ) volatile && { return 0; }
};

struct CallableMemberTestConstVolatileRValueReference
{
	void ReturningVoid() const volatile && {}
	int ReturningInt() const volatile && { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile && {}
	int ReturningIntWithParameter( int, float, double ) const volatile && { return 0; }
};

struct CallableMemberTestNoexcept
{
	void ReturningVoid() noexcept {}
	int ReturningInt() noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) noexcept {}
	int ReturningIntWithParameter( int, float, double ) noexcept { return 0; }
};

struct CallableMemberTestConstNoexcept
{
	void ReturningVoid() const noexcept {}
	int ReturningInt() const noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const noexcept {}
	int ReturningIntWithParameter( int, float, double ) const noexcept { return 0; }
};

struct CallableMemberTestVolatileNoexcept
{
	void ReturningVoid() volatile noexcept {}
	int ReturningInt() volatile noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile noexcept {}
	int ReturningIntWithParameter( int, float, double ) volatile noexcept { return 0; }
};

struct CallableMemberTestConstVolatileNoexcept
{
	void ReturningVoid() const volatile noexcept {}
	int ReturningInt() const volatile noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile  noexcept {}
	int ReturningIntWithParameter( int, float, double ) const volatile  noexcept { return 0; }
};

struct CallableMemberTestLValueReferenceNoexcept
{
	void ReturningVoid() & noexcept {}
	int ReturningInt() & noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) & noexcept {}
	int ReturningIntWithParameter( int, float, double ) & noexcept { return 0; }
};

struct CallableMemberTestConstLValueReferenceNoexcept
{
	void ReturningVoid() const & noexcept {}
	int ReturningInt() const & noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const & noexcept {}
	int ReturningIntWithParameter( int, float, double ) const & noexcept { return 0; }
};

struct CallableMemberTestVolatileLValueReferenceNoexcept
{
	void ReturningVoid() volatile & noexcept {}
	int ReturningInt() volatile & noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile & noexcept {}
	int ReturningIntWithParameter( int, float, double ) volatile & noexcept { return 0; }
};

struct CallableMemberTestConstVolatileLValueReferenceNoexcept
{
	void ReturningVoid() const volatile & noexcept {}
	int ReturningInt() const volatile & noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile & noexcept {}
	int ReturningIntWithParameter( int, float, double ) const volatile & noexcept { return 0; }
};

struct CallableMemberTestRValueReferenceNoexcept
{
	void ReturningVoid() && noexcept {}
	int ReturningInt() && noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) && noexcept {}
	int ReturningIntWithParameter( int, float, double ) && noexcept { return 0; }
};

struct CallableMemberTestConstRValueReferenceNoexcept
{
	void ReturningVoid() const && noexcept {}
	int ReturningInt() const && noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const && noexcept {}
	int ReturningIntWithParameter( int, float, double ) const && noexcept { return 0; }
};

struct CallableMemberTestVolatileRValueReferenceNoexcept
{
	void ReturningVoid() volatile && noexcept {}
	int ReturningInt() volatile && noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile && noexcept {}
	int ReturningIntWithParameter( int, float, double ) volatile && noexcept { return 0; }
};

struct CallableMemberTestConstVolatileRValueReferenceNoexcept
{
	void ReturningVoid() const volatile && noexcept {}
	int ReturningInt() const volatile && noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile && noexcept {}
	int ReturningIntWithParameter( int, float, double ) const volatile && noexcept { return 0; }
};

static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTest::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTest::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTest::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTest::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTest::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTest::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConst::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConst::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConst::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConst::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConst::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConst::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConst::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConst::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatile::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatile::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatile::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatile::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );


static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::ParameterCount() == 0 );
static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningInt )>::ParameterCount() == 0 );
static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningVoidWithParameter )>::ParameterCount() == 3 );
static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningIntWithParameter )>::ParameterCount() == 3 );

static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTest::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestConst::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConst::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConst::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConst::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConst::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConst::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConst::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatile::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatile::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestLValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReference::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReference::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReference::ReturningVoid )>::IsLValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReference::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReference::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReference::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReference::ReturningVoid )>::IsLValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReference::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReference::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReference::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReference::ReturningVoid )>::IsLValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReference::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsLValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoid )>::IsRValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoid )>::IsRValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoid )>::IsRValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsRValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( &CallableMemberTestNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestConstNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestVolatileNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == true );
static_assert( CallableTraits<decltype( &CallableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );


// Lambdas.
static_assert( std::is_same_v<typename CallableTraits<decltype( []() {} )>::ReturnType, void> );
static_assert( std::is_same_v<typename CallableTraits<decltype( []() { return 0; } )>::ReturnType, int> );
static_assert( std::is_same_v<typename CallableTraits<decltype( []( int, float, double ) {} )>::ReturnType, void> );
static_assert( std::is_same_v<typename CallableTraits<decltype( []( int, float, double ) { return 0; } )>::ReturnType, int> );

static_assert( std::is_same_v<typename CallableTraits<decltype( []() {} )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<typename CallableTraits<decltype( []() { return 0; } )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<typename CallableTraits<decltype( []( int, float, double ) {} )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<typename CallableTraits<decltype( []( int, float, double ) { return 0; } )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( CallableTraits<decltype( []() {} )>::IsPlainFunction() == false );
static_assert( CallableTraits<decltype( []() {} )>::IsMemberFunction() == false );
static_assert( CallableTraits<decltype( []() {} )>::IsConst() == true );
static_assert( CallableTraits<decltype( []() {} )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( []() {} )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( []() {} )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( []() {} )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( []() mutable {} )>::IsPlainFunction() == false );
static_assert( CallableTraits<decltype( []() mutable {} )>::IsMemberFunction() == false );
static_assert( CallableTraits<decltype( []() mutable {} )>::IsConst() == false );		// Note that mutable lambda is not const.
static_assert( CallableTraits<decltype( []() mutable {} )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( []() mutable {} )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( []() mutable {} )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( []() mutable {} )>::IsNoexcept() == false );

static_assert( CallableTraits<decltype( []() noexcept {} )>::IsPlainFunction() == false );
static_assert( CallableTraits<decltype( []() noexcept {} )>::IsMemberFunction() == false );
static_assert( CallableTraits<decltype( []() noexcept {} )>::IsConst() == true );
static_assert( CallableTraits<decltype( []() noexcept {} )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( []() noexcept {} )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( []() noexcept {} )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( []() noexcept {} )>::IsNoexcept() == true );

static_assert( CallableTraits<decltype( []() mutable noexcept {} )>::IsPlainFunction() == false );
static_assert( CallableTraits<decltype( []() mutable noexcept {} )>::IsMemberFunction() == false );
static_assert( CallableTraits<decltype( []() mutable noexcept {} )>::IsConst() == false );		// Note that mutable lambda is not const.
static_assert( CallableTraits<decltype( []() mutable noexcept {} )>::IsVolatile() == false );
static_assert( CallableTraits<decltype( []() mutable noexcept {} )>::IsLValueOnly() == false );
static_assert( CallableTraits<decltype( []() mutable noexcept {} )>::IsRValueOnly() == false );
static_assert( CallableTraits<decltype( []() mutable noexcept {} )>::IsNoexcept() == true );


// Callable objects.
struct CallableTestReturningVoid
{
	void operator()() {}
};
struct CallableTestReturningInt
{
	int operator()() { return 0; }
};
struct CallableTestReturningVoidWithParameter
{
	void operator()( int, float, double ) {}
};
struct CallableTestReturningIntWithParameter
{
	int operator()( int, float, double ) { return 0; }
};

struct CallableTestReturningVoid_Const
{
	void operator()() const {}
};
struct CallableTestReturningInt_Const
{
	int operator()() const { return 0; }
};
struct CallableTestReturningVoidWithParameter_Const
{
	void operator()( int, float, double ) const {}
};
struct CallableTestReturningIntWithParameter_Const
{
	int operator()( int, float, double ) const { return 0; }
};

struct CallableTestReturningVoid_Volatile
{
	void operator()() volatile {}
};
struct CallableTestReturningInt_Volatile
{
	int operator()() volatile { return 0; }
};
struct CallableTestReturningVoidWithParameter_Volatile
{
	void operator()( int, float, double ) volatile {}
};
struct CallableTestReturningIntWithParameter_Volatile
{
	int operator()( int, float, double ) volatile { return 0; }
};

struct CallableTestReturningVoid_ConstVolatile
{
	void operator()() const volatile {}
};
struct CallableTestReturningInt_ConstVolatile
{
	int operator()() const volatile { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstVolatile
{
	void operator()( int, float, double ) const volatile {}
};
struct CallableTestReturningIntWithParameter_ConstVolatile
{
	int operator()( int, float, double ) const volatile { return 0; }
};

struct CallableTestReturningVoid_LValueReference
{
	void operator()() & {}
};
struct CallableTestReturningInt_LValueReference
{
	int operator()() & { return 0; }
};
struct CallableTestReturningVoidWithParameter_LValueReference
{
	void operator()( int, float, double ) & {}
};
struct CallableTestReturningIntWithParameter_LValueReference
{
	int operator()( int, float, double ) & { return 0; }
};

struct CallableTestReturningVoid_ConstLValueReference
{
	void operator()() const & {}
};
struct CallableTestReturningInt_ConstLValueReference
{
	int operator()() const & { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstLValueReference
{
	void operator()( int, float, double ) const & {}
};
struct CallableTestReturningIntWithParameter_ConstLValueReference
{
	int operator()( int, float, double ) const & { return 0; }
};

struct CallableTestReturningVoid_VolatileLValueReference
{
	void operator()() volatile & {}
};
struct CallableTestReturningInt_VolatileLValueReference
{
	int operator()() volatile & { return 0; }
};
struct CallableTestReturningVoidWithParameter_VolatileLValueReference
{
	void operator()( int, float, double ) volatile & {}
};
struct CallableTestReturningIntWithParameter_VolatileLValueReference
{
	int operator()( int, float, double ) volatile & { return 0; }
};

struct CallableTestReturningVoid_ConstVolatileLValueReference
{
	void operator()() const volatile & {}
};
struct CallableTestReturningInt_ConstVolatileLValueReference
{
	int operator()() const volatile & { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstVolatileLValueReference
{
	void operator()( int, float, double ) const volatile & {}
};
struct CallableTestReturningIntWithParameter_ConstVolatileLValueReference
{
	int operator()( int, float, double ) const volatile & { return 0; }
};

struct CallableTestReturningVoid_RValueReference
{
	void operator()() && {}
};
struct CallableTestReturningInt_RValueReference
{
	int operator()() && { return 0; }
};
struct CallableTestReturningVoidWithParameter_RValueReference
{
	void operator()( int, float, double ) && {}
};
struct CallableTestReturningIntWithParameter_RValueReference
{
	int operator()( int, float, double ) && { return 0; }
};

struct CallableTestReturningVoid_ConstRValueReference
{
	void operator()() const && {}
};
struct CallableTestReturningInt_ConstRValueReference
{
	int operator()() const && { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstRValueReference
{
	void operator()( int, float, double ) const && {}
};
struct CallableTestReturningIntWithParameter_ConstRValueReference
{
	int operator()( int, float, double ) const && { return 0; }
};

struct CallableTestReturningVoid_VolatileRValueReference
{
	void operator()() volatile && {}
};
struct CallableTestReturningInt_VolatileRValueReference
{
	int operator()() volatile && { return 0; }
};
struct CallableTestReturningVoidWithParameter_VolatileRValueReference
{
	void operator()( int, float, double ) volatile && {}
};
struct CallableTestReturningIntWithParameter_VolatileRValueReference
{
	int operator()( int, float, double ) volatile && { return 0; }
};

struct CallableTestReturningVoid_ConstVolatileRValueReference
{
	void operator()() const volatile && {}
};
struct CallableTestReturningInt_ConstVolatileRValueReference
{
	int operator()() const volatile && { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstVolatileRValueReference
{
	void operator()( int, float, double ) const volatile && {}
};
struct CallableTestReturningIntWithParameter_ConstVolatileRValueReference
{
	int operator()( int, float, double ) const volatile && { return 0; }
};

struct CallableTestReturningVoid_Noexcept
{
	void operator()() noexcept {}
};
struct CallableTestReturningInt_Noexcept
{
	int operator()() noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_Noexcept
{
	void operator()( int, float, double ) noexcept {}
};
struct CallableTestReturningIntWithParameter_Noexcept
{
	int operator()( int, float, double ) noexcept { return 0; }
};

struct CallableTestReturningVoid_ConstNoexcept
{
	void operator()() const noexcept {}
};
struct CallableTestReturningInt_ConstNoexcept
{
	int operator()() const noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstNoexcept
{
	void operator()( int, float, double ) const noexcept {}
};
struct CallableTestReturningIntWithParameter_ConstNoexcept
{
	int operator()( int, float, double ) const noexcept { return 0; }
};

struct CallableTestReturningVoid_VolatileNoexcept
{
	void operator()() volatile noexcept {}
};
struct CallableTestReturningInt_VolatileNoexcept
{
	int operator()() volatile noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_VolatileNoexcept
{
	void operator()( int, float, double ) volatile noexcept {}
};
struct CallableTestReturningIntWithParameter_VolatileNoexcept
{
	int operator()( int, float, double ) volatile noexcept { return 0; }
};

struct CallableTestReturningVoid_ConstVolatileNoexcept
{
	void operator()() const volatile noexcept {}
};
struct CallableTestReturningInt_ConstVolatileNoexcept
{
	int operator()() const volatile noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstVolatileNoexcept
{
	void operator()( int, float, double ) const volatile noexcept {}
};
struct CallableTestReturningIntWithParameter_ConstVolatileNoexcept
{
	int operator()( int, float, double ) const volatile noexcept { return 0; }
};

struct CallableTestReturningVoid_LValueReferenceNoexcept
{
	void operator()() & noexcept {}
};
struct CallableTestReturningInt_LValueReferenceNoexcept
{
	int operator()() & noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_LValueReferenceNoexcept
{
	void operator()( int, float, double ) & noexcept {}
};
struct CallableTestReturningIntWithParameter_LValueReferenceNoexcept
{
	int operator()( int, float, double ) & noexcept { return 0; }
};

struct CallableTestReturningVoid_ConstLValueReferenceNoexcept
{
	void operator()() const & noexcept {}
};
struct CallableTestReturningInt_ConstLValueReferenceNoexcept
{
	int operator()() const & noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstLValueReferenceNoexcept
{
	void operator()( int, float, double ) const & noexcept {}
};
struct CallableTestReturningIntWithParameter_ConstLValueReferenceNoexcept
{
	int operator()( int, float, double ) const & noexcept { return 0; }
};

struct CallableTestReturningVoid_VolatileLValueReferenceNoexcept
{
	void operator()() volatile & noexcept {}
};
struct CallableTestReturningInt_VolatileLValueReferenceNoexcept
{
	int operator()() volatile & noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_VolatileLValueReferenceNoexcept
{
	void operator()( int, float, double ) volatile & noexcept {}
};
struct CallableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept
{
	int operator()( int, float, double ) volatile & noexcept { return 0; }
};

struct CallableTestReturningVoid_ConstVolatileLValueReferenceNoexcept
{
	void operator()() const volatile & noexcept {}
};
struct CallableTestReturningInt_ConstVolatileLValueReferenceNoexcept
{
	int operator()() const volatile & noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstVolatileLValueReferenceNoexcept
{
	void operator()( int, float, double ) const volatile & noexcept {}
};
struct CallableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept
{
	int operator()( int, float, double ) const volatile & noexcept { return 0; }
};

struct CallableTestReturningVoid_RValueReferenceNoexcept
{
	void operator()() && noexcept {}
};
struct CallableTestReturningInt_RValueReferenceNoexcept
{
	int operator()() && noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_RValueReferenceNoexcept
{
	void operator()( int, float, double ) && noexcept {}
};
struct CallableTestReturningIntWithParameter_RValueReferenceNoexcept
{
	int operator()( int, float, double ) && noexcept { return 0; }
};

struct CallableTestReturningVoid_ConstRValueReferenceNoexcept
{
	void operator()() const && noexcept {}
};
struct CallableTestReturningInt_ConstRValueReferenceNoexcept
{
	int operator()() const && noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstRValueReferenceNoexcept
{
	void operator()( int, float, double ) const && noexcept {}
};
struct CallableTestReturningIntWithParameter_ConstRValueReferenceNoexcept
{
	int operator()( int, float, double ) const && noexcept { return 0; }
};

struct CallableTestReturningVoid_VolatileRValueReferenceNoexcept
{
	void operator()() volatile && noexcept {}
};
struct CallableTestReturningInt_VolatileRValueReferenceNoexcept
{
	int operator()() volatile && noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_VolatileRValueReferenceNoexcept
{
	void operator()( int, float, double ) volatile && noexcept {}
};
struct CallableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept
{
	int operator()( int, float, double ) volatile && noexcept { return 0; }
};

struct CallableTestReturningVoid_ConstVolatileRValueReferenceNoexcept
{
	void operator()() const volatile && noexcept {}
};
struct CallableTestReturningInt_ConstVolatileRValueReferenceNoexcept
{
	int operator()() const volatile && noexcept { return 0; }
};
struct CallableTestReturningVoidWithParameter_ConstVolatileRValueReferenceNoexcept
{
	void operator()( int, float, double ) const volatile && noexcept {}
};
struct CallableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept
{
	int operator()( int, float, double ) const volatile && noexcept { return 0; }
};


static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_Const>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_Const>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_Const>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_Const>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_Const>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_Const>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_Const>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_Const>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_Volatile>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_Volatile>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_Volatile>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_Volatile>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_Volatile>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_Volatile>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_Volatile>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_Volatile>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstVolatile>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstVolatile>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstVolatile>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstVolatile>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstVolatile>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstVolatile>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstVolatile>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstVolatile>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_LValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_LValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_LValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_LValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_LValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_LValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_LValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_LValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstLValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_VolatileLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_VolatileLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_VolatileLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_VolatileLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_VolatileLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_VolatileLValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstVolatileLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstVolatileLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstVolatileLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstVolatileLValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_RValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_RValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_RValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_RValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_RValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_RValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_RValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_RValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstRValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_VolatileRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_VolatileRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_VolatileRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_VolatileRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_VolatileRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_VolatileRValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstVolatileRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstVolatileRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningInt_ConstVolatileRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningVoidWithParameter_ConstVolatileRValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReference>::ParameterTypeList, TypeList<int, float, double>> );


static_assert( CallableTraits<CallableTestReturningVoid>::ParameterCount() == 0 );
static_assert( CallableTraits<CallableTestReturningInt>::ParameterCount() == 0 );
static_assert( CallableTraits<CallableTestReturningVoidWithParameter>::ParameterCount() == 3 );
static_assert( CallableTraits<CallableTestReturningIntWithParameter>::ParameterCount() == 3 );

static_assert( CallableTraits<CallableTestReturningVoid>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_Const>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Const>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Const>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_Const>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Const>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Const>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Const>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_Volatile>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Volatile>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Volatile>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Volatile>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_Volatile>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Volatile>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Volatile>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatile>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatile>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatile>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatile>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatile>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatile>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatile>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_LValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReference>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReference>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReference>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReference>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReference>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReference>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReference>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReference>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReference>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReference>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReference>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReference>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReference>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReference>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReference>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReference>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_RValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReference>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReference>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReference>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReference>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReference>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReference>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReference>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReference>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReference>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReference>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReference>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReference>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReference>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReference>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReference>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReference>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningVoid_Noexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Noexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Noexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_Noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_ConstNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_VolatileNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_LValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReferenceNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReferenceNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_LValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReferenceNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReferenceNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_RValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReferenceNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReferenceNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_RValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReferenceNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReferenceNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstRValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsNoexcept() == true );


static_assert( CallableTraits<CallableTestReturningIntWithParameter>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_Const>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Const>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Const>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Const>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Const>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Const>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Const>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_Volatile>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Volatile>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Volatile>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Volatile>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Volatile>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Volatile>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Volatile>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatile>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatile>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatile>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatile>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatile>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatile>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatile>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReference>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReference>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReference>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReference>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReference>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReference>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReference>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReference>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReference>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReference>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReference>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReference>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReference>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReference>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReference>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReference>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReference>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReference>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReference>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReference>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReference>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReference>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReference>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReference>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsNoexcept() == false );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_Noexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Noexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Noexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Noexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Noexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Noexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_Noexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsVolatile() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsConst() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsConst() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsVolatile() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( CallableTraits<CallableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsNoexcept() == true );



} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // utility
} // bc
