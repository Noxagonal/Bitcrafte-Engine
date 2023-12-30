#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/crash_handling/Panic.hpp>



namespace bc {



#if BITCRAFTE_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make sure condition is true or crash the application.
///
/// BHardAssert is similar to C/C++ assert. BHardAssert is meant to be used in situations where BAssert is not available. For
/// example in SimpleTextBase which are not allowed to throw.
///
///	Similar to assert but invokes custom crash handler. This macro is also active in development release builds, it is only
/// disabled in shipping builds. Regular C/C++ assert macro should still be used when debugging the engine itself.
///
/// This macro is not meant to be used in situations that should always panic when something goes wrong, for example, failure to
/// allocate memory from the system should always panic and that test should be done in shipping builds too.
///
/// @see
/// BAssert
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



#else



#define BHardAssert( m_condition, m_failure_explanation )



#endif



} // bc
