#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>
#include <core/data_types/FundamentalTypes.hpp>
#include <core/utility/template/TypeList.hpp>
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
	struct alignas( MessageTypeList::TypeMaxAlignment() ) MessageStorage
	{
		u8 storage[ MessageTypeList::TypeMaxSize() ];
	};

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	template<typename MessageBusMessageType>
	void AddMessage( MessageBusMessageType && type )
	{
		static_assert( !std::is_abstract_v<MessageBusMessageType>, "Message type must not be abstract" );

		message_type_index_list.PushBack( MessageTypeList::template TypeToIndex<MessageBusMessageType>() );
		message_storage_list.EmplaceBack();
		auto storage = reinterpret_cast<MessageBusMessageType*>( message_storage_list.Back().storage );
		new( storage ) MessageBusMessageType( std::forward<MessageBusMessageType>( type ) );
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const List<u64> & GetMessageTypeIndexList() const
	{
		return message_type_index_list;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const List<MessageStorage> & GetMessageList() const
	{
		return message_storage_list;
	}

private:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	List<u64>					message_type_index_list;
	List<MessageStorage>		message_storage_list;
};



} // bc
