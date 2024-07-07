#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/exception/Exception.hpp>



namespace bc {



#if BITCRAFTE_GAME_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make sure condition is true in both development and shipping builds.
///
///	If condition is false, in development builds, an exception will be thrown. In shipping builds, this will cause the engine to
/// shut down immediately.
///
/// @see
/// bc::diagnostic::Throw(), bc::diagnostic::Panic()
///
///	This macro stays active in all builds.
/// 
/// @warning
/// Do not ignore these errors, final shipping build will crash if they're not encountered.
///
/// @param m_condition
///	Must be convertible to bool and must be true for this test to pass.
///
/// @param m_failure_explanation
///	bc::diagnostic::MessageEntryText. General explanation of what went wrong. Eg, "Index out of scope" when iterating over
/// array.
#define BEnsure( m_condition, m_failure_explanation )								\
do {																				\
	if( !( m_condition ) ) [[unlikely]] {											\
		::bc::diagnostic::Throw( m_failure_explanation );							\
	}																				\
} while( false )



#else // BITCRAFTE_GAME_DEVELOPMENT_BUILD



#define BEnsure( m_condition, m_failure_explanation )								\
do {																				\
	if( !( m_condition ) ) [[unlikely]] {											\
		::bc::diagnostic::Panic( m_failure_explanation );							\
	}																				\
} while( false )



#endif // BITCRAFTE_GAME_DEVELOPMENT_BUILD



} // bc
