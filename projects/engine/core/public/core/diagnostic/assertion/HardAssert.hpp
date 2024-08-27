#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/diagnostic/crash_handling/Panic.hpp>



namespace bc {



#if BITCRAFTE_GAME_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make sure condition is true in game development builds or crash. Test is disabled in shipping builds.
///
///	If condition is false, in game development builds, application will crash.
///
/// Similar to BAssert except that BHardAssert is meant to be used in situations where BAssert is not available, for example in
/// functions that are not allowed to throw exceptions.
///
/// @note
///	This test stays active in all development debug and release builds and only gets removed in the final game release.
/// 
/// @see
/// BAssert, bc::diagnostic::Panic()
///
/// @warning
/// Do not ignore these errors, final shipping release will not do these checks and if they're not handled during development,
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
		/* TODO: Add logging. */													\
		::bc::diagnostic::Panic();													\
	}																				\
} while( false )

#else // BITCRAFTE_GAME_DEVELOPMENT_BUILD

#define BHardAssert( m_condition, m_failure_explanation )

#endif // BITCRAFTE_GAME_DEVELOPMENT_BUILD



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make sure condition is true in engine development builds. Test is disabled in shipping builds and game development builds.
///
/// Similar to assert and will crash the application if condition is false. BEngineHardAssert is meant to be used in situations
/// where BEngineAssert is not available, for example in functions that are not allowed to throw exceptions.
///
/// @see
/// BEngineAssert, bc::diagnostic::Panic()
///
/// @warning
/// Do not ignore these errors, final shipping release will not do these checks and if they're not handled during  development,
/// the final application will not work properly and/or crash.
///
/// @param m_condition
/// Must be convertible to bool and must equal true for this test to pass.
///
/// @param m_failure_explanation
/// General explanation of what went wrong. Eg, "Index out of scope" when iterating over array. This must be a string literal or
/// c_string.
#define BEngineHardAssert( m_condition, m_failure_explanation )						\
do {																				\
	if( !( m_condition ) ) [[unlikely]] {											\
		/* TODO: Add logging. */													\
		::bc::diagnostic::Panic();													\
	}																				\
} while( false )

#else // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD

#define BEngineHardAssert( m_condition, m_failure_explanation )

#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // bc
