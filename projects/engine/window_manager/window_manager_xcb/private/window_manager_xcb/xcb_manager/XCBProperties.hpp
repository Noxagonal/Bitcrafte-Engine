#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/math/Vector.hpp>

#include <xcb/xcb.h>



namespace bc {
namespace window_manager {
namespace xcb {



struct XSizeHints
{
	long flags;						// marks which fields in this structure are defined
	int x, y;						// Obsolete
	int width, height;				// Obsolete
	int min_width, min_height;
	int max_width, max_height;
	int width_inc, height_inc;
	struct {
		int x;						// numerator
		int y;						// denominator
	} min_aspect, max_aspect;
	int base_width, base_height;
	int win_gravity;
};



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
xcb_atom_t									GetAtom(
	xcb_connection_t					*	xcb_connection,
	bc::TextView							atom_name,
	bool									create_if_not_exists
);

enum class PropertyFormat
{
	F8		= 8,
	F16		= 16,
	F32		= 32
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Get handle for XCB properties
///
/// @warning 
/// Returned handle must be freed manually by calling free().
///
/// @param connection
///	Pointer to XCB connection
///
/// @param window
///	Window handle
///
/// @param atom_property
///	Property handle
///
/// @param type
///	Property type
///
/// @param data_length
///	Property data length
///
/// @return
///	Property reply handle, or nullptr if property does not exist. MUST BE FREED MANUALLY.
xcb_get_property_reply_t				*	GetPropertyReply(
	xcb_connection_t					*	xcb_connection,
	xcb_window_t							xcb_window,
	xcb_atom_t								atom_property,
	xcb_atom_t								type,
	uint32_t								data_length
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void										ChangePropertyRaw(
	xcb_connection_t					*	xcb_connection,
	xcb_window_t							xcb_window,
	xcb_atom_t								atom_property,
	xcb_atom_t								type,
	PropertyFormat							format,
	uint32_t								data_length,
	const void							*	in_data_buffer
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void										GetPropertyRaw(
	xcb_connection_t					*	xcb_connection,
	xcb_window_t							xcb_window,
	xcb_atom_t								atom_property,
	xcb_atom_t								type,
	PropertyFormat							format,
	uint32_t								data_length,
	void								*	out_data_buffer
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GetPropertyRawNoCheck(
	xcb_get_property_reply_t			*	xcb_reply,
	uint32_t								data_length,
	void								*	out_data_buffer
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Get property data from reply
///
/// @warning
/// Reply handle is not freed by this function, it must be freed manually by calling free().
///
/// @param reply
///	Property reply handle
///
/// @param data_length
///	Property data expected length
///
/// @param out_data_buffer
///	Property data out buffer
void										GetPropertyRaw(
	xcb_get_property_reply_t			*	xcb_reply,
	uint32_t								data_length,
	void								*	out_data_buffer
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t									GetPropertyRawSize(
	xcb_connection_t					*	xcb_connection,
	xcb_window_t							xcb_window,
	xcb_atom_t								atom_property,
	xcb_atom_t								type
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Get property size from reply
///
/// @param reply
///	Property reply handle
///
/// @return
///	Property data total length
int32_t										GetPropertyRawSize(
	xcb_get_property_reply_t			*	xcb_reply
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename PropertyType>
void										ChangeProperty(
	xcb_connection_t					*	xcb_connection,
	xcb_window_t							xcb_window,
	xcb_atom_t								atom_property,
	xcb_atom_t								type,
	PropertyFormat							format,
	const PropertyType					&	data
)
{
	auto format_bytes = std::to_underlying( format ) / 8;
	uint32_t data_length = sizeof( PropertyType );
	assert( sizeof( PropertyType ) % format_bytes == 0 && "PropertyType must be a multiple of format bytes" );
	ChangePropertyRaw(
		xcb_connection,
		xcb_window,
		atom_property,
		type,
		format,
		data_length,
		&data
	);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename PropertyType>
PropertyType								GetProperty(
	xcb_connection_t					*	xcb_connection,
	xcb_window_t							xcb_window,
	xcb_atom_t								atom_property,
	xcb_atom_t								type,
	PropertyFormat							format
)
{
	auto data_length = sizeof( PropertyType );
	static_assert( sizeof( PropertyType ) % 4 == 0 );

	PropertyType data = {};
	GetPropertyRaw(
		xcb_connection,
		xcb_window,
		atom_property,
		type,
		format,
		data_length,
		&data
	);

	return data;
}


namespace internal_ {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PropertyHandleBase
{
protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PropertyHandleBase() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PropertyHandleBase(
		const PropertyHandleBase		&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	inline PropertyHandleBase(
		xcb_connection_t				*	xcb_connection,
		xcb_window_t						xcb_window,
		xcb_atom_t							atom_property,
		xcb_atom_t							type,
		PropertyFormat						format
	) :
		xcb_connection( xcb_connection ),
		xcb_window( xcb_window ),
		atom_property( atom_property ),
		type( type ),
		format( format )
	{}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	xcb_connection_t					*	xcb_connection;
	xcb_window_t							xcb_window;
	xcb_atom_t								atom_property;
	xcb_atom_t								type;
	PropertyFormat							format;
};

} // internal_


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Handle for XCB properties
///
/// @tparam PropertyType
///	Property type
template<typename PropertyType>
class PropertyHandle : public internal_::PropertyHandleBase
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PropertyHandle() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PropertyHandle(
		const PropertyHandle			&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PropertyHandle(
		xcb_connection_t				*	xcb_connection,
		xcb_window_t						xcb_window,
		xcb_atom_t							atom_property,
		xcb_atom_t							type,
		PropertyFormat						format
	) :
		internal_::PropertyHandleBase( xcb_connection, xcb_window, atom_property, type, format )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void									Change(
		const PropertyType				&	data
	)
	{
		auto format_bytes = std::to_underlying( format ) / 8;
		uint32_t data_length = sizeof( PropertyType );
		assert( data_length % format_bytes == 0 && "Data length must be a multiple of format bytes" );
		ChangeProperty<PropertyType>(
			xcb_connection,
			xcb_window,
			atom_property,
			type,
			format,
			data
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PropertyType							Get()
	{
		auto format_bytes = std::to_underlying( format ) / 8;
		uint32_t data_length = sizeof( PropertyType );
		assert( data_length % format_bytes == 0 && "Data length must be a multiple of format bytes" );
		return GetProperty<PropertyType>(
			xcb_connection,
			xcb_window,
			atom_property,
			type,
			format
		);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
///	Specialization of PropertyHandle for linear containers
///
/// @tparam PropertyType
///	Linear container property type
template<typename ContainerType>
requires( utility::LinearContainer<ContainerType> )
class PropertyHandle<ContainerType> : public internal_::PropertyHandleBase
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PropertyHandle() = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PropertyHandle(
		const PropertyHandle			&	other
	) = default;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PropertyHandle(
		xcb_connection_t				*	xcb_connection,
		xcb_window_t						xcb_window,
		xcb_atom_t							atom_property,
		xcb_atom_t							type,
		PropertyFormat						format
	) :
		internal_::PropertyHandleBase( xcb_connection, xcb_window, atom_property, type, format )
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void									Change(
		const ContainerType				&	container
	)
	{
		auto format_bytes = std::to_underlying( format ) / 8;
		uint32_t data_length = static_cast<uint32_t>( container.Size() * sizeof( typename ContainerType::value_type ) );
		assert( data_length % format_bytes == 0 && "Data length must be a multiple of format bytes" );
		ChangePropertyRaw(
			xcb_connection,
			xcb_window,
			atom_property,
			type,
			format,
			data_length,
			container.Data()
		);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	ContainerType							Get()
	{
		auto reply = GetPropertyReply(
			xcb_connection,
			xcb_window,
			atom_property,
			type,
			std::numeric_limits<uint32_t>::max()
		);
		if( !reply )
		{
			assert( 0 && "Failed to get property, reply is nullptr" );
			return ContainerType();
		}

		auto data_length = GetPropertyRawSize( reply );
		if( data_length == 0 )
		{
			assert( 0 && "Failed to get property, property data length is 0" );
			return ContainerType();
		}
		auto format_bytes = std::to_underlying( format ) / 8;
		assert( data_length % format_bytes == 0 && "Data length must be a multiple of format bytes" );

		auto container_element_count = data_length / sizeof( typename ContainerType::value_type );
		assert( data_length % sizeof( typename ContainerType::value_type ) == 0 && "Property data length is not a multiple of sizeof( typename ContainerType::value_type )" );

		auto container = ContainerType();
		container.Resize( container_element_count );
		assert( container.Size() == container_element_count );
		GetPropertyRawNoCheck(
			reply,
			data_length,
			container.Data()
		);
		free( reply );

		return container;
	}
};



} // xcb
} // window_manager
} // bc
