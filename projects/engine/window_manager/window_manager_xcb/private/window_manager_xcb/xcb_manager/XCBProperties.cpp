
#include <window_manager_xcb/PreCompiledHeader.hpp>

#include <window_manager_xcb/xcb_manager/XCBProperties.hpp>
#include "XCBProperties.hpp"



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
xcb_atom_t bc::window_manager::xcb::GetAtom(
	xcb_connection_t	*	connection,
	bc::TextView			atom_name,
	bool					create_if_not_exists
)
{
	auto cookie = xcb_intern_atom( connection, create_if_not_exists, static_cast<uint16_t>( atom_name.Size() ), atom_name.Data() );
	auto reply = xcb_intern_atom_reply( connection, cookie, nullptr );
	if( !reply ) return XCB_ATOM_NONE;

	auto atom = reply->atom;
	free( reply );

	return atom;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
xcb_get_property_reply_t * bc::window_manager::xcb::GetPropertyReply(
	xcb_connection_t					*	connection,
	xcb_window_t							window,
	xcb_atom_t								atom_property,
	xcb_atom_t								type,
	uint32_t								data_length
)
{
	auto cookie = xcb_get_property(
		connection,
		0,
		window,
		atom_property,
		type,
		0,
		data_length
	);
	auto reply = xcb_get_property_reply( connection, cookie, nullptr );
	if( !reply ) return nullptr;
	return reply;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::xcb::ChangePropertyRaw(
	xcb_connection_t	*	connection,
	xcb_window_t			window,
	xcb_atom_t				atom_property,
	xcb_atom_t				type,
	PropertyFormat			format,
	uint32_t				data_length,
	const void			*	in_data_buffer
)
{
	auto format_bytes = std::to_underlying( format ) / 8;
	assert( data_length % format_bytes == 0 && "Data length is not a multiple of the format" );
	xcb_change_property(
		connection,
		XCB_PROP_MODE_REPLACE,
		window,
		atom_property,
		type,
		static_cast<uint8_t>( format ),
		data_length / format_bytes,
		in_data_buffer
	);
	xcb_flush( connection );
}

void bc::window_manager::xcb::GetPropertyRaw(
	xcb_connection_t	*	connection,
	xcb_window_t			window,
	xcb_atom_t				atom_property,
	xcb_atom_t				type,
	PropertyFormat			format,
	uint32_t				data_length,
	void				*	out_data_buffer
)
{
	auto format_bytes = std::to_underlying<PropertyFormat>( format ) / 8;
	xcb_get_property_reply_t * reply = GetPropertyReply(
		connection,
		window,
		atom_property,
		type,
		data_length / format_bytes
	);
	if( !reply )
	{
		assert( 0 && "Failed to get property, reply is nullptr" );
		return;
	}
 
	auto property_actual_length = xcb_get_property_value_length( reply );
	assert( property_actual_length == data_length && "Failed to get property, length mismatch" );
	if( property_actual_length == data_length )
	{
		GetPropertyRawNoCheck( reply, data_length, out_data_buffer );
	}
	free( reply );
}

void bc::window_manager::xcb::GetPropertyRawNoCheck(
	xcb_get_property_reply_t	*	reply,
	uint32_t						data_length,
	void						*	out_data_buffer
)
{
	auto property_value = xcb_get_property_value( reply );
	memcpy( out_data_buffer, property_value, data_length );
}

void bc::window_manager::xcb::GetPropertyRaw(
	xcb_get_property_reply_t	*	reply,
	uint32_t						data_length,
	void						*	out_data_buffer
)
{
	if( !reply )
	{
		assert( 0 && "Failed to get property, reply is nullptr" );
		return;
	}
 
	auto property_actual_length = xcb_get_property_value_length( reply );
	assert( property_actual_length == data_length && "Failed to get property, length mismatch" );
	if( property_actual_length == data_length )
	{
		GetPropertyRawNoCheck( reply, data_length, out_data_buffer );
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t bc::window_manager::xcb::GetPropertyRawSize(
	xcb_connection_t	*	connection,
	xcb_window_t			window,
	xcb_atom_t				atom_property,
	xcb_atom_t				type
)
{
	xcb_get_property_reply_t * reply = GetPropertyReply(
		connection,
		window,
		atom_property,
		type,
		std::numeric_limits<uint32_t>::max()
	);
	if( !reply )
	{
		assert( 0 && "Failed to get property, reply is nullptr" );
		return 0;
	}
 
	auto property_actual_length = xcb_get_property_value_length( reply );
	free( reply );
	return property_actual_length;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int32_t bc::window_manager::xcb::GetPropertyRawSize(
	xcb_get_property_reply_t	*	reply
)
{
	if( !reply ) return 0;
	return xcb_get_property_value_length( reply );
}
