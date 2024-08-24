
#include <gtest/gtest.h>

#include <core/CoreComponent.hpp>
#include <core/message_bus/MessageBus.hpp>



namespace core {



static constexpr float		Basic_MAValue = 5.0f;
static constexpr int		Basic_MBValue = 19;
static constexpr double		Basic_MCValue = 1010.5;



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sample message types
struct MessageA
{
	MessageA( float value ) : value( value ) {}
	float value = 0.0f;
};
struct MessageB
{
	MessageB( int value ) : value( value ) {}
	int value = 0;
};
struct MessageC
{
	MessageC( double value ) : value( value ) {}
	double value = 0.0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Sample message handlers
struct MessageHandlerA : public bc::MessageBusMessageHandler<MessageA>
{
	void operator()( const MessageType* message )
	{
		EXPECT_FLOAT_EQ( message->value, Basic_MAValue );
	}
};
struct MessageHandlerB : public bc::MessageBusMessageHandler<MessageB>
{
	MessageHandlerB() {};

	void operator()( const MessageType* message )
	{
		EXPECT_EQ( message->value, Basic_MBValue );
	}
};
struct MessageHandlerC : public bc::MessageBusMessageHandler<MessageC>
{
	void operator()( const MessageType* message )
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
		packet_1->AddMessage( MessageA( Basic_MAValue ) );
		packet_1->AddMessage( MessageB( Basic_MBValue ) );
		packet_1->AddMessage( MessageC( Basic_MCValue ) );
		UniquePtr<Packet_2> packet_2 = MakeUniquePtr<Packet_2>();
		packet_2->AddMessage( MessageB( Basic_MBValue ) );
		packet_2->AddMessage( MessageC( Basic_MCValue ) );
		UniquePtr<Packet_3> packet_3 = MakeUniquePtr<Packet_3>();
		packet_3->AddMessage( MessageA( Basic_MAValue ) );
		packet_3->AddMessage( MessageC( Basic_MCValue ) );

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
		receiver_1.ProcessMessages( *received_packet_1 );
		receiver_2.ProcessMessages( *received_packet_2 );
		receiver_3.ProcessMessages( *received_packet_3 );
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
				receiver_1.ProcessMessages( *packet_1 );

				messages_handled = true;
			}
		);

		EXPECT_FALSE( messages_handled );

		// Make packet.
		auto packet_1 = bc::MakeUniquePtr<Packet_1>();
		packet_1->AddMessage( MessageA( Basic_MAValue ) );
		packet_1->AddMessage( MessageB( Basic_MBValue ) );
		packet_1->AddMessage( MessageC( Basic_MCValue ) );

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
		packet_1->AddMessage( MessageA( Basic_MAValue ) );
		packet_1->AddMessage( MessageB( Basic_MBValue ) );
		packet_1->AddMessage( MessageC( Basic_MCValue ) );
		UniquePtr<Packet_2> packet_2 = MakeUniquePtr<Packet_2>();
		packet_2->AddMessage( MessageB( Basic_MBValue ) );
		packet_2->AddMessage( MessageC( Basic_MCValue ) );

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



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::i64 MessageBus_EnsureDestructorCalled_handler_alive_count = 0;
TEST( MessageBus, EnsureDestructorCalled )
{
	using namespace bc;

	MessageBus_EnsureDestructorCalled_handler_alive_count = 0;

	struct NonThrowingMessageHandler_A : public MessageBusMessageHandler<MessageA>
	{
		NonThrowingMessageHandler_A()
		{
			MessageBus_EnsureDestructorCalled_handler_alive_count++;
		}
		NonThrowingMessageHandler_A( const NonThrowingMessageHandler_A & other )
		{
			MessageBus_EnsureDestructorCalled_handler_alive_count++;
		}
		NonThrowingMessageHandler_A( NonThrowingMessageHandler_A && other )
		{
			MessageBus_EnsureDestructorCalled_handler_alive_count++;
		}
		~NonThrowingMessageHandler_A()
		{
			MessageBus_EnsureDestructorCalled_handler_alive_count--;
		}
		void operator()( const MessageType* message ) {}
	};
	struct NonThrowingMessageHandler_B : public NonThrowingMessageHandler_A {};
	struct NonThrowingMessageHandler_C : public NonThrowingMessageHandler_A {};

	{
		using Receiver = MessageBusReceiver<NonThrowingMessageHandler_A, NonThrowingMessageHandler_B, NonThrowingMessageHandler_C>;

		MessageBus_EnsureDestructorCalled_handler_alive_count = 0;

		//auto message_bus		= MessageBus<Packet>();
		Receiver();
		EXPECT_EQ( MessageBus_EnsureDestructorCalled_handler_alive_count, 0 );
	}
	{
		using Receiver = MessageBusReceiver<NonThrowingMessageHandler_C, NonThrowingMessageHandler_A, NonThrowingMessageHandler_B>;

		MessageBus_EnsureDestructorCalled_handler_alive_count = 0;

		//auto message_bus		= MessageBus<Packet>();
		Receiver();
		EXPECT_EQ( MessageBus_EnsureDestructorCalled_handler_alive_count, 0 );
	}
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bc::i64 MessageBus_ThrowingHandlerConstructor_handler_alive_count = 0;
TEST( MessageBus, ThrowingHandlerConstructor )
{
	using namespace bc;

	struct NonThrowingMessageHandler_A : public MessageBusMessageHandler<MessageA>
	{
		NonThrowingMessageHandler_A()
		{
			MessageBus_ThrowingHandlerConstructor_handler_alive_count++;
		}
		NonThrowingMessageHandler_A( const NonThrowingMessageHandler_A & other )
		{
			MessageBus_ThrowingHandlerConstructor_handler_alive_count++;
		}
		NonThrowingMessageHandler_A( NonThrowingMessageHandler_A && other )
		{
			MessageBus_ThrowingHandlerConstructor_handler_alive_count++;
		}
		~NonThrowingMessageHandler_A()
		{
			MessageBus_ThrowingHandlerConstructor_handler_alive_count--;
		}
		void operator()( const MessageType* message ) {}
	};
	struct NonThrowingMessageHandler_B : public NonThrowingMessageHandler_A {};
	
	struct ThrowingMessageHandler : public NonThrowingMessageHandler_A
	{
		ThrowingMessageHandler() {
			throw diagnostic::Exception( "ThrowingHandlerConstructor" );
		}
	};

	{
		using Receiver = MessageBusReceiver<ThrowingMessageHandler, NonThrowingMessageHandler_A, NonThrowingMessageHandler_B>;

		MessageBus_ThrowingHandlerConstructor_handler_alive_count = 0;

		//auto message_bus		= MessageBus<Packet>();
		EXPECT_THROW( Receiver(), diagnostic::Exception );
		EXPECT_EQ( MessageBus_ThrowingHandlerConstructor_handler_alive_count, 0 );
	}
	{
		using Receiver = MessageBusReceiver<NonThrowingMessageHandler_A, NonThrowingMessageHandler_B, ThrowingMessageHandler>;

		MessageBus_ThrowingHandlerConstructor_handler_alive_count = 0;

		//auto message_bus		= MessageBus<Packet>();
		EXPECT_THROW( Receiver(), diagnostic::Exception );
		EXPECT_EQ( MessageBus_ThrowingHandlerConstructor_handler_alive_count, 0 );
	}
};


} // core
