#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/data_types/FundamentalTypes.hpp>
#include <core/utility/concepts/TypeTraitConcepts.hpp>
#include <core/utility/template/TypeList.hpp>



namespace bc {
namespace utility {
namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<
	bool IsPlainFunctionInvocable,
	bool IsMemberInvocable,
	bool IsConstInvocable,
	bool IsVolatileInvocable,
	bool IsLValueOnlyInvocable,
	bool IsRValueOnlyInvocable,
	bool IsNoexceptInvocable,
	typename InvocableReturnType,
	typename ...InvocableParameterTypePack
>
class InvocableTraitsBase
{
public:
	using Signature				= InvocableReturnType(InvocableParameterTypePack...);
	using ReturnType			= InvocableReturnType;
	using ParameterTypeList		= TypeList<InvocableParameterTypePack...>;

	static consteval auto ParameterCount() noexcept { return ParameterTypeList::Size(); }

	static consteval auto IsPlainFunction() { return IsPlainFunctionInvocable; }
	static consteval auto IsMemberFunction() { return IsMemberInvocable; }
	static consteval auto IsConst() { return IsConstInvocable; }
	static consteval auto IsVolatile() { return IsVolatileInvocable; }
	static consteval auto IsLValueOnly() { return IsLValueOnlyInvocable; }
	static consteval auto IsRValueOnly() { return IsRValueOnlyInvocable; }
	static consteval auto IsNoexcept() { return IsNoexceptInvocable; }
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Helper class to resolve the traits of a Invocable object.
///
/// Specializations below are used to resolve the traits of each Invocable object operator().
///
/// @tparam Type
/// Unused.
template<typename Type>
class InvocableObjectResolver
{
	// Disabled default error condition because it may be useful to use this class as a test in SFINAE, eg. in deduction guides.
	//static constexpr bool is_Invocable = []<bool b = false>(){
	//	static_assert( b, "Cannot get a Invocable traits from a non Invocable type." );
	//	return b;
	//}();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... )>
	: public InvocableTraitsBase<false, false, false, false, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const>
	: public InvocableTraitsBase<false, false, true, false, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) volatile>
	: public InvocableTraitsBase<false, false, false, true, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const volatile>
	: public InvocableTraitsBase<false, false, true, true, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) &>
	: public InvocableTraitsBase<false, false, false, false, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const &>
	: public InvocableTraitsBase<false, false, true, false, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) volatile &>
	: public InvocableTraitsBase<false, false, false, true, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const volatile &>
	: public InvocableTraitsBase<false, false, true, true, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) &&>
	: public InvocableTraitsBase<false, false, false, false, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const &&>
	: public InvocableTraitsBase<false, false, true, false, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) volatile &&>
	: public InvocableTraitsBase<false, false, false, true, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const volatile &&>
	: public InvocableTraitsBase<false, false, true, true, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) noexcept>
	: public InvocableTraitsBase<false, false, false, false, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const noexcept>
	: public InvocableTraitsBase<false, false, true, false, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) volatile noexcept>
	: public InvocableTraitsBase<false, false, false, true, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const volatile noexcept>
	: public InvocableTraitsBase<false, false, true, true, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) & noexcept>
	: public InvocableTraitsBase<false, false, false, false, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const & noexcept>
	: public InvocableTraitsBase<false, false, true, false, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) volatile & noexcept>
	: public InvocableTraitsBase<false, false, false, true, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const volatile & noexcept>
	: public InvocableTraitsBase<false, false, true, true, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) && noexcept>
	: public InvocableTraitsBase<false, false, false, false, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const && noexcept>
	: public InvocableTraitsBase<false, false, true, false, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) volatile && noexcept>
	: public InvocableTraitsBase<false, false, false, true, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableObjectResolver<InvocableReturnType( InvocableClassType::* )( InvocableParameterTypePack... ) const volatile && noexcept>
	: public InvocableTraitsBase<false, false, true, true, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

} // internal_



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Collects information about a Invocable object.
///
/// This class is useful when trying to extract information from a Invocable object.
///
/// @tparam InvocableType
/// Invocable object type.
template<typename InvocableType>
class InvocableTraits
{
	// Disabled default error condition because it may be useful to use this class as a test in SFINAE, eg. in deduction guides.
	//static constexpr bool is_Invocable = []<bool b = false>(){
	//	static_assert( b, "Not a Invocable type" );
	//	return b;
	//}();
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<InvocableObject InvocableType>
class InvocableTraits<InvocableType> :
	public internal_::InvocableObjectResolver<decltype( &InvocableType::operator() )>
{};



// Plain functions.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...)> :
	public internal_::InvocableTraitsBase<true, false, false, false, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const> :
	public internal_::InvocableTraitsBase<true, false, true, false, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) volatile> :
	public internal_::InvocableTraitsBase<true, false, false, true, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const volatile> :
	public internal_::InvocableTraitsBase<true, false, true, true, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) &> :
	public internal_::InvocableTraitsBase<true, false, false, false, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const &> :
	public internal_::InvocableTraitsBase<true, false, true, false, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) volatile &> :
	public internal_::InvocableTraitsBase<true, false, false, true, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const volatile &> :
	public internal_::InvocableTraitsBase<true, false, true, true, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) &&> :
	public internal_::InvocableTraitsBase<true, false, false, false, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const &&> :
	public internal_::InvocableTraitsBase<true, false, true, false, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) volatile &&> :
	public internal_::InvocableTraitsBase<true, false, false, true, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const volatile &&> :
	public internal_::InvocableTraitsBase<true, false, true, true, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) noexcept> :
	public internal_::InvocableTraitsBase<true, false, false, false, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const noexcept> :
	public internal_::InvocableTraitsBase<true, false, true, false, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) volatile noexcept> :
	public internal_::InvocableTraitsBase<true, false, false, true, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const volatile noexcept> :
	public internal_::InvocableTraitsBase<true, false, true, true, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) & noexcept> :
	public internal_::InvocableTraitsBase<true, false, false, false, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const & noexcept> :
	public internal_::InvocableTraitsBase<true, false, true, false, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) volatile & noexcept> :
	public internal_::InvocableTraitsBase<true, false, false, true, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const volatile & noexcept> :
	public internal_::InvocableTraitsBase<true, false, true, true, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) && noexcept> :
	public internal_::InvocableTraitsBase<true, false, false, false, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const && noexcept> :
	public internal_::InvocableTraitsBase<true, false, true, false, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) volatile && noexcept> :
	public internal_::InvocableTraitsBase<true, false, false, true, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType(InvocableParameterTypePack...) const volatile && noexcept> :
	public internal_::InvocableTraitsBase<true, false, true, true, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};



