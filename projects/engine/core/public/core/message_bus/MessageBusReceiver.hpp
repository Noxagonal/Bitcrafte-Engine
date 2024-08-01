#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/message_bus/MessageBusMessage.hpp>
#include <core/message_bus/MessageBusMessageHandler.hpp>

#include <core/utility/template/TypeList.hpp>
#include <core/data_types/FundamentalTypes.hpp>
#include <core/containers/UniquePtr.hpp>
#include <core/containers/Array.hpp>

#include <assert.h>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Class that is used to receive messages from a message packet and to resolve messages to different message handlers.
/// 
/// @tparam MessageBusMessageHandlerTypePack
/// Parameter pack of message handlers that this MessageReceiver can receive. Must match the template argument list of messages
/// in MessageBusPacket. Each message handler type must have been derived from MessageBusMessageHandler.
template<typename ...MessageBusMessageHandlerTypePack>
class MessageBusReceiver
{
public:
	using MessageReceiverTypeList = utility::TypeList<MessageBusMessageHandlerTypePack...>;

	static_assert(
		!MessageReceiverTypeList::HasDuplicates(),
		"MessageBusMessage handlers types must be unique, duplicate message handler types are not allowed as a template parameter to MessageReceiver"
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Process all messages from a message packet.
	///
	/// @tparam MessagePacketType
	/// Type of message packet.
	/// 
	/// @param message_packet
	/// Message packet to process.
	template<typename MessageBusPacketType>
	void ProcessMessages( const MessageBusPacketType * message_packet )
	{
		using MessageTypeList = typename MessageBusPacketType::MessageTypeList;
		static_assert(
			MessageReceiverTypeList::Size() == MessageTypeList::Size(),
			"Message types and message handlers must match, make sure that the template arguments of your implementation "
			"of MessagePacketType matches the template argument list of messages in MessageBusPacket"
		);
		TestMessageTypeMatchesHandlerType<MessageTypeList>();

		auto & message_index_list = message_packet->GetMessageIndexList();
		auto & message_list = message_packet->GetMessageList();

		// Message indices are stored separately for each message type. Indices are used to determine which message handler is used
		// for each message. Indices are stored in the same order as the messages are stored in the message packet.
		assert( message_index_list.Size() == message_list.Size() );

		for( u64 i = 0; i < message_index_list.Size(); ++i )
		{
			message_receivers[ message_index_list[ i ] ]->ReceiveMessage( message_list[ i ].Get() );
		}
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Test that the message types in the message packet match the message handlers in the message receiver.
	/// 
	/// @tparam MessageBusMessageTypeList
	/// List of message types in the message packet.
	template<utility::TypeListType MessageBusMessageTypeList>
	constexpr void TestMessageTypeMatchesHandlerType()
	{
		if constexpr( MessageBusMessageTypeList::Size() == 0 ) return;
		DoTestMessageTypeMatchesHandlerType<0, MessageBusMessageTypeList>();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Recursively test that the message types in the message packet match the message handlers in the message receiver.
	/// 
	/// @see TestMessageTypeMatchesHandlerType()
	/// 
	/// @tparam Index 
	/// Index of the current iteration to message type in the message packet.
	/// 
	/// @tparam MessageBusMessageTypeList
	/// List of message types in the message packet.
	template<u64 Index, utility::TypeListType MessageBusMessageTypeList>
	constexpr void DoTestMessageTypeMatchesHandlerType()
	{
		static_assert(
			std::is_same_v<typename MessageBusMessageTypeList::template IndexToType<Index>, typename MessageReceiverTypeList::template IndexToType<Index>::MessageType>,
			"Message types and message handlers must match, make sure that the template arguments of your implementation "
			"of MessagePacketType matches the template argument list of messages in MessageBusPacket"
		);
		if constexpr( Index + 1 < MessageReceiverTypeList::Size() )
		{
			DoTestMessageTypeMatchesHandlerType<Index + 1, MessageBusMessageTypeList>();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// List of functions for direct function dispatching which allows fast selection of behaviour based on type.
	//Array<UniquePtr<MessageBusMessageHandlerBase>, MessageReceiverTypeList::Size()> message_receivers {
	//	MakeUniquePtr<MessageBusMessageHandlerTypePack>()...
	//};
	// TODO: Replace Array<UniquePtr<MessageBusMessageHandlerBase>, MessageReceiverTypeList::Size()> above with something
	// equivalent to Array<std::variant<MessageBusMessageHandlerTypePack...>, MessageReceiverTypeList::Size()> once we have custom
	// variant type in engine.

	UniquePtr<MessageBusMessageHandlerBase> message_receivers[ MessageReceiverTypeList::Size() ] = { MakeUniquePtr<MessageBusMessageHandlerTypePack>()... };
};



} // bc
