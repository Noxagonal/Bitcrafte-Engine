#pragma once

#include "BuildOptions.hpp"
#include "platform/Platform.hpp"
#include "api/EngineAPI.hpp"



namespace bc {



#if defined( _DEBUG ) || defined( DEBUG ) || !defined( NDEBUG ) || ( BITCRAFTE_DEBUG_ENABLE == 1 )
#if defined( BITCRAFTE_DEBUG )
#undef BITCRAFTE_DEBUG
#endif
#define BITCRAFTE_DEBUG 1
#else
#define BITCRAFTE_DEBUG 0
#endif

// Force production build always when debugging.
#if BITCRAFTE_DEBUG
#if defined( BITCRAFTE_DEVELOPMENT_BUILD )
#undef BITCRAFTE_DEVELOPMENT_BUILD
#endif
#define BITCRAFTE_DEVELOPMENT_BUILD		1
#endif



} // bc
