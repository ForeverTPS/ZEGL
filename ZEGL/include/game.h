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

#include "mymath.h"
#include <vector>

class Camera;
class Input;
class Light;
class Shader;
class Window;

class Game
{
public:
	Game();
	virtual ~Game();

	void Init(const Window& window);

	void ProcessInput(const Input& input, float delta);
	void Update(float delta);
	void Render();

	inline Light*	GetActiveLight()	{ return m_activeLight; }
	inline Camera*  GetCamera()			{ return m_camera; }

	inline Vector3f	GetAmbientColor()		const { return m_ambientColor; }
	inline float	GetAmbientIntensity()	const { return m_ambientIntensity; }

	inline const Window* GetWindow()	{ return m_window; }

protected:
private:
	Game(Game const&) = delete;
	Game& operator=(Game const&) = delete;

	Camera*				m_camera;
	const Window*		m_window;

	Vector3f			m_ambientColor;
	float				m_ambientIntensity;

	Shader*				m_defaultShader;
	Light*				m_activeLight;
	std::vector<Light*>	m_lights;
};

#endif
