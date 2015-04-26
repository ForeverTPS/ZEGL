/*
* Copyright(c) 2014, Phil Sampson (http://www.zamma.co.uk)
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#pragma once

#include <glm/glm.hpp>
#include <SDL.h>
#include <string>

namespace ZEGL
{
#define MAX_KEYS			512		/*!< The maximum number of keyboard keys */
#define MAX_MOUSE_BUTTONS	6		/*!< The maximum number of mouse buttons */

	class Window;

	/**
	* Manages the state of keyboard and mouse input. The class maintains 3 arrays 
	* which hold true or false values. These arrays represent if the key/button was 
	* just pressed in the current frame, if the key/button was pressed in the previous 
	* frame and also if the key/button was just released in the frame. By combining them
	* the class is able to report if key/mouse button was just pressed, is currently held 
	* or was just released. The cursor can also be manipulated (position, visibility).
	*/
	class Input
	{
	public:
		/**
		* Remap of the SDL mouse button codes
		*/
		enum
		{
			MOUSE_LEFT_BUTTON = SDL_BUTTON_LEFT,				/*!< Left mouse button */
			MOUSE_MIDDLE_BUTTON = SDL_BUTTON_MIDDLE,			/*!< Middle mouse button */
			MOUSE_RIGHT_BUTTON = SDL_BUTTON_RIGHT,				/*!< Right mouse button */
			MOUSE_WHEEL_UP = SDL_BUTTON_X1,						/*!< Mouse wheel up */
			MOUSE_WHEEL_DOWN = SDL_BUTTON_X2					/*!< Mouse wheel down */
		};

		/**
		* Remap of the SDL key codes
		*/
		enum
		{
			KEY_UNKOWN = SDLK_UNKNOWN,							/*!< Unknown key value */

			KEY_RETURN = SDLK_RETURN,							/*!< . */
			KEY_ESCAPE = SDLK_ESCAPE,							/*!< . */
			KEY_BACKSPACE = SDLK_BACKSPACE,						/*!< . */
			KEY_TAB = SDLK_TAB,									/*!< . */
			KEY_SPACE = SDLK_SPACE,								/*!< . */
			KEY_EXCLAIM = SDLK_EXCLAIM,							/*!< . */
			KEY_QUOTEDBL = SDLK_QUOTEDBL,						/*!< . */
			KEY_HASH = SDLK_HASH,								/*!< . */
			KEY_PERCENT = SDLK_PERCENT,							/*!< . */
			KEY_DOLLAR = SDLK_DOLLAR,							/*!< . */
			KEY_AMPERSAND = SDLK_AMPERSAND,						/*!< . */
			KEY_QUOTE = SDLK_QUOTE,								/*!< . */
			KEY_LEFTPAREN = SDLK_LEFTPAREN,						/*!< . */
			KEY_RIGHTPAREN = SDLK_RIGHTPAREN,					/*!< . */
			KEY_ASTERISK = SDLK_ASTERISK,						/*!< . */
			KEY_PLUS = SDLK_PLUS,								/*!< . */
			KEY_COMMA = SDLK_COMMA,								/*!< . */
			KEY_MINUS = SDLK_MINUS,								/*!< . */
			KEY_PERIOD = SDLK_PERIOD,							/*!< . */
			KEY_SLASH = SDLK_SLASH,								/*!< . */
			KEY_0 = SDLK_0,										/*!< . */
			KEY_1 = SDLK_1,										/*!< . */
			KEY_2 = SDLK_2,										/*!< . */
			KEY_3 = SDLK_3,										/*!< . */
			KEY_4 = SDLK_4,										/*!< . */
			KEY_5 = SDLK_5,										/*!< . */
			KEY_6 = SDLK_6,										/*!< . */
			KEY_7 = SDLK_7,										/*!< . */
			KEY_8 = SDLK_8,										/*!< . */
			KEY_9 = SDLK_9,										/*!< . */
			KEY_COLON = SDLK_COLON,								/*!< . */
			KEY_SEMICOLON = SDLK_SEMICOLON,						/*!< . */
			KEY_LESS = SDLK_LESS,								/*!< . */
			KEY_EQUALS = SDLK_EQUALS,							/*!< . */
			KEY_GREATER = SDLK_GREATER,							/*!< . */
			KEY_QUESTION = SDLK_QUESTION,						/*!< . */
			KEY_AT = SDLK_AT,									/*!< . */
																	 
			// Skip uppercase letters								 
																	 
			KEY_LEFTBRACKET = SDLK_LEFTBRACKET,					/*!< . */
			KEY_BACKSLASH = SDLK_BACKSLASH,						/*!< . */
			KEY_RIGHTBRACKET = SDLK_RIGHTBRACKET,				/*!< . */
			KEY_CARET = SDLK_CARET,								/*!< . */
			KEY_UNDERSCORE = SDLK_UNDERSCORE,					/*!< . */
			KEY_BACKQUOTE = SDLK_BACKQUOTE,						/*!< . */
			KEY_A = SDLK_a,										/*!< . */
			KEY_B = SDLK_b,										/*!< . */
			KEY_C = SDLK_c,										/*!< . */
			KEY_D = SDLK_d,										/*!< . */
			KEY_E = SDLK_e,										/*!< . */
			KEY_F = SDLK_f,										/*!< . */
			KEY_G = SDLK_g,										/*!< . */
			KEY_H = SDLK_h,										/*!< . */
			KEY_I = SDLK_i,										/*!< . */
			KEY_J = SDLK_j,										/*!< . */
			KEY_K = SDLK_k,										/*!< . */
			KEY_L = SDLK_l,										/*!< . */
			KEY_M = SDLK_m,										/*!< . */
			KEY_N = SDLK_n,										/*!< . */
			KEY_O = SDLK_o,										/*!< . */
			KEY_P = SDLK_p,										/*!< . */
			KEY_Q = SDLK_q,										/*!< . */
			KEY_R = SDLK_r,										/*!< . */
			KEY_S = SDLK_s,										/*!< . */
			KEY_T = SDLK_t,										/*!< . */
			KEY_U = SDLK_u,										/*!< . */
			KEY_V = SDLK_v,										/*!< . */
			KEY_W = SDLK_w,										/*!< . */
			KEY_X = SDLK_x,										/*!< . */
			KEY_Y = SDLK_y,										/*!< . */
			KEY_Z = SDLK_z,										/*!< . */
																	 
			KEY_CAPSLOCK = SDLK_CAPSLOCK,						/*!< . */
																	 
			KEY_F1 = SDLK_F1,									/*!< . */
			KEY_F2 = SDLK_F2,									/*!< . */
			KEY_F3 = SDLK_F3,									/*!< . */
			KEY_F4 = SDLK_F4,									/*!< . */
			KEY_F5 = SDLK_F5,									/*!< . */
			KEY_F6 = SDLK_F6,									/*!< . */
			KEY_F7 = SDLK_F7,									/*!< . */
			KEY_F8 = SDLK_F8,									/*!< . */
			KEY_F9 = SDLK_F9,									/*!< . */
			KEY_F10 = SDLK_F10,									/*!< . */
			KEY_F11 = SDLK_F11,									/*!< . */
			KEY_F12 = SDLK_F12,									/*!< . */
																	 
			KEY_PRINTSCREEN = SDLK_PRINTSCREEN,					/*!< . */
			KEY_SCROLLLOCK = SDLK_SCROLLLOCK,					/*!< . */
			KEY_PAUSE = SDLK_PAUSE,								/*!< . */
			KEY_INSERT = SDLK_INSERT,							/*!< . */
			KEY_HOME = SDLK_HOME,								/*!< . */
			KEY_PAGEUP = SDLK_PAGEUP,							/*!< . */
			KEY_DELETE = SDLK_DELETE,							/*!< . */
			KEY_END = SDLK_END,									/*!< . */
			KEY_PAGEDOWN = SDLK_PAGEDOWN,						/*!< . */
			KEY_RIGHT = SDLK_RIGHT,								/*!< . */
			KEY_LEFT = SDLK_LEFT,								/*!< . */
			KEY_DOWN = SDLK_DOWN,								/*!< . */
			KEY_UP = SDLK_UP,									/*!< . */
																	 
			KEY_NUMLOCKCLEAR = SDLK_NUMLOCKCLEAR,				/*!< . */
			KEY_KP_DIVIDE = SDLK_KP_DIVIDE,						/*!< . */
			KEY_KP_MULTIPLY = SDLK_KP_MULTIPLY,					/*!< . */
			KEY_KP_MINUS = SDLK_KP_MINUS,						/*!< . */
			KEY_KP_PLUS = SDLK_KP_PLUS,							/*!< . */
			KEY_KP_ENTER = SDLK_KP_ENTER,						/*!< . */
			KEY_KP_1 = SDLK_KP_1,								/*!< . */
			KEY_KP_2 = SDLK_KP_2,								/*!< . */
			KEY_KP_3 = SDLK_KP_3,								/*!< . */
			KEY_KP_4 = SDLK_KP_4,								/*!< . */
			KEY_KP_5 = SDLK_KP_5,								/*!< . */
			KEY_KP_6 = SDLK_KP_6,								/*!< . */
			KEY_KP_7 = SDLK_KP_7,								/*!< . */
			KEY_KP_8 = SDLK_KP_8,								/*!< . */
			KEY_KP_9 = SDLK_KP_9,								/*!< . */
			KEY_KP_0 = SDLK_KP_0,								/*!< . */
			KEY_KP_PERIOD = SDLK_KP_PERIOD,						/*!< . */
																	 
			KEY_APPLICATION = SDLK_APPLICATION,					/*!< . */
			KEY_POWER = SDLK_POWER,								/*!< . */
			KEY_KP_EQUALS = SDLK_KP_EQUALS,						/*!< . */
			KEY_F13 = SDLK_F13,									/*!< . */
			KEY_F14 = SDLK_F14,									/*!< . */
			KEY_F15 = SDLK_F15,									/*!< . */
			KEY_F16 = SDLK_F16,									/*!< . */
			KEY_F17 = SDLK_F17,									/*!< . */
			KEY_F18 = SDLK_F18,									/*!< . */
			KEY_F19 = SDLK_F19,									/*!< . */
			KEY_F20 = SDLK_F20,									/*!< . */
			KEY_F21 = SDLK_F21,									/*!< . */
			KEY_F22 = SDLK_F22,									/*!< . */
			KEY_F23 = SDLK_F23,									/*!< . */
			KEY_F24 = SDLK_F24,									/*!< . */
			KEY_EXECUTE = SDLK_EXECUTE,							/*!< . */
			KEY_HELP = SDLK_HELP,								/*!< . */
			KEY_MENU = SDLK_MENU,								/*!< . */
			KEY_SELECT = SDLK_SELECT,							/*!< . */
			KEY_STOP = SDLK_STOP,								/*!< . */
			KEY_AGAIN = SDLK_AGAIN,								/*!< . */
			KEY_UNDO = SDLK_UNDO,								/*!< . */
			KEY_CUT = SDLK_CUT,									/*!< . */
			KEY_COPY = SDLK_COPY,								/*!< . */
			KEY_PASTE = SDLK_PASTE,								/*!< . */
			KEY_FIND = SDLK_FIND,								/*!< . */
			KEY_MUTE = SDLK_MUTE,								/*!< . */
			KEY_VOLUMEUP = SDLK_VOLUMEUP,						/*!< . */
			KEY_VOLUMEDOWN = SDLK_VOLUMEDOWN,					/*!< . */
			KEY_KP_COMMA = SDLK_KP_COMMA,						/*!< . */
			KEY_KP_EQUALSAS400 = SDLK_KP_EQUALSAS400,			/*!< . */
																	 
			KEY_ALTERASE = SDLK_ALTERASE,						/*!< . */
			KEY_SYSREQ = SDLK_SYSREQ,							/*!< . */
			KEY_CANCEL = SDLK_CANCEL,							/*!< . */
			KEY_CLEAR = SDLK_CLEAR,								/*!< . */
			KEY_PRIOR = SDLK_PRIOR,								/*!< . */
			KEY_RETURN2 = SDLK_RETURN2,							/*!< . */
			KEY_SEPARATOR = SDLK_SEPARATOR,						/*!< . */
			KEY_OUT = SDLK_OUT,									/*!< . */
			KEY_OPER = SDLK_OPER,								/*!< . */
			KEY_CLEARAGAIN = SDLK_CLEARAGAIN,					/*!< . */
			KEY_CRSEL = SDLK_CRSEL,								/*!< . */
			KEY_EXSEL = SDLK_EXSEL,								/*!< . */
																	 
			KEY_KP_00 = SDLK_KP_00,								/*!< . */
			KEY_KP_000 = SDLK_KP_000,							/*!< . */
			KEY_THOUSANDSSEPARATOR = SDLK_THOUSANDSSEPARATOR,	/*!< . */
			KEY_DECIMALSEPARATOR = SDLK_DECIMALSEPARATOR,		/*!< . */
			KEY_CURRENCYUNIT = SDLK_CURRENCYUNIT,				/*!< . */
			KEY_CURRENCYSUBUNIT = SDLK_CURRENCYSUBUNIT,			/*!< . */
			KEY_KP_LEFTPAREN = SDLK_KP_LEFTPAREN,				/*!< . */
			KEY_KP_RIGHTPAREN = SDLK_KP_RIGHTPAREN,				/*!< . */
			KEY_KP_LEFTBRACE = SDLK_KP_LEFTBRACE,				/*!< . */
			KEY_KP_RIGHTBRACE = SDLK_KP_RIGHTBRACE,				/*!< . */
			KEY_KP_TAB = SDLK_KP_TAB,							/*!< . */
			KEY_KP_BACKSPACE = SDLK_KP_BACKSPACE,				/*!< . */
			KEY_KP_A = SDLK_KP_A,								/*!< . */
			KEY_KP_B = SDLK_KP_B,								/*!< . */
			KEY_KP_C = SDLK_KP_C,								/*!< . */
			KEY_KP_D = SDLK_KP_D,								/*!< . */
			KEY_KP_E = SDLK_KP_E,								/*!< . */
			KEY_KP_F = SDLK_KP_F,								/*!< . */
			KEY_KP_XOR = SDLK_KP_XOR,							/*!< . */
			KEY_KP_POWER = SDLK_KP_POWER,						/*!< . */
			KEY_KP_PERCENT = SDLK_KP_PERCENT,					/*!< . */
			KEY_KP_LESS = SDLK_KP_LESS,							/*!< . */
			KEY_KP_GREATER = SDLK_KP_GREATER,					/*!< . */
			KEY_KP_AMPERSAND = SDLK_KP_AMPERSAND,				/*!< . */
			KEY_KP_DBLAMPERSAND = SDLK_KP_DBLAMPERSAND,			/*!< . */
			KEY_KP_VERTICALBAR = SDLK_KP_VERTICALBAR,			/*!< . */
			KEY_KP_DBLVERTICALBAR = SDLK_KP_DBLVERTICALBAR,		/*!< . */
			KEY_KP_COLON = SDLK_KP_COLON,						/*!< . */
			KEY_KP_HASH = SDLK_KP_HASH,							/*!< . */
			KEY_KP_SPACE = SDLK_KP_SPACE,						/*!< . */
			KEY_KP_AT = SDLK_KP_AT,								/*!< . */
			KEY_KP_EXCLAM = SDLK_KP_EXCLAM,						/*!< . */
			KEY_KP_MEMSTORE = SDLK_KP_MEMSTORE,					/*!< . */
			KEY_KP_MEMRECALL = SDLK_KP_MEMRECALL,				/*!< . */
			KEY_KP_MEMCLEAR = SDLK_KP_MEMCLEAR,					/*!< . */
			KEY_KP_MEMADD = SDLK_KP_MEMADD,						/*!< . */
			KEY_KP_MEMSUBTRACT = SDLK_KP_MEMSUBTRACT,			/*!< . */
			KEY_KP_MEMMULTIPLY = SDLK_KP_MEMMULTIPLY,			/*!< . */
			KEY_KP_MEMDIVIDE = SDLK_KP_MEMDIVIDE,				/*!< . */
			KEY_KP_PLUSMINUS = SDLK_KP_PLUSMINUS,				/*!< . */
			KEY_KP_CLEAR = SDLK_KP_CLEAR,						/*!< . */
			KEY_KP_CLEARENTRY = SDLK_KP_CLEARENTRY,				/*!< . */
			KEY_KP_BINARY = SDLK_KP_BINARY,						/*!< . */
			KEY_KP_OCTAL = SDLK_KP_OCTAL,						/*!< . */
			KEY_KP_DECIMAL = SDLK_KP_DECIMAL,					/*!< . */
			KEY_KP_HEXADECIMAL = SDLK_KP_HEXADECIMAL,			/*!< . */
																	 
			KEY_LCTRL = SDLK_LCTRL,								/*!< . */
			KEY_LSHIFT = SDLK_LSHIFT,							/*!< . */
			KEY_LALT = SDLK_LALT,								/*!< . */
			KEY_LGUI = SDLK_LGUI,								/*!< . */
			KEY_RCTRL = SDLK_RCTRL,								/*!< . */
			KEY_RSHIFT = SDLK_RSHIFT,							/*!< . */
			KEY_RALT = SDLK_RALT,								/*!< . */
			KEY_RGUI = SDLK_RGUI,								/*!< . */
																	 
			KEY_MODE = SDLK_MODE,								/*!< . */
																	 
			KEY_AUDIONEXT = SDLK_AUDIONEXT,						/*!< . */
			KEY_AUDIOPREV = SDLK_AUDIOPREV,						/*!< . */
			KEY_AUDIOSTOP = SDLK_AUDIOSTOP,						/*!< . */
			KEY_AUDIOPLAY = SDLK_AUDIOPLAY,						/*!< . */
			KEY_AUDIOMUTE = SDLK_AUDIOMUTE,						/*!< . */
			KEY_MEDIASELECT = SDLK_MEDIASELECT,					/*!< . */
			KEY_WWW = SDLK_WWW,									/*!< . */
			KEY_MAIL = SDLK_MAIL,								/*!< . */
			KEY_CALCULATOR = SDLK_CALCULATOR,					/*!< . */
			KEY_COMPUTER = SDLK_COMPUTER,						/*!< . */
			KEY_AC_SEARCH = SDLK_AC_SEARCH,						/*!< . */
			KEY_AC_HOME = SDLK_AC_HOME,							/*!< . */
			KEY_AC_BACK = SDLK_AC_BACK,							/*!< . */
			KEY_AC_FORWARD = SDLK_AC_FORWARD,					/*!< . */
			KEY_AC_STOP = SDLK_AC_STOP,							/*!< . */
			KEY_AC_REFRESH = SDLK_AC_REFRESH,					/*!< . */
			KEY_AC_BOOKMARKS = SDLK_AC_BOOKMARKS,				/*!< . */
																	 
			KEY_BRIGHTNESSDOWN = SDLK_BRIGHTNESSDOWN,			/*!< . */
			KEY_BRIGHTNESSUP = SDLK_BRIGHTNESSUP,				/*!< . */
			KEY_DISPLAYDWITCH = SDLK_DISPLAYSWITCH,				/*!< . */
			KEY_KBDILLUMTOGGLE = SDLK_KBDILLUMTOGGLE,			/*!< . */
			KEY_KBDILLUMDOWN = SDLK_KBDILLUMDOWN,				/*!< . */
			KEY_KBDILLUMUP = SDLK_KBDILLUMUP,					/*!< . */
			KEY_EJECT = SDLK_EJECT,								/*!< . */
			KEY_SLEEP = SDLK_SLEEP								/*!< . */
		};

		/**
		* Constructor
		*
		* Initializes all key and button monitoring arrays to be default state
		* which is false.
		*/
		Input();

		Input(Input const&) = delete;
		Input& operator=(Input const&) = delete;

		/**
		* Test if a specific key is held down.
		*
		* \param[in] keyCode The enum value representing the desired key
		*
		* \return True if the key is held down otherwise false
		*/
		inline bool GetKey(int keyCode) const { return m_inputs[keyCode]; }

		/**
		* Set the value of the array used for keys which are held down.
		*
		* \param[in] keyCode Index value for the array using KeyboardKey
		* \param[in] value The state to set the array value to - true/false
		*
		* \see [KeyboardKey]
		*/
		inline void SetKey(int keyCode, bool value) { m_inputs[keyCode] = value; }

		/**
		* Test if a specific key was just pressed this frame.
		*
		* \param[in] keyCode The enum value representing the desired key
		*
		* \return True if the key was just pressed otherwise false
		*/
		inline bool	GetKeyDown(int keyCode) const { return m_downKeys[keyCode]; }

		/**
		* Set the value of the array used for keys which have just been pressed.
		*
		* \param[in] keyCode Index value for the array using KeyboardKey
		* \param[in] value The state to set the array value to - true/false
		*
		* \see [KeyboardKey]
		*/
		inline void SetKeyDown(int keyCode, bool value) { m_downKeys[keyCode] = value; }

		/**
		* Test if a specific key was just released this frame.
		*
		* \param[in] keyCode The enum value representing the desired key
		*
		* \return True if the key was just released otherwise false
		*/
		inline bool	GetKeyUp(int keyCode) const { return m_upKeys[keyCode]; }

		/**
		* Set the value of the array used for keys which have just been released.
		*
		* \param[in] keyCode Index value for the array using KeyboardKey
		* \param[in] value The state to set the array value to - true/false
		*
		* \see [KeyboardKey]
		*/
		inline void SetKeyUp(int keyCode, bool value) { m_upKeys[keyCode] = value; }

		/**
		* Test if a specific mouse button is held down.
		*
		* \param[in] keyCode The enum value representing the desired mouse button
		*
		* \return True if the mouse is held down otherwise false
		*/
		inline bool	GetMouse(int keyCode) const { return m_mouseInput[keyCode]; }

		/**
		* Set the value of the array used for mouse buttons which are held down.
		*
		* \param[in] keyCode Index value for the array using MouseButton
		* \param[in] value The state to set the array value to - true/false
		*
		* \see [MouseButton]
		*/
		inline void SetMouse(int keyCode, bool value) { m_mouseInput[keyCode] = value; }

		/**
		* Test if a specific mouse button was just pressed this frame.
		*
		* \param[in] keyCode The enum value representing the desired mouse button
		*
		* \return True if the mouse button was just pressed otherwise false
		*/
		inline bool	GetMouseDown(int keyCode) const { return m_downMouse[keyCode]; }

		/**
		* Set the value of the array used for mouse buttons which have just been pressed.
		*
		* \param[in] keyCode Index value for the array using MouseButton
		* \param[in] value The state to set the array value to - true/false
		*
		* \see [MouseButton]
		*/
		inline void SetMouseDown(int keyCode, bool value) { m_downMouse[keyCode] = value; }

		/**
		* Test if a specific mouse button was just released this frame.
		*
		* \param[in] keyCode The enum value representing the desired mouse button
		*
		* \return True if the mouse button was just released otherwise false
		*/
		inline bool	GetMouseUp(int keyCode) const { return m_upMouse[keyCode]; }

		/**
		* Set the value of the array used for mouse buttons which have just been released.
		*
		* \param[in] keyCode Index value for the array using MouseButton
		* \param[in] value The state to set the array value to - true/false
		*
		* \see [MouseButton]
		*/
		inline void SetMouseUp(int keyCode, bool value) { m_upMouse[keyCode] = value; }

		/**
		* Get the position of the cursor on screen.
		*
		* \return The screen coordinates of the cursor in a glm::vec2
		*/
		inline glm::vec2 GetMousePosition() const { return glm::vec2(m_mouseX, m_mouseY); }

		/**
		* Set the position of the mouse cursor in the Window.
		*
		* \param[in] window The Window in which to position the cursor
		* \param[in] x The new x coordinate
		* \param[in] y The new y coordinate
		*/
		void SetMousePosition(Window* window, int x, int y);

		/**
		* Set the x coordinate of the mouse cursor in the Window.
		*
		* \param[in] value The new x coordinate
		*/
		inline void SetMouseX(int value) { m_mouseX = value; }

		/**
		* Set the y coordinate of the mouse cursor in the Window.
		*
		* \param[in] value The new y coordinate
		*/
		inline void SetMouseY(int value) { m_mouseY = value; }
		
		/**
		* Hide or display the mouse cursor.
		*
		* \param[in] visible Flag whether to show or hide the cursor (true/false)
		*/
		void SetCursorVisible(bool visible) const;
		
		/**
		* Reset the array monitoring keys which have just been pressed to all be false.
		*/
		inline void ResetAllKeyDown() { memset(m_downKeys, 0, MAX_KEYS * sizeof(bool)); }

		/**
		* Reset the array monitoring keys which have just been released to all be false.
		*/
		inline void ResetAllKeyUp() { memset(m_upKeys, 0, MAX_KEYS * sizeof(bool)); }

		/**
		* Reset the array monitoring keys which are held down to all be false.
		*/
		inline void ResetAllKey() { memset(m_inputs, 0, MAX_KEYS * sizeof(bool)); }

		/**
		* Reset the array monitoring mouse buttons which have just been pressed to all be false.
		*/
		inline void ResetAllMouseButtonDown() { memset(m_downMouse, 0, MAX_MOUSE_BUTTONS * sizeof(bool)); }

		/**
		* Reset the array monitoring mouse buttons which have just been released to all be false.
		*/
		inline void ResetAllMouseButtonUp() { memset(m_upMouse, 0, MAX_MOUSE_BUTTONS * sizeof(bool)); }

		/**
		* Reset the array monitoring mouse buttons which are held down to all be false.
		*/
		inline void ResetAllMouseButton() {	memset(m_mouseInput, 0, MAX_MOUSE_BUTTONS * sizeof(bool)); }

	protected:
	private:
		bool		m_inputs[MAX_KEYS];					
		bool		m_downKeys[MAX_KEYS];				
		bool		m_upKeys[MAX_KEYS];					
		bool		m_mouseInput[MAX_MOUSE_BUTTONS];	
		bool		m_downMouse[MAX_MOUSE_BUTTONS];		
		bool		m_upMouse[MAX_MOUSE_BUTTONS];		
		int			m_mouseX;							
		int			m_mouseY;							
	};
}
