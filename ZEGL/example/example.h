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

#include "game.h"
#include "tilemap.h"

using namespace ZEGL;

/**
* Example of inherting the Game class to create a new game.
*/
class MyGame : public Game
{
public:
	MyGame();
	~MyGame();

	void Init(Window* window);
	void Update(float delta);
	void Render();
	
protected:
private:
	MyGame(MyGame const&) = delete;
	MyGame& operator=(MyGame const&) = delete;

	Light*		m_light;
	TileMap*	m_tileMap;
};