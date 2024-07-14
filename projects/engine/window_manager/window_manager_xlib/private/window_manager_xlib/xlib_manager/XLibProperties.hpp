#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <core/containers/Text.hpp>
#include <core/math/Vector.hpp>

#include <X11/Xlib.h>



namespace bc {
namespace window_manager {
namespace xlib {



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
Atom										GetAtom(
	Display								*	display,
	Text									atom_name,
	bool									create_if_not_exists
);

enum class PropertyFormat
{
	F8		= 8,
	F16		= 16,
	F32		= 32
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void										ChangePropertyRaw(
	::Display							*	display,
	::Window								window,
	::Atom									atom_property,
	::Atom									type,
	PropertyFormat							format,
	uint32_t								data_length,
	const void							*	in_data_buffer
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void										GetPropertyRaw(
	::Display							*	display,
	::Window								window,
	::Atom									atom_property,
	::Atom									type,
	PropertyFormat							format,
	uint32_t								data_length,
	void								*	out_data_buffer
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
uint32_t									GetPropertyRawSize(
	::Display							*	display,
	::Window								window,
	::Atom									atom_property,
	::Atom									type,
	PropertyFormat							format
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename PropertyType>
void										ChangeProperty(
	::Display							*	display,
	::Window								window,
	::Atom									atom_property,
	::Atom									type,
	PropertyFormat							format,
	const PropertyType					&	data
)
{
	auto format_bytes = std::to_underlying( format ) / 8;
	uint32_t data_length = sizeof( PropertyType );
	assert( data_length % format_bytes == 0 && "Data length must be a multiple of format bytes" );
	ChangePropertyRaw(
		display,
		window,
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
	::Display							*	display,
	::Window								window,
	::Atom									atom_property,
	::Atom									type,
	PropertyFormat							format
)
{
	auto format_bytes = std::to_underlying( format ) / 8;
	uint32_t data_length = sizeof( PropertyType );
	assert( data_length % format_bytes == 0 && "Data length must be a multiple of format bytes" );

	PropertyType data = {};
	GetPropertyRaw(
		display,
		window,
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
		::Display						*	display,
		::Window							window,
		::Atom								atom_property,
		::Atom								type,
		PropertyFormat						format
	) :
		display( display ),
		window( window ),
		atom_property( atom_property ),
		type( type ),
		format( format )
	{}

protected:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	::Display							*	display;
	::Window								window;
	::Atom									atom_property;
	::Atom									type;
	PropertyFormat							format;
};

} // internal_


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Handle for XLib properties
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
		::Display						*	display,
		::Window							window,
		::Atom								atom_property,
		::Atom								type,
		PropertyFormat						format
	) :
		internal_::PropertyHandleBase( display, window, atom_property, type, format )
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
			display,
			window,
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
			display,
			window,
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
		::Display						*	display,
		::Window							window,
		::Atom								atom_property,
		::Atom								type,
		PropertyFormat						format
	) :
		internal_::PropertyHandleBase( display, window, atom_property, type, format )
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
			display,
			window,
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
		auto data_length = GetPropertyRawSize(
			display,
			window,
			atom_property,
			type
		);
		if( data_length == 0 )
		{
			assert( 0 && "Failed to get property, property size is 0" );
			return {};
		}

		auto format_bytes = std::to_underlying( format ) / 8;
		assert( data_length % format_bytes == 0 && "Data length must be a multiple of format bytes" );

		auto container_element_count = data_length / sizeof( typename ContainerType::value_type );
		assert( data_length % sizeof( typename ContainerType::value_type ) == 0 && "Property data length is not a multiple of sizeof( typename ContainerType::value_type )" );

		auto container = ContainerType();
		container.Resize( container_element_count );
		GetPropertyRaw(
			display,
			window,
			atom_property,
			type,
			format,
			data_length,
			container.Data()
		);
		return container;
	}
};



} // xlib
} // window_manager
} // bc
