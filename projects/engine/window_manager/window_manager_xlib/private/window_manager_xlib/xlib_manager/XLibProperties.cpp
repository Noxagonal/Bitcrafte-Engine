
#include <window_manager_xlib/PreCompiledHeader.hpp>

#include <window_manager_xlib/xlib_manager/XLibProperties.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Atom bc::window_manager::xlib::GetAtom(
	::Display			*	display,
	bc::Text				atom_name,
	bool					create_if_not_exists
)
{
	return XInternAtom( display, atom_name.ToCStr(), !create_if_not_exists );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::xlib::ChangePropertyRaw(
	::Display			*	display,
	::Window				window,
	::Atom					atom_property,
	::Atom					type,
	PropertyFormat			format,
	uint32_t				data_length,
	const void			*	in_data_buffer
)
{
	auto format_bytes = std::to_underlying( format ) / 8;
	assert( data_length % format_bytes == 0 && "Data length is not a multiple of the format" );
	XChangeProperty(
		display,
		window,
		atom_property,
		type,
		static_cast<int>( format ),
		PropModeReplace,
		reinterpret_cast<const unsigned char*>( in_data_buffer ),
		data_length / format_bytes
	);
	XFlush( display );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void bc::window_manager::xlib::GetPropertyRaw(
	::Display			*	display,
	::Window				window,
	::Atom					atom_property,
	::Atom					type,
	PropertyFormat			format,
	uint32_t				data_length,
	void				*	out_data_buffer
)
{
	auto format_bytes = std::to_underlying<PropertyFormat>( format ) / 8;
	auto actual_type = ::Atom {};
	auto actual_format = int {};
	unsigned long nitems = {};
	unsigned long bytes_after = {};
	unsigned char * prop = nullptr;
	auto aligned_data_length = ( data_length - 1 ) / 4 + 1;

	auto status = XGetWindowProperty(
		display,
		window,
		atom_property,
		0, aligned_data_length,
		false,
		type,
		&actual_type,
		&actual_format,
		&nitems,
		&bytes_after,
		&prop
	);
	if( status != Success || !prop )
	{
		if( prop ) XFree( prop );
		return;
	}
	if( actual_type != type )
	{
		XFree( prop );
		return;
	}
	if( static_cast<PropertyFormat>( actual_format ) != format )
	{
		XFree( prop );
		return;
	}
	if( nitems * format_bytes != data_length )
	{
		XFree( prop );
		return;
	}

	memcpy( out_data_buffer, prop, data_length );
	XFree( prop );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t bc::window_manager::xlib::GetPropertyRawSize(
	::Display		*	display,
	::Window			window,
	::Atom				atom_property,
	::Atom				type,
	PropertyFormat		format
)
{
	auto format_bytes = std::to_underlying<PropertyFormat>( format ) / 8;
	auto actual_type = ::Atom {};
	auto actual_format = int {};
	unsigned long nitems = {};
	unsigned long bytes_after = {};
	unsigned char * prop = nullptr;

	auto status = XGetWindowProperty(
		display,
		window,
		atom_property,
		0, 0,
		false,
		type,
		&actual_type,
		&actual_format,
		&nitems,
		&bytes_after,
		&prop
	);
	if( prop ) XFree( prop );
	if( status != Success ) return {};
	if( actual_type != type ) return {};
	if( static_cast<PropertyFormat>( actual_format ) != format ) return {};
	return static_cast<uint32_t>( nitems * format_bytes ); // + bytes_after;
}
