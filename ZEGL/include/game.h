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

#include "input.h"
#include <glm/glm.hpp>
#include <vector>

struct FONScontext;

namespace ZEGL
{
	class Camera;
	class Light;
	class Window;

	/**
	* Base game class
	*
	* This class is enough to run on its own and could be modified in to a full game.
	* However, the intended use is that it is not used directly and is instead inherited 
	* from to create a new class for each project. The game class would be where all
	* game-play related systems/objects reside e.g levels, fonts, entities. Then each can
	* be updated and rendered in the respective Update() and Render() funtions of the inherited
	* game class.
	*/
	class Game
	{
	public:
		Game();
		virtual ~Game();

		/**
		* All resource loading, game related initialization and OpenGL rendering
		* configuration is performed here.
		*
		* \param[in] window Pointer to an already created Window
		* \see [Window]
		*/
		void Init(Window* window);

		/**
		* Update all game objects using delta time step. Any SDL_Events are polled
		* for and handled here
		*
		* \param[in] delta Delta frame time in milliseconds
		*/
		void Update(float delta);

		/**
		* Render all objects and swap the buffers to display to the screen.
		*/
		void Render();

		/**
		* Update the char array m_fps to include the passed int value.
		*
		* \param[in] frames Current frame rate
		*/
		void SetFPSDisplay(int frames);

		/**
		* Get the currently active light.
		*
		* \return Pointer to the currently active light (can be nullptr)
		*
		* \see [Light]
		*/
		inline const Light*	GetActiveLight() const { return m_activeLight; }

		/**
		* Get the game Camera.
		*
		* \return The Camera being used by the game for display calculations
		*
		* \see [Camera]
		*/
		inline const Camera& GetCamera() const { return *m_camera; }

		/**
		* Get the ambient color for the Game environment.
		*
		* \return A glm::vec4 using r,g,b as color and a as intensity
		*/
		inline const glm::vec4&	GetAmbientColor() const { return m_ambientColor; }
		/**
		* Get the ambient intensity for the Game environment.
		*
		* \return Just the alpha component of m_ambientColor
		*/
		inline float GetAmbientIntensity() const { return m_ambientColor.a; }

		/**
		* Get the game Window.
		*
		* \return Pointer to the Window being used by the Game for rendering
		*
		* \see [Window]
		*/
		inline const Window* GetWindow() { return m_window; }

	protected:
		Camera*				m_camera;		
		Window*				m_window;		
		Input				m_input;		

		glm::vec4			m_ambientColor;	

		Light*				m_activeLight;	
		std::vector<Light*>	m_lights;		

		FONScontext*		m_fontContext;	

		char				m_fps[16];		

	private:
		Game(Game const&) = delete;
		Game& operator=(Game const&) = delete;
	};
}