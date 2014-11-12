/*  ____ ____ ____ ____
 * ||Z |||E |||G |||L ||
 * ||__|||__|||__|||__||
 * |/__\|/__\|/__\|/__\|
 *
 * Copyright(c) 2014, Phil Sampson
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

#ifndef INPUT_H
#define INPUT_H

#include "glm/glm.hpp"

class Window;

class Input
{
public:
	enum
	{
		MOUSE_LEFT_BUTTON = 1,
		MOUSE_MIDDLE_BUTTON = 2,
		MOUSE_RIGHT_BUTTON = 3,
		MOUSE_WHEEL_UP = 4,
		MOUSE_WHEEL_DOWN = 5
	};

	enum
	{
		KEY_UNKNOWN = 0,

		KEY_A = 4,
		KEY_B = 5,
		KEY_C = 6,
		KEY_D = 7,
		KEY_E = 8,
		KEY_F = 9,
		KEY_G = 10,
		KEY_H = 11,
		KEY_I = 12,
		KEY_J = 13,
		KEY_K = 14,
		KEY_L = 15,
		KEY_M = 16,
		KEY_N = 17,
		KEY_O = 18,
		KEY_P = 19,
		KEY_Q = 20,
		KEY_R = 21,
		KEY_S = 22,
		KEY_T = 23,
		KEY_U = 24,
		KEY_V = 25,
		KEY_W = 26,
		KEY_X = 27,
		KEY_Y = 28,
		KEY_Z = 29,

		KEY_1 = 30,
		KEY_2 = 31,
		KEY_3 = 32,
		KEY_4 = 33,
		KEY_5 = 34,
		KEY_6 = 35,
		KEY_7 = 36,
		KEY_8 = 37,
		KEY_9 = 38,
		KEY_0 = 39,

		KEY_RETURN = 40,
		KEY_ESCAPE = 41,
		KEY_BACKSPACE = 42,
		KEY_TAB = 43,
		KEY_SPACE = 44,

		KEY_MINUS = 45,
		KEY_EQUALS = 46,
		KEY_LEFTBRACKET = 47,
		KEY_RIGHTBRACKET = 48,
		KEY_BACKSLASH = 49,	  /**< Located at the lower left of the return
								*   key on ISO keyboards and at the right end
								*   of the QWERTY row on ANSI keyboards.
								*   Produces REVERSE SOLIDUS (backslash) and
								*   VERTICAL LINE in a US layout, REVERSE
								*   SOLIDUS and VERTICAL LINE in a UK Mac
								*   layout, NUMBER SIGN and TILDE in a UK
								*   Windows layout, DOLLAR SIGN and POUND SIGN
								*   in a Swiss German layout, NUMBER SIGN and
								*   APOSTROPHE in a German layout, GRAVE
								*   ACCENT and POUND SIGN in a French Mac
								*   layout, and ASTERISK and MICRO SIGN in a
								*   French Windows layout.
								*/
		KEY_NONUSHASH = 50,	  /**< ISO USB keyboards actually use this code
								*   instead of 49 for the same key, but all
								*   OSes I've seen treat the two codes
								*   identically. So, as an implementor, unless
								*   your keyboard generates both of those
								*   codes and your OS treats them differently,
								*   you should generate KEY_BACKSLASH
								*   instead of this code. As a user, you
								*   should not rely on this code because SDL
								*   will never generate it with most (all?)
								*   keyboards.
								*/
		KEY_SEMICOLON = 51,
		KEY_APOSTROPHE = 52,
		KEY_GRAVE = 53,		  /**< Located in the top left corner (on both ANSI
								*   and ISO keyboards). Produces GRAVE ACCENT and
								*   TILDE in a US Windows layout and in US and UK
								*   Mac layouts on ANSI keyboards, GRAVE ACCENT
								*   and NOT SIGN in a UK Windows layout, SECTION
								*   SIGN and PLUS-MINUS SIGN in US and UK Mac
								*   layouts on ISO keyboards, SECTION SIGN and
								*   DEGREE SIGN in a Swiss German layout (Mac:
								*   only on ISO keyboards), CIRCUMFLEX ACCENT and
								*   DEGREE SIGN in a German layout (Mac: only on
								*   ISO keyboards), SUPERSCRIPT TWO and TILDE in a
								*   French Windows layout, COMMERCIAL AT and
								*   NUMBER SIGN in a French Mac layout on ISO
								*   keyboards, and LESS-THAN SIGN and GREATER-THAN
								*   SIGN in a Swiss German, German, or French Mac
								*   layout on ANSI keyboards.
								*/
		KEY_COMMA = 54,
		KEY_PERIOD = 55,
		KEY_SLASH = 56,

		KEY_CAPSLOCK = 57,

		KEY_F1 = 58,
		KEY_F2 = 59,
		KEY_F3 = 60,
		KEY_F4 = 61,
		KEY_F5 = 62,
		KEY_F6 = 63,
		KEY_F7 = 64,
		KEY_F8 = 65,
		KEY_F9 = 66,
		KEY_F10 = 67,
		KEY_F11 = 68,
		KEY_F12 = 69,

		KEY_PRINTSCREEN = 70,
		KEY_SCROLLLOCK = 71,
		KEY_PAUSE = 72,
		KEY_INSERT = 73,	  /**< insert on PC, help on some Mac keyboards (but
								*   does send code 73, not 117) 
								*/
		KEY_HOME = 74,
		KEY_PAGEUP = 75,
		KEY_DELETE = 76,
		KEY_END = 77,
		KEY_PAGEDOWN = 78,
		KEY_RIGHT = 79,
		KEY_LEFT = 80,
		KEY_DOWN = 81,
		KEY_UP = 82,

		KEY_NUMLOCKCLEAR = 83, /**< num lock on PC, clear on Mac keyboards */

		KEY_KP_DIVIDE = 84,
		KEY_KP_MULTIPLY = 85,
		KEY_KP_MINUS = 86,
		KEY_KP_PLUS = 87,
		KEY_KP_ENTER = 88,
		KEY_KP_1 = 89,
		KEY_KP_2 = 90,
		KEY_KP_3 = 91,
		KEY_KP_4 = 92,
		KEY_KP_5 = 93,
		KEY_KP_6 = 94,
		KEY_KP_7 = 95,
		KEY_KP_8 = 96,
		KEY_KP_9 = 97,
		KEY_KP_0 = 98,
		KEY_KP_PERIOD = 99,

		KEY_NONUSBACKSLASH = 100, /**< This is the additional key that ISO
									*   keyboards have over ANSI ones,
									*   located between left shift and Y.
									*   Produces GRAVE ACCENT and TILDE in a
									*   US or UK Mac layout, REVERSE SOLIDUS
									*   (backslash) and VERTICAL LINE in a
									*   US or UK Windows layout, and
									*   LESS-THAN SIGN and GREATER-THAN SIGN
									*   in a Swiss German, German, or French
									*   layout. */

		KEY_LCTRL = 224,
		KEY_LSHIFT = 225,
		KEY_LALT = 226,		/**< alt, option */
		KEY_LGUI = 227,		/**< windows, command (apple), meta */
		KEY_RCTRL = 228,
		KEY_RSHIFT = 229,
		KEY_RALT = 230,		/**< alt gr, option */
		KEY_RGUI = 231,		/**< windows, command (apple), meta */
	};

	static const int NUM_KEYS = 512;
	static const int NUM_MOUSEBUTTONS = 256;

	Input(Window* window);

	inline bool			GetKey(int keyCode)				const { return m_inputs[keyCode]; }
	inline bool			GetKeyDown(int keyCode)			const { return m_downKeys[keyCode]; }
	inline bool			GetKeyUp(int keyCode)			const { return m_upKeys[keyCode]; }
	inline bool			GetMouse(int keyCode)			const { return m_mouseInput[keyCode]; }
	inline bool			GetMouseDown(int keyCode)		const { return m_downMouse[keyCode]; }
	inline bool			GetMouseUp(int keyCode)			const { return m_upMouse[keyCode]; }
	inline glm::vec2	GetMousePosition()				const { return glm::vec2((float)m_mouseX, (float)m_mouseY); }

	void SetCursor(bool value)					const;
	void SetMousePosition(const glm::vec2& pos)	const;

	inline void SetKey(int keyCode, bool value)       { m_inputs[keyCode] = value; }
	inline void SetKeyDown(int keyCode, bool value)   { m_downKeys[keyCode] = value; }
	inline void SetKeyUp(int keyCode, bool value)     { m_upKeys[keyCode] = value; }
	inline void SetMouse(int keyCode, bool value)     { m_mouseInput[keyCode] = value; }
	inline void SetMouseDown(int keyCode, bool value) { m_downMouse[keyCode] = value; }
	inline void SetMouseUp(int keyCode, bool value)   { m_upMouse[keyCode] = value; }
	inline void SetMouseX(int value)                  { m_mouseX = value; }
	inline void SetMouseY(int value)                  { m_mouseY = value; }

protected:
private:
	bool	m_inputs[NUM_KEYS];
	bool	m_downKeys[NUM_KEYS];
	bool	m_upKeys[NUM_KEYS];
	bool	m_mouseInput[NUM_MOUSEBUTTONS];
	bool	m_downMouse[NUM_MOUSEBUTTONS];
	bool	m_upMouse[NUM_MOUSEBUTTONS];
	int		m_mouseX;
	int		m_mouseY;
	Window* m_window;
};

#endif
