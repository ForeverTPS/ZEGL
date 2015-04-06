/**
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

#include "input.h"
#include "window.h"

using namespace ZEGL;

Input::Input() : 
	m_mouseX(0),
	m_mouseY(0)
{
	ResetAllKeyDown();
	ResetAllKeyUp();
	ResetAllKey();
	ResetAllMouseButtonDown();
	ResetAllMouseButtonUp();
	ResetAllMouseButton();
}

void Input::SetCursorVisible(bool visible) const
{
	visible ? SDL_ShowCursor(1) : SDL_ShowCursor(0);

	if (visible)
	{
		SDL_ShowCursor(1);
	}
	else
	{
		SDL_ShowCursor(0);
	}
}

void Input::SetMousePosition(Window* window, int x, int y)
{
	SDL_WarpMouseInWindow(window->GetSDLWindow(), x, y);
	m_mouseX = x;
	m_mouseY = y;
}