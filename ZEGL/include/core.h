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

#pragma once

namespace ZEGL
{
	class Game;
	class Window;

	/**
	* This controls the running of the game engine and holds the game loop. The
	* game is started using Start() and stopped using Stop() which would be called
	* externally (usually from the entry point main())
	*/
	class Core
	{
	public:
		/**
		* Constructor
		*
		* In addition to initializing variables this also calls Game::Init()
		* to initialize the game class.
		*
		* \param[in] frameRate The desired FPS that the game should run at
		* \param[in] window Pointer to a pre-created Window
		* \param[in] game Pointer to the Game (or inherited game)
		*
		* \see [Game][Window]
		*/
		Core(double frameRate, Window* window, Game* game);

		/**
		* Starts the game timer and enters the game loop which will run until
		* Stop() is called.
		*/
		void Start();

		/**
		* Flags the game loop to stop
		*
		* The game loops runs continuously as long as m_isRunning is true. Setting
		* it to false will cause the game loops to exit and consequently the game
		* to exit.
		*/
		inline void Stop() { m_isRunning = false; }

	protected:
	private:
		Core(Core const&) = delete;
		Core& operator=(Core const&) = delete;

		Window*		m_window;		/*!< The Window used for rendering */
		Game*		m_game;			/*!< The Game or inherited game which is running */
		bool		m_isRunning;	/*!< Flag to control if the game loop is running */
		double		m_frameTime;	/*!< The desired frame rate (used to calculate fixed time step updates) */
	};
}
