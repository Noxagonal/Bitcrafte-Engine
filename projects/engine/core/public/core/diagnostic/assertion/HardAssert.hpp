#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/crash_handling/Panic.hpp>



namespace bc {



#if BITCRAFTE_GAME_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make sure condition is true in development builds. Test is disabled in shipping builds.
///
/// Similar to assert and will crash the application if condition is false. BHardAssert is meant to be used
/// in situations where BAssert is not available, for example in functions that are not allowed to throw exceptions.
///
/// @see
/// BAssert, bc::diagnostic::Panic()
///
/// @warning
/// Do not ignore these errors, final shipping release will not do these checks and if they're not handled during  development,
/// the final application will not work properly and/or crash.
///
/// @param m_condition
///	Must be convertible to bool and must equal true for this test to pass.
///
/// @param m_failure_explanation
///	General explanation of what went wrong. Eg, "Index out of scope" when iterating over array. This must be a string literal or
/// c_string.
#define BHardAssert( m_condition, m_failure_explanation )							\
do {																				\
	if( !( m_condition ) ) [[unlikely]] {											\
		::bc::diagnostic::Panic( m_failure_explanation );							\
	}																				\
} while( false )



#else // BITCRAFTE_GAME_DEVELOPMENT_BUILD



#define BHardAssert( m_condition, m_failure_explanation )



#endif // BITCRAFTE_GAME_DEVELOPMENT_BUILD



} // bc
