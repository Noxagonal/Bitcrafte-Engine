#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Base class for a message type sent to a message bus.
/// 
/// You can derive from this class to create a message type that can be sent to a message bus.
class MessageBusMessage
{
public:
	virtual ~MessageBusMessage() = default;
};



} // bc
