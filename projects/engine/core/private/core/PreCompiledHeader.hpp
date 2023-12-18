#pragma once

// Components
#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/CoreComponent.hpp>

// Containers
#include <core/containers/simple/SimpleUniquePtr.hpp>
#include <core/containers/simple/SimpleList.hpp>
#include <core/containers/simple/SimpleText.hpp>
#include <core/containers/simple/SimplePair.hpp>
#include <core/containers/UniquePtr.hpp>
#include <core/containers/List.hpp>
#include <core/containers/Text.hpp>
#include <core/containers/Pair.hpp>
#include <core/containers/Map.hpp>
#include <core/containers/Path.hpp>

// Diagnostic
#include <core/diagnostic/assertion/Assert.hpp>
#include <core/diagnostic/assertion/HardAssert.hpp>
#include <core/diagnostic/crash_handling/Panic.hpp>
#include <core/diagnostic/exception/Exception.hpp>
#include <core/diagnostic/logger/Logger.hpp>
#include <core/diagnostic/print_record/PrintRecord.hpp>
#include <core/diagnostic/print_record/PrintRecordFactory.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>
#include <core/diagnostic/stack_trace/StackTrace.hpp>
#include <core/diagnostic/system_console/SystemConsole.hpp>
#include <core/diagnostic/thread/ThreadingDebug.hpp>

// Timers
#include <core/timer/BasicTimer.hpp>

// C++ libraries
#include <assert.h>
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <utility>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
