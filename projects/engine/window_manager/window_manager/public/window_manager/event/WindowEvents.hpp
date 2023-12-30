#pragma once

#include <build_configuration/BuildConfigurationComponent.hpp>

#include <window_manager/event/WindowEventEnums.hpp>

#include <core/containers/Text.hpp>
#include <core/math/linear_algebra/Vector.hpp>

#include <core/event/Event.hpp>



namespace bc {
namespace window_manager {



class WindowEvents
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<List<Text>&>															FileDrop;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<>																		CloseRequested;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<math::Vector2i>														PositionChanged;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<math::Vector2i>														SizeChanged;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<bool>																	FocusChanged;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<WindowState>															StateChanged;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<MouseButton, MouseButtonAction, ModifierKeyFlags>						MouseButton;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<math::Vector2d>														MousePosition;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<math::Vector2d>														MouseScroll;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Event<bool>																	MouseEnter;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief		Use this for keyboard input for player control.
	/// 
	///				Keyboard input event returning individual key presses. Returns
	///				Keyboard button, scancode of the key, action of what happened
	///				with the key and modifier keys pressed down at the same time.
	Event<KeyboardButton, int32_t, KeyboardButtonAction, ModifierKeyFlags>		KeyboardKey;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/// @brief		Use this for keyboard text input.
	/// 
	///				Keyboard input event returning unicode characters. Returns
	///				unicode character.
	Event<int32_t>																KeyboardCharacter;
};



} // window
} // bc
