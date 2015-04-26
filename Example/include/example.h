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

#include "ZEGL.h"

using namespace ZEGL;

/**
* Example of inheriting the Game class to create a new game.
*/
class MyGame : public Game
{
public:
	MyGame();
	~MyGame();

	MyGame(MyGame const&) = delete;
	MyGame& operator=(MyGame const&) = delete;

	void Init(Window* window);
	void Update(float delta);
	void Render();
	
protected:
private:
	Light*				m_light;
	TileMap*			m_tileMap;

	SpriteBatch			m_spriteBatch;
	Sprite*				m_testSprite;
	AnimatedSprite*		m_testAnimSprite;

	Texture*			m_screenFBO;
	GLuint				m_screenVAO;
	GLuint				m_screenVBO;
	Shader				g_screenShader;
};