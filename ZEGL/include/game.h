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

#include "shader.h"
#include <GL/glew.h>
#include <vector>
#include <algorithm>

class Camera;
class Input;
class Light;
class Window;

class Game
{
public:
	Game();
	virtual ~Game();

	static inline void	SetInstance(Game* game) { s_game = game; }
	static inline Game* GetInstance()			{ return s_game; }

	void Init(const Window& window);
	void LoadResources();

	void ProcessInput(const Input& input, float delta);
	void Update(float delta);

	void Draw();
	void DrawLight();

	inline void AddLight(Light* light) { m_lights.push_back(light); }

	inline const Light*		GetActiveLight()	const { return m_activeLight; }
	inline const Camera&	GetCamera()			const { return *m_camera; }

	inline const Window*	GetWindow()			const { return m_window; }

protected:
private:
	Game(Game const&) = delete;
	Game& operator=(Game const&) = delete;

	void InitShaders();
	void InitFrameBuffers();

	static Game*		s_game;

	Camera*				m_camera;
	const Window*		m_window;

	Shader				m_spriteShader;
	Shader				m_shadowMapShader;
	Shader				m_shadowRenderShader;

	const Light*		m_activeLight;
	std::vector<Light*>	m_lights;

	GLuint				m_VAO;
	GLuint				m_VBO;
	size_t				m_VBO_size;

	GLuint				m_occlusionFBO;
	GLuint				m_occlusionFBO_tex;

	GLuint				m_shadowMapFBO;
	GLuint				m_shadowMapFBO_tex;
};

#endif
