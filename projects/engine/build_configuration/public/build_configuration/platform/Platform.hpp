#pragma once

#if defined( _WIN32 ) && !defined( _WIN64 )
#error "32 bit systems are not supported"


#elif defined( _WIN32 ) && defined( _WIN64 )
// We're compiling on windows.
#define BITCRAFTE_PLATFORM_WINDOWS       1



#elif defined( __linux__ ) && !defined( __ANDROID__ ) && !defined( __APPLE__ )
// We're compiling for linux (not android)
#define BITCRAFTE_PLATFORM_LINUX         1



#elif defined( __ANDROID__ )
// We're compiling for android
#define BITCRAFTE_PLATFORM_ANDROID       1



#elif defined( __APPLE__ )
// We're compiling for apple
#define BITCRAFTE_PLATFORM_APPLE         1



#else

#error "please add platform support here!"

#endif
