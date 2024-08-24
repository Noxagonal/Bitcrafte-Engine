#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
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
	using HandlerInvokeSignature = void( void*, const void* );
	using HandlerDestructorSignature = void( void* );

public:
	using MessageHandlerTypeList = utility::TypeList<MessageBusMessageHandlerTypePack...>;

	static_assert(
		!MessageHandlerTypeList::HasDuplicates(),
		"All message handlers types must be unique, duplicate message handler types are not allowed as a template parameter to MessageReceiver"
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	struct alignas( std::max( alignof( HandlerInvokeSignature* ), MessageHandlerTypeList::TypeMaxAlignment() ) ) HandlerStorage
	{
		u8								storage[ MessageHandlerTypeList::TypeMaxSize() ];
		HandlerInvokeSignature		*	invoker;
		HandlerDestructorSignature	*	destruct;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	MessageBusReceiver()
	{
		auto index = i64( 0 );
		try
		{
			( ( message_handler_storage_list[ index++ ] = CreateHandlerStorageFromHandler( MessageBusMessageHandlerTypePack() ) ), ... );
		}
		catch( const diagnostic::Exception & exeption )
		{
			for( i64 i = index - 1; i >= 0; --i )
			{
				auto & handler = message_handler_storage_list[ i ];
				handler.destruct( &handler.storage );
			}
			throw;
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	~MessageBusReceiver()
	{
		for( i64 i = MessageHandlerTypeList::Size() - 1; i >= 0; --i )
		{
			auto & handler = message_handler_storage_list[ i ];
			handler.destruct( &handler.storage );
		}
	}

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
	void ProcessMessages( const MessageBusPacketType & message_packet )
	{
		using MessageTypeList = typename MessageBusPacketType::MessageTypeList;
		static_assert(
			MessageHandlerTypeList::Size() == MessageTypeList::Size(),
			"Message types and message handlers must match, make sure that the template arguments of your implementation "
			"of MessagePacketType matches the template argument list of messages in MessageBusPacket"
		);
		TestMessageTypeMatchesHandlerType<MessageTypeList>();

		auto & message_type_index_list = message_packet.GetMessageTypeIndexList();
		auto & message_list = message_packet.GetMessageList();

		// Message indices are stored separately for each message type. Indices are used to determine which message handler is used
		// for each message. Indices are stored in the same order as the messages are stored in the message packet.
		assert( message_type_index_list.Size() == message_list.Size() );

		for( u64 i = 0; i < message_type_index_list.Size(); ++i )
		{
			auto & handler = message_handler_storage_list[ message_type_index_list[ i ] ];
			handler.invoker( &handler.storage, &message_list[ i ] );
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
			std::is_same_v<typename MessageBusMessageTypeList::template IndexToType<Index>, typename MessageHandlerTypeList::template IndexToType<Index>::MessageType>,
			"Message types and message handlers must match, make sure that the template arguments of your implementation "
			"of MessagePacketType matches the template argument list of messages in MessageBusPacket"
		);
		if constexpr( Index + 1 < MessageHandlerTypeList::Size() )
		{
			DoTestMessageTypeMatchesHandlerType<Index + 1, MessageBusMessageTypeList>();
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename HandlerType>
	HandlerStorage		CreateHandlerStorageFromHandler( HandlerType && handler )
	{
		using HandlerMessageType = HandlerType::MessageType;
		HandlerStorage storage;
		new( &storage.storage ) HandlerType( std::forward<HandlerType>( handler ) );
		storage.invoker = []( void* handler_storage, const void* message ) {
			( *reinterpret_cast<HandlerType*>( handler_storage ) )( reinterpret_cast<const HandlerMessageType*>( message ) );
		};
		storage.destruct = []( void* handler_storage ) {
			( *reinterpret_cast<HandlerType*>( handler_storage ) ).~HandlerType();
		};
		return storage;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Array<HandlerStorage, MessageHandlerTypeList::Size()>		message_handler_storage_list;
};



} // bc
