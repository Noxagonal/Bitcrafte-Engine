#pragma once

#include "BuildOptions.hpp"
#include "platform/Platform.hpp"
#include "api/EngineAPI.hpp"



namespace bc {



// These definitions are now declared in CMakeLists.txt.
// BITCRAFTE_GAME_SHIPPING_BUILD for shipping builds
// BITCRAFTE_GAME_DEVELOPMENT_BUILD for game development builds
// BITCRAFTE_ENGINE_DEVELOPMENT_BUILD for engine development and debug builds

/*
#if defined( _DEBUG ) || defined( DEBUG ) || !defined( NDEBUG ) || ( BITCRAFTE_ENGINE_DEVELOPMENT_BUILD_ENABLE == 1 )
#if defined( BITCRAFTE_ENGINE_DEVELOPMENT_BUILD )
#undef BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
#endif
#define BITCRAFTE_ENGINE_DEVELOPMENT_BUILD 1
#else
#define BITCRAFTE_ENGINE_DEVELOPMENT_BUILD 0
#endif

// Force production build always when debugging.
#if BITCRAFTE_ENGINE_DEVELOPMENT_BUILD
#if defined( BITCRAFTE_GAME_DEVELOPMENT_BUILD )
#undef BITCRAFTE_GAME_DEVELOPMENT_BUILD
#endif
#define BITCRAFTE_GAME_DEVELOPMENT_BUILD		1
#endif
*/



} // bc
