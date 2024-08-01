#pragma once

#include "../BuildOptions.hpp"
#include "../platform/Platform.hpp"



// Supported compilers:
// - Windows:
//   - MSVC (primary)
//   - Clang
// - Linux:
//   - Clang (primary)
//   - GCC
// - Apple:
//   - XCode-Clang (primary)



// Minimum required compiler versions.
#define BITCRAFTE_MSVC_MINIMUM_VERSION		1930
#define BITCRAFTE_CLANG_MINIMUM_VERSION		14
#define BITCRAFTE_GCC_MINIMUM_VERSION		12



#if !defined( DOXYGEN )



#if defined( BITCRAFTE_PLATFORM_WINDOWS )

#if defined( _MSC_VER ) && !defined( __clang__ )
#if( _MSC_VER < BITCRAFTE_MSVC_MINIMUM_VERSION )
#error "MSVC version not supported, please update your compiler"
#endif
#define BITCRAFTE_COMPILER_MSVC				1
#endif

#if defined( __clang__ )
#if( __clang_major__ < BITCRAFTE_CLANG_MINIMUM_VERSION )
#error "Clang version not supported, please update your compiler"
#endif
#define BITCRAFTE_COMPILER_CLANG			1
#endif

// define dynamic library export/import
#if BITCRAFTE_IS_ENGINE_SHARED
#if BITCRAFTE_IS_ENGINE_COMPONENT
#define BITCRAFTE_ENGINE_API				__declspec(dllexport)
#else
#define BITCRAFTE_ENGINE_API				__declspec(dllimport)
#endif
#else
#define BITCRAFTE_ENGINE_API
#endif

#endif // defined( BITCRAFTE_PLATFORM_WINDOWS )



#if defined( BITCRAFTE_PLATFORM_LINUX )

#if defined( __clang__ )
#if( __clang_major__ < BITCRAFTE_CLANG_MINIMUM_VERSION )
#error "Clang version not supported, please update your compiler"
#endif
#define BITCRAFTE_COMPILER_CLANG			1
#endif

#if defined( __GNUC__ ) && !defined( __clang__ )
#if( __GNUC__ < BITCRAFTE_GCC_MINIMUM_VERSION )
#error "GCC version not supported, please update your compiler"
#endif
#define BITCRAFTE_COMPILER_GCC				1
#endif

// define dynamic library export/import
#if BITCRAFTE_IS_ENGINE_SHARED
#if BITCRAFTE_IS_ENGINE_COMPONENT
#define BITCRAFTE_ENGINE_API				__attribute__((visibility("default")))
#else
#define BITCRAFTE_ENGINE_API
#endif
#else
#define BITCRAFTE_ENGINE_API
#endif

#endif // defined( BITCRAFTE_PLATFORM_LINUX )



#if defined( BITCRAFTE_PLATFORM_APPLE )

#if defined( __clang__ )
#if( __clang_major__ < BITCRAFTE_CLANG_MINIMUM_VERSION )
#error "Clang version not supported, please update your compiler"
#endif
#define BITCRAFTE_COMPILER_CLANG				1
#endif

// define dynamic library export/import
#if BITCRAFTE_IS_ENGINE_SHARED
#if BITCRAFTE_IS_ENGINE_COMPONENT
#define BITCRAFTE_ENGINE_API				__attribute__((visibility("default")))
#else
#define BITCRAFTE_ENGINE_API
#endif
#else
#define BITCRAFTE_ENGINE_API
#endif

#endif // defined( BITCRAFTE_PLATFORM_APPLE )



#endif // !defined( DOXYGEN )



#if !defined( BITCRAFTE_ENGINE_API )
#error "BITCRAFTE_ENGINE_API not defined!"
#endif

#if !defined( BITCRAFTE_COMPILER_MSVC ) && !defined( BITCRAFTE_COMPILER_CLANG ) && !defined( BITCRAFTE_COMPILER_GCC )
#error "Couldn't identify a compiler being used"
#endif



#undef BITCRAFTE_MSVC_MINIMUM_VERSION
#undef BITCRAFTE_CLANG_MINIMUM_VERSION
#undef BITCRAFTE_GCC_MINIMUM_VERSION
