#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief 
/// Base class for a message handler which is used to receive messages from a message packet.
/// 
/// @tparam MessageBusMessageDerivedType
/// Type of message to this message handler is used to receive. Must be derived from Message class.
template<typename MessageBusMessageDerivedType>
class MessageBusMessageHandler
{
public:
	using MessageType = MessageBusMessageDerivedType;
};



} // bc
