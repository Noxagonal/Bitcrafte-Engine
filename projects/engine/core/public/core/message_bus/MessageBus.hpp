#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
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



// TODO: Performance considerations. A lot of the MessageBus behaviour relies on virtual classes. This is not very cache
// friendly. It could probably be improved by using a std::variant kind of approach, where everything is stored locally.
// A few ideas come to mind:
// - Need a way to convert a base type to their derived types at runtime. We already have a way of getting type index.
//   It needs a runtime check. This could be achieved by using a lot of switch statements which is fast.
//   Idea is that input is runtime index, and output is an operation so, (number) -> (operation). The opration itself can be
//   a template function as well so that we could use it all the different types. A type of lookup table, instead of double
//   indirection caused by the virtual classes. This could be a generic operation.
//
// - This method cannot output anything because the type is erased, it can only modify data we give it.
//   This may not be a problem however.
//
// Here's some test code:
/*
// Runtime index-based functor invocation
template<template<typename> class F, typename... Ts>
class CallFunctorWithType
{
	// List of functions to create the variant and run the functor
	std::vector<std::function<void()>> functors = {
		[]() { F<Ts>()(); }...
	};
public:
	void Run(size_t index)
	{
		if (index < functors.size()) {
			functors[index]();
		} else {
			std::cerr << "Index out of bounds!" << std::endl;
		}
	}
};

// Functor template
template<typename T>
struct Callable {
	void operator()() {
		std::cout << "Running functor with type: " << typeid(T).name() << std::endl;
	}
};

// Usage example
int main() {
	// Run functor with different indices
	auto test = CallFunctorWithType<Callable, int, double, std::string>();

	test.Run(0); // Should run with int
	test.Run(1); // Should run with double
	test.Run(2); // Should run with std::string
	test.Run(3); // Out of bounds

	return 0;
}
*/



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
		static_assert(
			std::is_base_of_v<MessageBusPacketBase, MessageBusPacketType>,
			"MessageBusPacketType must be derived from MessageBusPacketBase"
		);
		static_assert(
			MessagePacketTypeList::template HasType<MessageBusPacketType>(),
			"MessageBusPacketType must have been introduced to MessageBus via template parameter pack argument before it can be sent"
		);

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
	auto ClaimPacket( u64 packet_id ) -> UniquePtr<MessageBusPacketType>
	{
		static_assert(
			std::is_base_of_v<MessageBusPacketBase, MessageBusPacketType>,
			"MessageBusPacketType must be derived from MessagePacketBase"
		);
		static_assert(
			MessagePacketTypeList::template HasType<MessageBusPacketType>(),
			"MessageBusPacketType must have been introduced to MessageBus via template parameter pack argument before it can be claimed"
		);

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
	auto GetMessagePacketIndex( u64 packet_id ) -> u64
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
	/// @note
	/// The event is signalled by the thread that sent the packet.
	///
	/// @warning
	/// The event is not thread safe. Listeners are responsible for making sure that they don't cause race conditions.
	///
	/// @see MessageBus::ClaimPacket()
	///
	/// @eventparam @ref u64
	/// ID of the packet that was sent. Should be used to claim the packet.
	Event<u64>									OnPacketSent;

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::mutex									message_bus_mutex;

	List<UniquePtr<MessageBusPacketBase>>		message_packet_list;
	List<u64>									message_packet_type_index_list;
	List<u64>									message_packet_id_list;

	u64 										message_packet_id_counter				= 0;
};



} // bc
