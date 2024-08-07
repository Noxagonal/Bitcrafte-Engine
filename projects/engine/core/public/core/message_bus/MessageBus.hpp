#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/message_bus/MessageBusMessage.hpp>
#include <core/message_bus/MessageBusMessageHandler.hpp>
#include <core/message_bus/MessageBusReceiver.hpp>
#include <core/message_bus/MessageBusPacket.hpp>

#include <core/utility/template/TypeList.hpp>
#include <core/data_types/FundamentalTypes.hpp>
#include <core/containers/Array.hpp>
#include <core/containers/List.hpp>
#include <core/containers/UniquePtr.hpp>

#include <core/event/Event.hpp>

#include <mutex>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief 
/// Thread safe message bus which allows efficiently sending and receiving message packets.
/// 
/// Message bus is used to send and receive messages between systems. Messages are grouped into message packets, which makes
/// sending and receiving messages more efficient. Each message packet can be created in isolation in one thread, sent to the
/// bus, and received from the bus in another thread. The message bus can handle multiple types of message packets, but those
/// message packet types must be given to the message bus as template arguments.
/// 
/// Each message packet is designed to contain multiple messages and for optimal performance, you should put as many messages
/// into a single packet as possible, within reason. Once the packet is sent, it is owned by the bus until something else claims
/// it. When a message packet is sent, an event is signalled to all listeners, the listener can then claim the packet by calling
/// MessageBus::ClaimPacket(). After the packet has been claimed, its ownership is transferred to the receiver. The message
/// packet will stay in the bus until it is claimed.
/// 
/// Data contained in messages is thread safe in a sense that no other thread, except the thread that accepted the message
/// packet, is allowed to access the data. Pointers in messages however, are obviously not thread safe and should be used with
/// care.
/// 
/// Message bus is meant for real-time data transfer, so it cannot be serialized or de-serialized.
/// 
/// @tparam ...MessageBusMessagePacketTypePack
/// List of message packet types that are used to send and receive messages. Only these packet types are allowed to be sent and
/// received via this message bus.
template<typename ...MessageBusMessagePacketTypePack>
class MessageBus
{
public:
	using MessagePacketTypeList = utility::TypeList<MessageBusMessagePacketTypePack...>;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename MessageBusPacketType>
	void SendPacket( UniquePtr<MessageBusPacketType> message_packet )
	{
		static_assert( std::is_base_of_v<MessageBusPacketBase, MessageBusPacketType>, "MessageBusPacketType must be derived from MessageBusPacketBase" );
		static_assert( MessagePacketTypeList::template HasType<MessageBusPacketType>(), "MessageBusPacketType must have been introduced to the bus via template parameter pack argument, see MessageBus documentation" );

		auto packet_id = [ this, &message_packet ]() -> u64
			{
				std::lock_guard lock( message_bus_mutex );

				message_packet_id_counter++;
				auto packet_id = message_packet_id_counter;

				message_packet_list.PushBack( std::move( message_packet ) );
				message_packet_type_index_list.PushBack( MessagePacketTypeList::template TypeToIndex<MessageBusPacketType>() );

				message_packet_id_list.PushBack( packet_id );
				return packet_id;
			}( );

		OnPacketSent.Signal( packet_id );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename MessageBusPacketType>
	[[nodiscard]]
	UniquePtr<MessageBusPacketType> ClaimPacket( u64 packet_id )
	{
		static_assert( std::is_base_of_v<MessageBusPacketBase, MessageBusPacketType>, "MessageBusPacketType must be derived from MessagePacketBase" );
		static_assert( MessagePacketTypeList::template HasType<MessageBusPacketType>(), "MessageBusPacketType must have been introduced to the bus via template parameter pack argument, see MessageBus documentation" );

		std::lock_guard lock( message_bus_mutex );

		auto packet_index = GetMessagePacketIndex( packet_id );
		if( packet_index == std::numeric_limits<u64>::max() ) return nullptr; // Packet not found.
		BAssert(
			message_packet_type_index_list[ packet_index ] == MessagePacketTypeList::template TypeToIndex<MessageBusPacketType>(),
			"Message bus packet found, but its type does not match, packet cannot be claimed until the correct type is given to "
			"MessageBus::ClaimPacket<Type>(), packet will remain in the bus."
		);
		auto packet = std::move( message_packet_list[ packet_index ] );
		ErasePacketSlot( packet_index );
		return packet.template CastTo<MessageBusPacketType>();
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	u64 GetMessagePacketIndex( u64 packet_id )
	{
		auto it = std::find( message_packet_id_list.begin(), message_packet_id_list.end(), packet_id );
		if( it == message_packet_id_list.end() ) return std::numeric_limits<u64>::max();
		return it.GetIndex();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void ErasePacketSlot( u64 packet_index )
	{
		message_packet_list.Erase( message_packet_list.begin() + packet_index );
		message_packet_type_index_list.Erase( message_packet_type_index_list.begin() + packet_index );
		message_packet_id_list.Erase( message_packet_id_list.begin() + packet_index );
	}

public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Event that is signalled when a message packet is sent to the bus.
	///
	/// Packets can be claimed using MessageBus::ClaimPacket().
	///
	/// The event is signalled on the thread that sent the packet.
	///
	/// @see MessageBus::ClaimPacket()
	///
	/// @eventparam @ref u64
	/// ID of the packet that was sent. Should be used to claim the packet.
	Event<u64>									OnPacketSent;

	// TODO: OnPacketReceived event must be made thread safe.

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::mutex									message_bus_mutex;

	List<UniquePtr<MessageBusPacketBase>>		message_packet_list;
	List<u64>									message_packet_type_index_list;
	List<u64>									message_packet_id_list;

	u64 										message_packet_id_counter				= 0;
};



} // bc
