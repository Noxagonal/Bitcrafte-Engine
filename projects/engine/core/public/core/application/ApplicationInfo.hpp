#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/utility/version/Version.hpp>
#include <core/containers/Text.hpp>



namespace bc {
namespace application {



static constexpr auto EngineName			= TextView( "BitCrafte Engine" );
static constexpr auto EngineVersion			= utility::Version( BITCRAFTE_ENGINE_VERSION_MAJOR, BITCRAFTE_ENGINE_VERSION_MINOR, BITCRAFTE_ENGINE_VERSION_PATCH );



} // application
} // bc