// Member functions.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... )> :
	public internal_::InvocableTraitsBase<true, true, false, false, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const> :
	public internal_::InvocableTraitsBase<true, true, true, false, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) volatile> :
	public internal_::InvocableTraitsBase<true, true, false, true, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const volatile> :
	public internal_::InvocableTraitsBase<true, true, true, true, false, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) &> :
	public internal_::InvocableTraitsBase<true, true, false, false, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const &> :
	public internal_::InvocableTraitsBase<true, true, true, false, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) volatile &> :
	public internal_::InvocableTraitsBase<true, true, false, true, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const volatile&> :
	public internal_::InvocableTraitsBase<true, true, true, true, true, false, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) &&> :
	public internal_::InvocableTraitsBase<true, true, false, false, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const &&> :
	public internal_::InvocableTraitsBase<true, true, true, false, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) volatile &&> :
	public internal_::InvocableTraitsBase<true, true, false, true, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const volatile &&> :
	public internal_::InvocableTraitsBase<true, true, true, true, false, true, false, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) noexcept> :
	public internal_::InvocableTraitsBase<true, true, false, false, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const noexcept> :
	public internal_::InvocableTraitsBase<true, true, true, false, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) volatile noexcept> :
	public internal_::InvocableTraitsBase<true, true, false, true, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const volatile noexcept> :
	public internal_::InvocableTraitsBase<true, true, true, true, false, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) & noexcept> :
	public internal_::InvocableTraitsBase<true, true, false, false, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const & noexcept> :
	public internal_::InvocableTraitsBase<true, true, true, false, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) volatile & noexcept> :
	public internal_::InvocableTraitsBase<true, true, false, true, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const volatile & noexcept> :
	public internal_::InvocableTraitsBase<true, true, true, true, true, false, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) && noexcept> :
	public internal_::InvocableTraitsBase<true, true, false, false, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const && noexcept> :
	public internal_::InvocableTraitsBase<true, true, true, false, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) volatile && noexcept> :
	public internal_::InvocableTraitsBase<true, true, false, true, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename ClassType, typename InvocableReturnType, typename ...InvocableParameterTypePack>
class InvocableTraits<InvocableReturnType( ClassType::* )( InvocableParameterTypePack... ) const volatile && noexcept> :
	public internal_::InvocableTraitsBase<true, true, true, true, false, true, true, InvocableReturnType, InvocableParameterTypePack...>
{};



#ifdef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
namespace tests {

// Basic functions.
static_assert( sizeof( InvocableTraits<void()> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double )> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) volatile> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const volatile> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) &> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const &> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) volatile &> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const volatile &> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) &&> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const &&> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) volatile &&> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const volatile &&> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) volatile noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const volatile noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) & noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const & noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) volatile & noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const volatile & noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) && noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const && noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) volatile && noexcept> ) == 1 );
static_assert( sizeof( InvocableTraits<void( int, float, double ) const volatile && noexcept> ) == 1 );

static_assert( std::is_same_v<InvocableTraits<void()>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<int()>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<int( int, float, double )>::ReturnType, int> );

static_assert( std::is_same_v<InvocableTraits<void()>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) volatile>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const volatile>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) &>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const &>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) volatile &>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const volatile &>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) &&>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const &&>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) volatile &&>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const volatile &&>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) volatile noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const volatile noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) & noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const & noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) volatile & noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const volatile & noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) && noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const && noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) volatile && noexcept>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<void( int, float, double ) const volatile && noexcept>::ParameterTypeList, TypeList<int, float, double>> );


static_assert( InvocableTraits<void()>::ParameterCount() == 0 );
static_assert( InvocableTraits<void( int, float, double )>::ParameterCount() == 3 );

static_assert( InvocableTraits<void()>::IsPlainFunction() == true );
static_assert( InvocableTraits<void()>::IsMemberFunction() == false );
static_assert( InvocableTraits<void()>::IsConst() == false );
static_assert( InvocableTraits<void()>::IsVolatile() == false );
static_assert( InvocableTraits<void()>::IsLValueOnly() == false );
static_assert( InvocableTraits<void()>::IsRValueOnly() == false );
static_assert( InvocableTraits<void()>::IsNoexcept() == false );

