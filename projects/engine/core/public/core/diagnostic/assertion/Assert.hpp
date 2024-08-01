#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/exception/Exception.hpp>



namespace bc {



#if BITCRAFTE_GAME_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make sure condition is true in development builds. Test is disabled in shipping builds.
///
///	If condition is false, in development builds, an exception will be thrown.
///
/// @see
/// bc::diagnostic::Throw()
///
///	This macro stays active in all development debug and release builds and only gets removed in the final game release.
/// 
/// @warning
/// Do not ignore these errors, final non-development release will not do these checks and if they're not handled during
/// development, the final application will not work properly and/or crash.
///
/// @param m_condition
///	Must be convertible to bool and must be true for this test to pass.
///
/// @param m_failure_explanation
///	bc::diagnostic::MessageEntryText. General explanation of what went wrong. Eg, "Index out of scope" when iterating over
/// array.
#define BAssert( m_condition, m_failure_explanation )								\
do {																				\
	if( !( m_condition ) ) [[unlikely]] {											\
		::bc::diagnostic::Throw( m_failure_explanation );							\
	}																				\
} while( false )



#else // BITCRAFTE_GAME_DEVELOPMENT_BUILD



#define BAssert( m_condition, m_failure_explanation, ... )



#endif // BITCRAFTE_GAME_DEVELOPMENT_BUILD



} // bc
