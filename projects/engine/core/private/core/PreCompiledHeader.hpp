#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/CoreComponent.hpp>

#include <core/containers/UniquePtr.hpp>
#include <core/containers/List.hpp>
#include <core/containers/Text.hpp>
#include <core/containers/Pair.hpp>
#include <core/containers/Map.hpp>
#include <core/containers/Path.hpp>

#include <core/diagnostic/assertion/Assert.hpp>
#include <core/diagnostic/assertion/HardAssert.hpp>
#include <core/diagnostic/source_location/SourceLocation.hpp>
#include <core/diagnostic/stack_trace/StackTrace.hpp>
#include <core/diagnostic/crash_handling/Panic.hpp>

#include <cstdint>
#include <cstdlib>
#include <memory>
