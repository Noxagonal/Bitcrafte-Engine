#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/diagnostic/exception/Exception.hpp>



namespace bc {



#if BITCRAFTE_GAME_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make sure condition is true in development builds or throw. Test is disabled in shipping builds.
///
///	If condition is false, in game development builds, an exception will be thrown.
///
/// @see
/// bc::diagnostic::Throw()
///
/// @note
///	This test stays active in all development debug and release builds and only gets removed in the final game release.
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
#define BAssert( m_condition, m_failure_explanation )										\
do {																						\
	if( !( m_condition ) ) [[unlikely]] {													\
		::bc::diagnostic::Throw( ::bc::diagnostic::Exception( m_failure_explanation ) );	\
	}																						\
} while( false )

#else // BITCRAFTE_GAME_DEVELOPMENT_BUILD

#define BAssert( m_condition, m_failure_explanation )

#endif // BITCRAFTE_GAME_DEVELOPMENT_BUILD



#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make sure condition is true in engine development builds. Test is disabled in shipping builds and game development builds.
///
///	If condition is false, in engine development builds, an exception will be thrown. In shipping builds and game development
/// builds, this test is disabled.
///
/// @see
/// bc::diagnostic::Throw()
///
/// @warning
/// Do not ignore these errors, final shipping release will not do these checks and if they're not handled during  development,
/// the final application will not work properly and/or crash.
///
/// @param m_condition
///	Must be convertible to bool and must be true for this test to pass.
///
/// @param m_failure_explanation
///	bc::diagnostic::MessageEntryText. General explanation of what went wrong. Eg, "Index out of scope" when iterating over
/// array.
#define BEngineAssert( m_condition, m_failure_explanation )									\
do {																						\
	if( !( m_condition ) ) [[unlikely]] {													\
		::bc::diagnostic::Throw( ::bc::diagnostic::Exception( m_failure_explanation ) );	\
	}																						\
} while( false )

#else // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD

#define BEngineAssert( m_condition, m_failure_explanation )

#endif // BITCRAFTE_ENGINE_DEVELOPMENT_BUILD



} // bc
