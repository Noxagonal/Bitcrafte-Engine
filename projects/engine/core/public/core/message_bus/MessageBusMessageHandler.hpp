#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/message_bus/MessageBusMessage.hpp>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Base class for a message handler.
/// 
/// Common non-template base class for a MessageBusMessageHandler which the MessageReceiver uses to receive messages.
class MessageBusMessageHandlerBase
{
	template<typename ...MessageBusMessageHandlerTypePack>
	friend class MessageBusReceiver;

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~MessageBusMessageHandlerBase() = default;

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Called by the MessageReceiver when a message should be processed.
	///
	/// @param message
	/// Message given to MessageBusMessageHandler.
	virtual void ReceiveMessage( const MessageBusMessage* message ) = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief 
/// Base class for a message handler which is used to receive messages from a message packet.
/// 
/// @tparam MessageBusMessageDerivedType
/// Type of message to this message handler is used to receive. Must be derived from Message class.
template<typename MessageBusMessageDerivedType>
class MessageBusMessageHandler : public MessageBusMessageHandlerBase
{
	static_assert( std::is_base_of_v<MessageBusMessage, MessageBusMessageDerivedType>, "MessageBusMessageDerivedType must be derived from MessageBusMessage" );

public:
	using MessageType = MessageBusMessageDerivedType;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Called by the MessageReceiver when a message should be processed.
	///
	/// @param message
	/// Message to process.
	virtual void operator()( const MessageType* message ) = 0;

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Called by the MessageReceiver when a message should be processed.
	virtual void ReceiveMessage( const MessageBusMessage* message ) override
	{
		// dynamic_cast is theoretically not needed because we check all the types at compile time. If these do not match, compiler
		// would have thrown an error. Leaving these here as reference.
		//auto casted_message = dynamic_cast<const MessageType*>( message );
		//if( casted_message == nullptr ) diagnostic::Throw( "Casting failed." );
		auto casted_message = static_cast<const MessageType*>( message );
		operator()( casted_message );
	}
};



} // bc
