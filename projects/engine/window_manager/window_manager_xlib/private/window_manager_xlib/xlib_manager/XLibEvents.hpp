#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <window_manager/event/WindowEventEnums.hpp>

#include <X11/Xlib.h>



namespace bc {
namespace window_manager {

class XLibWindow;

namespace xlib {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void										SetInputFocus(
	const XLibWindow					&	window
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void										ReplyToPing(
	const XEvent						&	event,
	const XLibWindow					&	window
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ModifierKeyFlags							GetModifierKeyFlags(
	uint32_t								state
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MouseButton									GetMouseButton(
	uint32_t								button
);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
KeyboardButton								GetKeyboardButton(
	KeySym									keysym
);



} // namespace xlib
} // namespace window_manager
} // namespace bc