static_assert( InvocableTraits<void() const>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const>::IsConst() == true );
static_assert( InvocableTraits<void() const>::IsVolatile() == false );
static_assert( InvocableTraits<void() const>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() const>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() const>::IsNoexcept() == false );

static_assert( InvocableTraits<void() volatile>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() volatile>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() volatile>::IsConst() == false );
static_assert( InvocableTraits<void() volatile>::IsVolatile() == true );
static_assert( InvocableTraits<void() volatile>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() volatile>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() volatile>::IsNoexcept() == false );

static_assert( InvocableTraits<void() const volatile>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const volatile>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const volatile>::IsConst() == true );
static_assert( InvocableTraits<void() const volatile>::IsVolatile() == true );
static_assert( InvocableTraits<void() const volatile>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() const volatile>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() const volatile>::IsNoexcept() == false );

static_assert( InvocableTraits<void() &>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() &>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() &>::IsConst() == false );
static_assert( InvocableTraits<void() &>::IsVolatile() == false );
static_assert( InvocableTraits<void() &>::IsLValueOnly() == true );
static_assert( InvocableTraits<void() &>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() &>::IsNoexcept() == false );

static_assert( InvocableTraits<void() const &>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const &>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const &>::IsConst() == true );
static_assert( InvocableTraits<void() const &>::IsVolatile() == false );
static_assert( InvocableTraits<void() const &>::IsLValueOnly() == true );
static_assert( InvocableTraits<void() const &>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() const &>::IsNoexcept() == false );

static_assert( InvocableTraits<void() volatile &>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() volatile &>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() volatile &>::IsConst() == false );
static_assert( InvocableTraits<void() volatile &>::IsVolatile() == true );
static_assert( InvocableTraits<void() volatile &>::IsLValueOnly() == true );
static_assert( InvocableTraits<void() volatile &>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() volatile &>::IsNoexcept() == false );

static_assert( InvocableTraits<void() const volatile &>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const volatile &>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const volatile &>::IsConst() == true );
static_assert( InvocableTraits<void() const volatile &>::IsVolatile() == true );
static_assert( InvocableTraits<void() const volatile &>::IsLValueOnly() == true );
static_assert( InvocableTraits<void() const volatile &>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() const volatile &>::IsNoexcept() == false );

static_assert( InvocableTraits<void() &&>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() &&>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() &&>::IsConst() == false );
static_assert( InvocableTraits<void() &&>::IsVolatile() == false );
static_assert( InvocableTraits<void() &&>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() &&>::IsRValueOnly() == true );
static_assert( InvocableTraits<void() &&>::IsNoexcept() == false );

static_assert( InvocableTraits<void() const &&>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const &&>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const &&>::IsConst() == true );
static_assert( InvocableTraits<void() const &&>::IsVolatile() == false );
static_assert( InvocableTraits<void() const &&>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() const &&>::IsRValueOnly() == true );
static_assert( InvocableTraits<void() const &&>::IsNoexcept() == false );

static_assert( InvocableTraits<void() volatile &&>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() volatile &&>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() volatile &&>::IsConst() == false );
static_assert( InvocableTraits<void() volatile &&>::IsVolatile() == true );
static_assert( InvocableTraits<void() volatile &&>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() volatile &&>::IsRValueOnly() == true );
static_assert( InvocableTraits<void() volatile &&>::IsNoexcept() == false );

static_assert( InvocableTraits<void() const volatile &&>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const volatile &&>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const volatile &&>::IsConst() == true );
static_assert( InvocableTraits<void() const volatile &&>::IsVolatile() == true );
static_assert( InvocableTraits<void() const volatile &&>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() const volatile &&>::IsRValueOnly() == true );
static_assert( InvocableTraits<void() const volatile &&>::IsNoexcept() == false );

