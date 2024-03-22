#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/diagnostic/exception/Exception.hpp>



namespace bc {



#if BITCRAFTE_GAME_DEVELOPMENT_BUILD



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Make sure condition is true or throw an exception.
///
///	Similar to assert but without crashing the editor. If in-editor game is running, and condition is false, this will cause the
/// in-editor game to close and popup to display what went wrong. If the in-editor game is not running, then whatever object
/// caused this assertion to fail will be disabled and only a popup will be displayed.
///
///	If regular C/C++ assert() is used, this will cause the engine to shut down immediately. This is only useful for engine
/// development and should not be used in any addon or game code.
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



#else



#define BAssert( m_condition, m_failure_explanation, ... )



#endif



} // bc
