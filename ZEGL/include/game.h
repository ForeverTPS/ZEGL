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

#ifndef GAME_H
#define GAME_H

#include "logfile.h"

class Camera;
class Input;
class Window;

class Game
{
public:
	Game() { LOG_INIT("ZEGL"); }
	virtual ~Game() { LOG_CLEANUP(); }

	void Init(const Window& window);

	void ProcessInput(const Input& input, float delta);
	void Update(float delta);
	void Render();

protected:
private:
	Game(Game const&) = delete;
	Game& operator=(Game const&) = delete;

	Camera*	m_camera;
};

#endif