static_assert( InvocableTraits<void() noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() noexcept>::IsConst() == false );
static_assert( InvocableTraits<void() noexcept>::IsVolatile() == false );
static_assert( InvocableTraits<void() noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() const noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const noexcept>::IsConst() == true );
static_assert( InvocableTraits<void() const noexcept>::IsVolatile() == false );
static_assert( InvocableTraits<void() const noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() const noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() const noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() volatile noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() volatile noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() volatile noexcept>::IsConst() == false );
static_assert( InvocableTraits<void() volatile noexcept>::IsVolatile() == true );
static_assert( InvocableTraits<void() volatile noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() volatile noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() volatile noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() const volatile noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const volatile noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const volatile noexcept>::IsConst() == true );
static_assert( InvocableTraits<void() const volatile noexcept>::IsVolatile() == true );
static_assert( InvocableTraits<void() const volatile noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() const volatile noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() const volatile noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() & noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() & noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() & noexcept>::IsConst() == false );
static_assert( InvocableTraits<void() & noexcept>::IsVolatile() == false );
static_assert( InvocableTraits<void() & noexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<void() & noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() & noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() const & noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const & noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const & noexcept>::IsConst() == true );
static_assert( InvocableTraits<void() const & noexcept>::IsVolatile() == false );
static_assert( InvocableTraits<void() const & noexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<void() const & noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() const & noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() volatile & noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() volatile & noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() volatile & noexcept>::IsConst() == false );
static_assert( InvocableTraits<void() volatile & noexcept>::IsVolatile() == true );
static_assert( InvocableTraits<void() volatile & noexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<void() volatile & noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() volatile & noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() const volatile & noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const volatile & noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const volatile & noexcept>::IsConst() == true );
static_assert( InvocableTraits<void() const volatile & noexcept>::IsVolatile() == true );
static_assert( InvocableTraits<void() const volatile & noexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<void() const volatile & noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<void() const volatile & noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() && noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() && noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() && noexcept>::IsConst() == false );
static_assert( InvocableTraits<void() && noexcept>::IsVolatile() == false );
static_assert( InvocableTraits<void() && noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() && noexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<void() && noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() const && noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const && noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const && noexcept>::IsConst() == true );
static_assert( InvocableTraits<void() const && noexcept>::IsVolatile() == false );
static_assert( InvocableTraits<void() const && noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() const && noexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<void() const && noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() volatile && noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() volatile && noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() volatile && noexcept>::IsConst() == false );
static_assert( InvocableTraits<void() volatile && noexcept>::IsVolatile() == true );
static_assert( InvocableTraits<void() volatile && noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() volatile && noexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<void() volatile && noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<void() const volatile && noexcept>::IsPlainFunction() == true );
static_assert( InvocableTraits<void() const volatile && noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<void() const volatile && noexcept>::IsConst() == true );
static_assert( InvocableTraits<void() const volatile && noexcept>::IsVolatile() == true );
static_assert( InvocableTraits<void() const volatile && noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<void() const volatile && noexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<void() const volatile && noexcept>::IsNoexcept() == true );


// Member functions.
struct InvocableMemberTest
{
	void ReturningVoid() {}
	int ReturningInt() { return 0; }
	void ReturningVoidWithParameter( int, float, double ) {}
	int ReturningIntWithParameter( int, float, double ) { return 0; }
};

struct InvocableMemberTestConst
{
	void ReturningVoid() const {}
	int ReturningInt() const { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const {}
	int ReturningIntWithParameter( int, float, double ) const { return 0; }
};

struct InvocableMemberTestVolatile
{
	void ReturningVoid() volatile {}
	int ReturningInt() volatile { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile {}
	int ReturningIntWithParameter( int, float, double ) volatile { return 0; }
};

struct InvocableMemberTestConstVolatile
{
	void ReturningVoid() const volatile {}
	int ReturningInt() const volatile { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile {}
	int ReturningIntWithParameter( int, float, double ) const volatile { return 0; }
};

struct InvocableMemberTestLValueReference
{
	void ReturningVoid() & {}
	int ReturningInt() & { return 0; }
	void ReturningVoidWithParameter( int, float, double ) & {}
	int ReturningIntWithParameter( int, float, double ) & { return 0; }
};

struct InvocableMemberTestConstLValueReference
{
	void ReturningVoid() const & {}
	int ReturningInt() const & { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const & {}
	int ReturningIntWithParameter( int, float, double ) const & { return 0; }
};

struct InvocableMemberTestVolatileLValueReference
{
	void ReturningVoid() volatile & {}
	int ReturningInt() volatile & { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile & {}
	int ReturningIntWithParameter( int, float, double ) volatile & { return 0; }
};

struct InvocableMemberTestConstVolatileLValueReference
{
	void ReturningVoid() const volatile & {}
	int ReturningInt() const volatile & { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile & {}
	int ReturningIntWithParameter( int, float, double ) const volatile & { return 0; }
};

struct InvocableMemberTestRValueReference
{
	void ReturningVoid() && {}
	int ReturningInt() && { return 0; }
	void ReturningVoidWithParameter( int, float, double ) && {}
	int ReturningIntWithParameter( int, float, double ) && { return 0; }
};

struct InvocableMemberTestConstRValueReference
{
	void ReturningVoid() const && {}
	int ReturningInt() const && { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const && {}
	int ReturningIntWithParameter( int, float, double ) const && { return 0; }
};

struct InvocableMemberTestVolatileRValueReference
{
	void ReturningVoid() volatile && {}
	int ReturningInt() volatile && { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile && {}
	int ReturningIntWithParameter( int, float, double ) volatile && { return 0; }
};

struct InvocableMemberTestConstVolatileRValueReference
{
	void ReturningVoid() const volatile && {}
	int ReturningInt() const volatile && { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile && {}
	int ReturningIntWithParameter( int, float, double ) const volatile && { return 0; }
};

struct InvocableMemberTestNoexcept
{
	void ReturningVoid() noexcept {}
	int ReturningInt() noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) noexcept {}
	int ReturningIntWithParameter( int, float, double ) noexcept { return 0; }
};

struct InvocableMemberTestConstNoexcept
{
	void ReturningVoid() const noexcept {}
	int ReturningInt() const noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const noexcept {}
	int ReturningIntWithParameter( int, float, double ) const noexcept { return 0; }
};

struct InvocableMemberTestVolatileNoexcept
{
	void ReturningVoid() volatile noexcept {}
	int ReturningInt() volatile noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile noexcept {}
	int ReturningIntWithParameter( int, float, double ) volatile noexcept { return 0; }
};

struct InvocableMemberTestConstVolatileNoexcept
{
	void ReturningVoid() const volatile noexcept {}
	int ReturningInt() const volatile noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile  noexcept {}
	int ReturningIntWithParameter( int, float, double ) const volatile  noexcept { return 0; }
};

struct InvocableMemberTestLValueReferenceNoexcept
{
	void ReturningVoid() & noexcept {}
	int ReturningInt() & noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) & noexcept {}
	int ReturningIntWithParameter( int, float, double ) & noexcept { return 0; }
};

struct InvocableMemberTestConstLValueReferenceNoexcept
{
	void ReturningVoid() const & noexcept {}
	int ReturningInt() const & noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const & noexcept {}
	int ReturningIntWithParameter( int, float, double ) const & noexcept { return 0; }
};

struct InvocableMemberTestVolatileLValueReferenceNoexcept
{
	void ReturningVoid() volatile & noexcept {}
	int ReturningInt() volatile & noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile & noexcept {}
	int ReturningIntWithParameter( int, float, double ) volatile & noexcept { return 0; }
};

struct InvocableMemberTestConstVolatileLValueReferenceNoexcept
{
	void ReturningVoid() const volatile & noexcept {}
	int ReturningInt() const volatile & noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile & noexcept {}
	int ReturningIntWithParameter( int, float, double ) const volatile & noexcept { return 0; }
};

struct InvocableMemberTestRValueReferenceNoexcept
{
	void ReturningVoid() && noexcept {}
	int ReturningInt() && noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) && noexcept {}
	int ReturningIntWithParameter( int, float, double ) && noexcept { return 0; }
};

struct InvocableMemberTestConstRValueReferenceNoexcept
{
	void ReturningVoid() const && noexcept {}
	int ReturningInt() const && noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const && noexcept {}
	int ReturningIntWithParameter( int, float, double ) const && noexcept { return 0; }
};

struct InvocableMemberTestVolatileRValueReferenceNoexcept
{
	void ReturningVoid() volatile && noexcept {}
	int ReturningInt() volatile && noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) volatile && noexcept {}
	int ReturningIntWithParameter( int, float, double ) volatile && noexcept { return 0; }
};

struct InvocableMemberTestConstVolatileRValueReferenceNoexcept
{
	void ReturningVoid() const volatile && noexcept {}
	int ReturningInt() const volatile && noexcept { return 0; }
	void ReturningVoidWithParameter( int, float, double ) const volatile && noexcept {}
	int ReturningIntWithParameter( int, float, double ) const volatile && noexcept { return 0; }
};

static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTest::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTest::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTest::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTest::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTest::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTest::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConst::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConst::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConst::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConst::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoid )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningInt )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoidWithParameter )>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningIntWithParameter )>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoid )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningInt )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoidWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningIntWithParameter )>::ParameterTypeList, TypeList<int, float, double>> );


