#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <window_manager/event/WindowEventEnums.hpp>

#include <core/containers/Text.hpp>
#include <core/math/Vector.hpp>

#include <core/event/Event.hpp>



namespace bc {
namespace window_manager {



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// @brief
/// Class containing all window events that the window can emit.
class WindowEvents
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// File drop event.
	///
	/// @eventparam List<Text>
	/// List of files that were dropped on the window.
	Event<List<Text>&>															FileDrop;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Window close requested.
	///
	/// This event is signaled to indicate that the window should be closed.
	Event<>																		CloseRequested;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Window position changed.
	///
	/// @eventparam @ref math::Vec2i32
	/// New position of the window in OS virtual screen coordinates.
	Event<math::Vec2i32>														PositionChanged;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Window size changed.
	///
	/// @eventparam @ref math::Vec2i32
	/// New size of the window.
	Event<math::Vec2i32>														SizeChanged;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Window focus changed.
	///
	/// @eventparam bool
	/// True if window gained focus, false if window lost focus.
	Event<bool>																	FocusChanged;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Window state changed. Eg. minimized, maximized, fullscreen.
	///
	/// @eventparam @ref WindowState
	/// New current state of the window.
	Event<WindowState>															StateChanged;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Mouse button event.
	///
	/// @eventparam @ref MouseButton
	/// Mouse button that was pressed.
	/// \~
	/// @eventparam @ref MouseButtonAction
	/// Either MouseButtonAction::PRESSED or MouseButtonAction::RELEASED.
	/// \~
	/// @eventparam @ref ModifierKeyFlags
	/// Modifier keys that were pressed down during the event.
	Event<MouseButton, MouseButtonAction, ModifierKeyFlags>						MouseButton;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Mouse position changed.
	///
	/// @eventparam @ref math::Vec2f64
	/// Current mouse position on the window framebuffer area.
	Event<math::Vec2f64>														MousePosition;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Mouse scroll event.
	///
	/// @eventparam @ref math::Vec2f64
	/// Mouse scroll delta. Eg. <tt>{ 0.0f, 0.0f }</tt> if no scroll happened, <tt>{ 0.0f, 1.0f }</tt> if scrolled down,
	/// <tt>{ 1.0f, 0.0f }</tt> if scrolled right. Typically only the y axis is used.
	Event<math::Vec2f64>														MouseScroll;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Used to check if mouse entered or exited the window area.
	///
	/// @eventparam @ref bool
	/// True if mouse entered, false if mouse exited.
	Event<bool>																	MouseEnter;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Use this for keyboard input for player control.
	/// 
	/// Keyboard input event returning individual key presses.
	///
	/// @eventparam @ref KeyboardButton
	/// Keyboard button that was pressed.
	/// \~
	/// @eventparam @ref i32
	/// Scancode of the key that was pressed.
	/// \~
	/// @eventparam @ref KeyboardButtonAction
	/// Action of what happened with the key. Eg. pressed, released, repeating.
	/// \~
	/// @eventparam @ref ModifierKeyFlags
	/// Modifier keys that were pressed down during the event.
	Event<KeyboardButton, i32, KeyboardButtonAction, ModifierKeyFlags>			KeyboardKey;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief
	/// Use this for keyboard text input.
	/// 
	///	Keyboard input event returning unicode characters. Returns unicode character.
	/// 
	/// @eventparam @ref c32
	/// Unicode character.
	Event<c32>																	KeyboardCharacter;
};



} // window
} // bc
