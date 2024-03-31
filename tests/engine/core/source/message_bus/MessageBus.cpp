
#include <gtest/gtest.h>

#include <core/CoreComponent.hpp>
#include <core/message_bus/MessageBus.hpp>



namespace core {



static constexpr float		Basic_MAValue = 5.0f;
static constexpr int		Basic_MBValue = 19;
static constexpr double		Basic_MCValue = 1010.5;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sample message types
struct MessageA : public bc::MessageBusMessage
{
	MessageA( float value ) : value( value ) {}
	float value = 0.0f;
};
struct MessageB : public bc::MessageBusMessage
{
	MessageB( int value ) : value( value ) {}
	int value = 0;
};
struct MessageC : public bc::MessageBusMessage
{
	MessageC( double value ) : value( value ) {}
	double value = 0.0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sample message handlers
struct MessageHandlerA : public bc::MessageBusMessageHandler<MessageA>
{
	virtual void operator()( const MessageType* message ) override
	{
		EXPECT_FLOAT_EQ( message->value, Basic_MAValue );
	}
};
struct MessageHandlerB : public bc::MessageBusMessageHandler<MessageB>
{
	virtual void operator()( const MessageType* message ) override
	{
		EXPECT_EQ( message->value, Basic_MBValue );
	}
};
struct MessageHandlerC : public bc::MessageBusMessageHandler<MessageC>
{
	virtual void operator()( const MessageType* message ) override
	{
		EXPECT_DOUBLE_EQ( message->value, Basic_MCValue );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MessageBus, Basic )
{
	using namespace bc;

	{
		using Packet_1 = MessageBusPacket<MessageA, MessageB, MessageC>;
		using Packet_2 = MessageBusPacket<MessageB, MessageC>;
		using Packet_3 = MessageBusPacket<MessageA, MessageC>;
		using Receiver_1 = MessageBusReceiver<MessageHandlerA, MessageHandlerB, MessageHandlerC>;
		using Receiver_2 = MessageBusReceiver<MessageHandlerB, MessageHandlerC>;
		using Receiver_3 = MessageBusReceiver<MessageHandlerA, MessageHandlerC>;

		MessageBus<Packet_1, Packet_2, Packet_3> message_bus;

		// Make packets.
		UniquePtr<Packet_1> packet_1 = MakeUniquePtr<Packet_1>();
		packet_1->AddMessage( bc::MakeUniquePtr<MessageA>( Basic_MAValue ) );
		packet_1->AddMessage( bc::MakeUniquePtr<MessageB>( Basic_MBValue ) );
		packet_1->AddMessage( bc::MakeUniquePtr<MessageC>( Basic_MCValue ) );
		UniquePtr<Packet_2> packet_2 = MakeUniquePtr<Packet_2>();
		packet_2->AddMessage( bc::MakeUniquePtr<MessageB>( Basic_MBValue ) );
		packet_2->AddMessage( bc::MakeUniquePtr<MessageC>( Basic_MCValue ) );
		UniquePtr<Packet_3> packet_3 = MakeUniquePtr<Packet_3>();
		packet_3->AddMessage( bc::MakeUniquePtr<MessageA>( Basic_MAValue ) );
		packet_3->AddMessage( bc::MakeUniquePtr<MessageC>( Basic_MCValue ) );

		// Send packet.
		message_bus.SendPacket( std::move( packet_1 ) );
		message_bus.SendPacket( std::move( packet_2 ) );
		message_bus.SendPacket( std::move( packet_3 ) );

		// Receive packet.
		Receiver_1 receiver_1;
		Receiver_2 receiver_2;
		Receiver_3 receiver_3;
		auto received_packet_1 = message_bus.ClaimPacket<Packet_1>( 1 );
		auto received_packet_2 = message_bus.ClaimPacket<Packet_2>( 2 );
		auto received_packet_3 = message_bus.ClaimPacket<Packet_3>( 3 );
		receiver_1.ProcessMessages( received_packet_1.Get() );
		receiver_2.ProcessMessages( received_packet_2.Get() );
		receiver_3.ProcessMessages( received_packet_3.Get() );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MessageBus, Event )
{
	using namespace bc;

	{
		using Packet_1 = MessageBusPacket<MessageA, MessageB, MessageC>;
		using Receiver_1 = MessageBusReceiver<MessageHandlerA, MessageHandlerB, MessageHandlerC>;

		auto message_bus		= MessageBus<Packet_1>();
		auto receiver_1			= Receiver_1();

		auto messages_handled	= false;

		// Register callback.
		message_bus.OnPacketSent.RegisterCallback( [ &message_bus, &receiver_1, &messages_handled ]( u64 packet_id )
			{
				EXPECT_EQ( packet_id, 1 );
				auto packet_1 = message_bus.ClaimPacket<Packet_1>( packet_id );
				packet_1->ReceiveMessages( receiver_1 );

				messages_handled = true;
			}
		);

		EXPECT_FALSE( messages_handled );

		// Make packet.
		auto packet_1 = bc::MakeUniquePtr<Packet_1>();
		packet_1->AddMessage( bc::MakeUniquePtr<MessageA>( Basic_MAValue ) );
		packet_1->AddMessage( bc::MakeUniquePtr<MessageB>( Basic_MBValue ) );
		packet_1->AddMessage( bc::MakeUniquePtr<MessageC>( Basic_MCValue ) );

		// Send packet.
		message_bus.SendPacket( std::move( packet_1 ) );

		EXPECT_TRUE( messages_handled );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
TEST( MessageBus, UnreceivablePacket )
{
	using namespace bc;

	{
		using Packet_1 = MessageBusPacket<MessageA, MessageB, MessageC>;
		using Packet_2 = MessageBusPacket<MessageB, MessageC>;
		using Receiver_1 = MessageBusReceiver<MessageHandlerA, MessageHandlerB, MessageHandlerC>;
		using Receiver_2 = MessageBusReceiver<MessageHandlerB, MessageHandlerC>;

		MessageBus<Packet_1, Packet_2> message_bus;

		// Make packets.
		UniquePtr<Packet_1> packet_1 = MakeUniquePtr<Packet_1>();
		packet_1->AddMessage( bc::MakeUniquePtr<MessageA>( Basic_MAValue ) );
		packet_1->AddMessage( bc::MakeUniquePtr<MessageB>( Basic_MBValue ) );
		packet_1->AddMessage( bc::MakeUniquePtr<MessageC>( Basic_MCValue ) );
		UniquePtr<Packet_2> packet_2 = MakeUniquePtr<Packet_2>();
		packet_2->AddMessage( bc::MakeUniquePtr<MessageB>( Basic_MBValue ) );
		packet_2->AddMessage( bc::MakeUniquePtr<MessageC>( Basic_MCValue ) );

		// Send packet.
		message_bus.SendPacket( std::move( packet_1 ) );
		message_bus.SendPacket( std::move( packet_2 ) );

		// Receive packet.
		Receiver_1 receiver_1;
		Receiver_2 receiver_2;
		auto received_packet_1 = message_bus.ClaimPacket<Packet_1>( 1 );
		auto received_packet_2 = message_bus.ClaimPacket<Packet_2>( 1 );

		EXPECT_FALSE( received_packet_1.IsEmpty() );
		EXPECT_TRUE( received_packet_2.IsEmpty() );
	}
};



} // core