static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::ParameterCount() == 0 );
static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningInt )>::ParameterCount() == 0 );
static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningVoidWithParameter )>::ParameterCount() == 3 );
static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningIntWithParameter )>::ParameterCount() == 3 );

static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTest::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConst::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatile::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatile::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReference::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReference::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReference::ReturningVoid )>::IsLValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReference::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReference::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReference::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReference::ReturningVoid )>::IsLValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReference::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReference::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReference::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReference::ReturningVoid )>::IsLValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReference::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsLValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoid )>::IsRValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoid )>::IsRValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoid )>::IsRValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsRValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReference::ReturningVoid )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( &InvocableMemberTestNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestLValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstLValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileLValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileLValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestRValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstRValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsConst() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestVolatileRValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsPlainFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsMemberFunction() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsConst() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsVolatile() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsRValueOnly() == true );
static_assert( InvocableTraits<decltype( &InvocableMemberTestConstVolatileRValueReferenceNoexcept::ReturningVoid )>::IsNoexcept() == true );


// Lambdas.
static_assert( std::is_same_v<typename InvocableTraits<decltype( []() {} )>::ReturnType, void> );
static_assert( std::is_same_v<typename InvocableTraits<decltype( []() { return 0; } )>::ReturnType, int> );
static_assert( std::is_same_v<typename InvocableTraits<decltype( []( int, float, double ) {} )>::ReturnType, void> );
static_assert( std::is_same_v<typename InvocableTraits<decltype( []( int, float, double ) { return 0; } )>::ReturnType, int> );

