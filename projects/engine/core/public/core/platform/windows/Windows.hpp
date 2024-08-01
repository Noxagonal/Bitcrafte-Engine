#pragma once


#define NOMINMAX
#include <Windows.h>
#undef NOMINMAX
// Prevent polluting Windows macros all over the place
#undef CreateWindow
#undef ERROR
