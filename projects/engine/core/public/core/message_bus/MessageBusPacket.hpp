#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/data_types/FundamentalTypes.hpp>
#include <core/utility/template/TypeList.hpp>
#include <core/message_bus/MessageBusMessage.hpp>
#include <core/containers/UniquePtr.hpp>
#include <core/containers/List.hpp>



namespace bc {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MessageBusPacketBase
{
public:
	virtual ~MessageBusPacketBase() = default;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename ...MessageBusMessageTypePack>
class MessageBusPacket : public MessageBusPacketBase
{
public:
	using MessageTypeList = utility::TypeList<MessageBusMessageTypePack...>;

	static_assert(
		!MessageTypeList::HasDuplicates(),
		"Message types must be unique, duplicate message types are not allowed as a template parameter to MessageBusPacket"
	);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename MessageBusMessageType>
	void AddMessage( UniquePtr<MessageBusMessageType> type )
	{
		static_assert( !std::is_abstract_v<MessageBusMessageType>, "Message type must not be abstract" );
		static_assert( std::is_base_of_v<MessageBusMessage, MessageBusMessageType>, "Type must be derived from BaseType" );

		message_index_list.PushBack( MessageTypeList::template TypeToIndex<MessageBusMessageType>() );
		message_list.PushBack( std::move( type ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename MessageBusMessageReceiverType>
	void ReceiveMessages( MessageBusMessageReceiverType& receiver )
	{
		receiver.ProcessMessages( this );
		message_index_list.Clear();
		message_list.Clear();
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const List<u64> & GetMessageIndexList() const
	{
		return message_index_list;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const List<UniquePtr<MessageBusMessage>> & GetMessageList() const
	{
		return message_list;
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	List<u64>								message_index_list;
	List<UniquePtr<MessageBusMessage>>		message_list;
};



} // bc