static_assert( std::is_same_v<typename InvocableTraits<decltype( []() {} )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<typename InvocableTraits<decltype( []() { return 0; } )>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<typename InvocableTraits<decltype( []( int, float, double ) {} )>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<typename InvocableTraits<decltype( []( int, float, double ) { return 0; } )>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( InvocableTraits<decltype( []() {} )>::IsPlainFunction() == false );
static_assert( InvocableTraits<decltype( []() {} )>::IsMemberFunction() == false );
static_assert( InvocableTraits<decltype( []() {} )>::IsConst() == true );
static_assert( InvocableTraits<decltype( []() {} )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( []() {} )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( []() {} )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( []() {} )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( []() mutable {} )>::IsPlainFunction() == false );
static_assert( InvocableTraits<decltype( []() mutable {} )>::IsMemberFunction() == false );
static_assert( InvocableTraits<decltype( []() mutable {} )>::IsConst() == false );		// Note that mutable lambda is not const.
static_assert( InvocableTraits<decltype( []() mutable {} )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( []() mutable {} )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( []() mutable {} )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( []() mutable {} )>::IsNoexcept() == false );

static_assert( InvocableTraits<decltype( []() noexcept {} )>::IsPlainFunction() == false );
static_assert( InvocableTraits<decltype( []() noexcept {} )>::IsMemberFunction() == false );
static_assert( InvocableTraits<decltype( []() noexcept {} )>::IsConst() == true );
static_assert( InvocableTraits<decltype( []() noexcept {} )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( []() noexcept {} )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( []() noexcept {} )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( []() noexcept {} )>::IsNoexcept() == true );

static_assert( InvocableTraits<decltype( []() mutable noexcept {} )>::IsPlainFunction() == false );
static_assert( InvocableTraits<decltype( []() mutable noexcept {} )>::IsMemberFunction() == false );
static_assert( InvocableTraits<decltype( []() mutable noexcept {} )>::IsConst() == false );		// Note that mutable lambda is not const.
static_assert( InvocableTraits<decltype( []() mutable noexcept {} )>::IsVolatile() == false );
static_assert( InvocableTraits<decltype( []() mutable noexcept {} )>::IsLValueOnly() == false );
static_assert( InvocableTraits<decltype( []() mutable noexcept {} )>::IsRValueOnly() == false );
static_assert( InvocableTraits<decltype( []() mutable noexcept {} )>::IsNoexcept() == true );


// Invocable objects.
struct InvocableTestReturningVoid
{
	void operator()() {}
};
struct InvocableTestReturningInt
{
	int operator()() { return 0; }
};
struct InvocableTestReturningVoidWithParameter
{
	void operator()( int, float, double ) {}
};
struct InvocableTestReturningIntWithParameter
{
	int operator()( int, float, double ) { return 0; }
};

struct InvocableTestReturningVoid_Const
{
	void operator()() const {}
};
struct InvocableTestReturningInt_Const
{
	int operator()() const { return 0; }
};
struct InvocableTestReturningVoidWithParameter_Const
{
	void operator()( int, float, double ) const {}
};
struct InvocableTestReturningIntWithParameter_Const
{
	int operator()( int, float, double ) const { return 0; }
};

struct InvocableTestReturningVoid_Volatile
{
	void operator()() volatile {}
};
struct InvocableTestReturningInt_Volatile
{
	int operator()() volatile { return 0; }
};
struct InvocableTestReturningVoidWithParameter_Volatile
{
	void operator()( int, float, double ) volatile {}
};
struct InvocableTestReturningIntWithParameter_Volatile
{
	int operator()( int, float, double ) volatile { return 0; }
};

struct InvocableTestReturningVoid_ConstVolatile
{
	void operator()() const volatile {}
};
struct InvocableTestReturningInt_ConstVolatile
{
	int operator()() const volatile { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstVolatile
{
	void operator()( int, float, double ) const volatile {}
};
struct InvocableTestReturningIntWithParameter_ConstVolatile
{
	int operator()( int, float, double ) const volatile { return 0; }
};

struct InvocableTestReturningVoid_LValueReference
{
	void operator()() & {}
};
struct InvocableTestReturningInt_LValueReference
{
	int operator()() & { return 0; }
};
struct InvocableTestReturningVoidWithParameter_LValueReference
{
	void operator()( int, float, double ) & {}
};
struct InvocableTestReturningIntWithParameter_LValueReference
{
	int operator()( int, float, double ) & { return 0; }
};

struct InvocableTestReturningVoid_ConstLValueReference
{
	void operator()() const & {}
};
struct InvocableTestReturningInt_ConstLValueReference
{
	int operator()() const & { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstLValueReference
{
	void operator()( int, float, double ) const & {}
};
struct InvocableTestReturningIntWithParameter_ConstLValueReference
{
	int operator()( int, float, double ) const & { return 0; }
};

struct InvocableTestReturningVoid_VolatileLValueReference
{
	void operator()() volatile & {}
};
struct InvocableTestReturningInt_VolatileLValueReference
{
	int operator()() volatile & { return 0; }
};
struct InvocableTestReturningVoidWithParameter_VolatileLValueReference
{
	void operator()( int, float, double ) volatile & {}
};
struct InvocableTestReturningIntWithParameter_VolatileLValueReference
{
	int operator()( int, float, double ) volatile & { return 0; }
};

struct InvocableTestReturningVoid_ConstVolatileLValueReference
{
	void operator()() const volatile & {}
};
struct InvocableTestReturningInt_ConstVolatileLValueReference
{
	int operator()() const volatile & { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstVolatileLValueReference
{
	void operator()( int, float, double ) const volatile & {}
};
struct InvocableTestReturningIntWithParameter_ConstVolatileLValueReference
{
	int operator()( int, float, double ) const volatile & { return 0; }
};

struct InvocableTestReturningVoid_RValueReference
{
	void operator()() && {}
};
struct InvocableTestReturningInt_RValueReference
{
	int operator()() && { return 0; }
};
struct InvocableTestReturningVoidWithParameter_RValueReference
{
	void operator()( int, float, double ) && {}
};
struct InvocableTestReturningIntWithParameter_RValueReference
{
	int operator()( int, float, double ) && { return 0; }
};

struct InvocableTestReturningVoid_ConstRValueReference
{
	void operator()() const && {}
};
struct InvocableTestReturningInt_ConstRValueReference
{
	int operator()() const && { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstRValueReference
{
	void operator()( int, float, double ) const && {}
};
struct InvocableTestReturningIntWithParameter_ConstRValueReference
{
	int operator()( int, float, double ) const && { return 0; }
};

struct InvocableTestReturningVoid_VolatileRValueReference
{
	void operator()() volatile && {}
};
struct InvocableTestReturningInt_VolatileRValueReference
{
	int operator()() volatile && { return 0; }
};
struct InvocableTestReturningVoidWithParameter_VolatileRValueReference
{
	void operator()( int, float, double ) volatile && {}
};
struct InvocableTestReturningIntWithParameter_VolatileRValueReference
{
	int operator()( int, float, double ) volatile && { return 0; }
};

struct InvocableTestReturningVoid_ConstVolatileRValueReference
{
	void operator()() const volatile && {}
};
struct InvocableTestReturningInt_ConstVolatileRValueReference
{
	int operator()() const volatile && { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstVolatileRValueReference
{
	void operator()( int, float, double ) const volatile && {}
};
struct InvocableTestReturningIntWithParameter_ConstVolatileRValueReference
{
	int operator()( int, float, double ) const volatile && { return 0; }
};

struct InvocableTestReturningVoid_Noexcept
{
	void operator()() noexcept {}
};
struct InvocableTestReturningInt_Noexcept
{
	int operator()() noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_Noexcept
{
	void operator()( int, float, double ) noexcept {}
};
struct InvocableTestReturningIntWithParameter_Noexcept
{
	int operator()( int, float, double ) noexcept { return 0; }
};

struct InvocableTestReturningVoid_ConstNoexcept
{
	void operator()() const noexcept {}
};
struct InvocableTestReturningInt_ConstNoexcept
{
	int operator()() const noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstNoexcept
{
	void operator()( int, float, double ) const noexcept {}
};
struct InvocableTestReturningIntWithParameter_ConstNoexcept
{
	int operator()( int, float, double ) const noexcept { return 0; }
};

struct InvocableTestReturningVoid_VolatileNoexcept
{
	void operator()() volatile noexcept {}
};
struct InvocableTestReturningInt_VolatileNoexcept
{
	int operator()() volatile noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_VolatileNoexcept
{
	void operator()( int, float, double ) volatile noexcept {}
};
struct InvocableTestReturningIntWithParameter_VolatileNoexcept
{
	int operator()( int, float, double ) volatile noexcept { return 0; }
};

struct InvocableTestReturningVoid_ConstVolatileNoexcept
{
	void operator()() const volatile noexcept {}
};
struct InvocableTestReturningInt_ConstVolatileNoexcept
{
	int operator()() const volatile noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstVolatileNoexcept
{
	void operator()( int, float, double ) const volatile noexcept {}
};
struct InvocableTestReturningIntWithParameter_ConstVolatileNoexcept
{
	int operator()( int, float, double ) const volatile noexcept { return 0; }
};

struct InvocableTestReturningVoid_LValueReferenceNoexcept
{
	void operator()() & noexcept {}
};
struct InvocableTestReturningInt_LValueReferenceNoexcept
{
	int operator()() & noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_LValueReferenceNoexcept
{
	void operator()( int, float, double ) & noexcept {}
};
struct InvocableTestReturningIntWithParameter_LValueReferenceNoexcept
{
	int operator()( int, float, double ) & noexcept { return 0; }
};

struct InvocableTestReturningVoid_ConstLValueReferenceNoexcept
{
	void operator()() const & noexcept {}
};
struct InvocableTestReturningInt_ConstLValueReferenceNoexcept
{
	int operator()() const & noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstLValueReferenceNoexcept
{
	void operator()( int, float, double ) const & noexcept {}
};
struct InvocableTestReturningIntWithParameter_ConstLValueReferenceNoexcept
{
	int operator()( int, float, double ) const & noexcept { return 0; }
};

struct InvocableTestReturningVoid_VolatileLValueReferenceNoexcept
{
	void operator()() volatile & noexcept {}
};
struct InvocableTestReturningInt_VolatileLValueReferenceNoexcept
{
	int operator()() volatile & noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_VolatileLValueReferenceNoexcept
{
	void operator()( int, float, double ) volatile & noexcept {}
};
struct InvocableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept
{
	int operator()( int, float, double ) volatile & noexcept { return 0; }
};

struct InvocableTestReturningVoid_ConstVolatileLValueReferenceNoexcept
{
	void operator()() const volatile & noexcept {}
};
struct InvocableTestReturningInt_ConstVolatileLValueReferenceNoexcept
{
	int operator()() const volatile & noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstVolatileLValueReferenceNoexcept
{
	void operator()( int, float, double ) const volatile & noexcept {}
};
struct InvocableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept
{
	int operator()( int, float, double ) const volatile & noexcept { return 0; }
};

struct InvocableTestReturningVoid_RValueReferenceNoexcept
{
	void operator()() && noexcept {}
};
struct InvocableTestReturningInt_RValueReferenceNoexcept
{
	int operator()() && noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_RValueReferenceNoexcept
{
	void operator()( int, float, double ) && noexcept {}
};
struct InvocableTestReturningIntWithParameter_RValueReferenceNoexcept
{
	int operator()( int, float, double ) && noexcept { return 0; }
};

struct InvocableTestReturningVoid_ConstRValueReferenceNoexcept
{
	void operator()() const && noexcept {}
};
struct InvocableTestReturningInt_ConstRValueReferenceNoexcept
{
	int operator()() const && noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstRValueReferenceNoexcept
{
	void operator()( int, float, double ) const && noexcept {}
};
struct InvocableTestReturningIntWithParameter_ConstRValueReferenceNoexcept
{
	int operator()( int, float, double ) const && noexcept { return 0; }
};

struct InvocableTestReturningVoid_VolatileRValueReferenceNoexcept
{
	void operator()() volatile && noexcept {}
};
struct InvocableTestReturningInt_VolatileRValueReferenceNoexcept
{
	int operator()() volatile && noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_VolatileRValueReferenceNoexcept
{
	void operator()( int, float, double ) volatile && noexcept {}
};
struct InvocableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept
{
	int operator()( int, float, double ) volatile && noexcept { return 0; }
};

struct InvocableTestReturningVoid_ConstVolatileRValueReferenceNoexcept
{
	void operator()() const volatile && noexcept {}
};
struct InvocableTestReturningInt_ConstVolatileRValueReferenceNoexcept
{
	int operator()() const volatile && noexcept { return 0; }
};
struct InvocableTestReturningVoidWithParameter_ConstVolatileRValueReferenceNoexcept
{
	void operator()( int, float, double ) const volatile && noexcept {}
};
struct InvocableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept
{
	int operator()( int, float, double ) const volatile && noexcept { return 0; }
};


static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_Const>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_Const>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_Const>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_Const>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_Const>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_Const>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_Const>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_Const>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_Volatile>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_Volatile>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_Volatile>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_Volatile>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_Volatile>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_Volatile>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_Volatile>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_Volatile>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstVolatile>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstVolatile>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstVolatile>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatile>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstVolatile>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstVolatile>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstVolatile>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatile>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_LValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_LValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_LValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_LValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_LValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_LValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_LValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_LValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstLValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_VolatileLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_VolatileLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_VolatileLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_VolatileLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_VolatileLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_VolatileLValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstVolatileLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstVolatileLValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstVolatileLValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstVolatileLValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_RValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_RValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_RValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_RValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_RValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_RValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_RValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_RValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstRValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_VolatileRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_VolatileRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_VolatileRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_VolatileRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_VolatileRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_VolatileRValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReference>::ParameterTypeList, TypeList<int, float, double>> );

static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstVolatileRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstVolatileRValueReference>::ReturnType, void> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReference>::ReturnType, int> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningInt_ConstVolatileRValueReference>::ParameterTypeList, TypeList<>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningVoidWithParameter_ConstVolatileRValueReference>::ParameterTypeList, TypeList<int, float, double>> );
static_assert( std::is_same_v<InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReference>::ParameterTypeList, TypeList<int, float, double>> );


static_assert( InvocableTraits<InvocableTestReturningVoid>::ParameterCount() == 0 );
static_assert( InvocableTraits<InvocableTestReturningInt>::ParameterCount() == 0 );
static_assert( InvocableTraits<InvocableTestReturningVoidWithParameter>::ParameterCount() == 3 );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter>::ParameterCount() == 3 );

static_assert( InvocableTraits<InvocableTestReturningVoid>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_Const>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Const>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Const>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_Const>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Const>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Const>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Const>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_Volatile>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Volatile>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Volatile>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Volatile>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_Volatile>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Volatile>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Volatile>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatile>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatile>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatile>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatile>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatile>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatile>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatile>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReference>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReference>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReference>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReference>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReference>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReference>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReference>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReference>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReference>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReference>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReference>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReference>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReference>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReference>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReference>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReference>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReference>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReference>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReference>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReference>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReference>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReference>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReference>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReference>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReference>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReference>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReference>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReference>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReference>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReference>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReference>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReference>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningVoid_Noexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Noexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Noexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_Noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReferenceNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReferenceNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_LValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReferenceNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReferenceNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReferenceNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReferenceNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_RValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReferenceNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReferenceNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstRValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_VolatileRValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningVoid_ConstVolatileRValueReferenceNoexcept>::IsNoexcept() == true );


static_assert( InvocableTraits<InvocableTestReturningIntWithParameter>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Const>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Const>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Const>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Const>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Const>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Const>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Const>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Volatile>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Volatile>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Volatile>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Volatile>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Volatile>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Volatile>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Volatile>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatile>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatile>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatile>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatile>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatile>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatile>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatile>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReference>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReference>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReference>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReference>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReference>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReference>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReference>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReference>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReference>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReference>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReference>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReference>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReference>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReference>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReference>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReference>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReference>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReference>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReference>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReference>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReference>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReference>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReference>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReference>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReference>::IsNoexcept() == false );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Noexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Noexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Noexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Noexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Noexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Noexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_Noexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_LValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsLValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsRValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileLValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_RValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsVolatile() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstRValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsConst() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_VolatileRValueReferenceNoexcept>::IsNoexcept() == true );

static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsPlainFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsMemberFunction() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsConst() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsVolatile() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsLValueOnly() == false );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsRValueOnly() == true );
static_assert( InvocableTraits<InvocableTestReturningIntWithParameter_ConstVolatileRValueReferenceNoexcept>::IsNoexcept() == true );



} // tests
#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // utility
} // bc
