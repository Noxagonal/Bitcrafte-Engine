#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

// Vulkan specific
#include <vulkan/vulkan.h>

// Assertion
#include <core/diagnostic/assertion/HardAssert.hpp>
#include <core/diagnostic/assertion/Assert.hpp>
#include <core/diagnostic/assertion/Ensure.hpp>
#include <rhi_vulkan/diagnostic/assertion/VulkanAssert.hpp>

// Containers
#include <core/containers/UniquePtr.hpp>
#include <core/containers/List.hpp>
#include <core/containers/Text.hpp>
#include <core/containers/Pair.hpp>
#include <core/containers/Map.hpp>

// STL
#include <bitset>
#include <utility>
