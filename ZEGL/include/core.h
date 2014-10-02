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

#ifndef CORE_H
#define CORE_H

#include <string>

class Game;
class Window;

class Core
{
public:
	Core(double frameRate, Window* window, Game* game);
	virtual ~Core() {};

	void Start();
	void Stop();

protected:
private:
	Core(Core const&) = delete;
	Core& operator=(Core const&) = delete;
	
	Window*		m_window;
	Game*		m_game;
	bool		m_isRunning;
	double		m_frameTime;	
};

#endif
