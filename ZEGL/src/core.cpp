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

#include "core.h"
#include "game.h"
#include "timing.h"
#include "util.h"
#include "window.h"
#include <stdio.h>

Core::Core(double frameRate, Window* window, Game* game) :
	m_isRunning(false),
	m_frameTime(1.0 / frameRate),
	m_window(window),
	m_game(game)
{
	m_game->Init(*m_window);
}

void Core::Start()
{
	if (m_isRunning)
	{
		return;
	}

	m_isRunning = true;

	double lastTime = Time::GetTime(); //Current time at the start of the last frame
	double frameCounter = 0;           //Total passed time since last frame counter display
	double unprocessedTime = 0;        //Amount of passed time that the engine hasn't accounted for
	int frames = 0;                    //Number of frames rendered since last

	while (m_isRunning)
	{
		bool render = false;						//Whether or not the game needs to be re-rendered.

		double startTime = Time::GetTime();			//Current time at the start of the frame.
		double passedTime = startTime - lastTime;	//Amount of passed time since last frame.
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		//The engine displays profiling statistics after every second because it needs to display them at some point.
		//The choice of once per second is arbitrary, and can be changed as needed.
		if (frameCounter >= 1.0)
		{
			//The frame counter code here is a temporary, basic profiling tool.
			//When proper profiling tools are implemented, this should probably be removed.
			printf("FPS: %i | %f ms\n", frames, 1000.0 / ((double)frames));

			frames = 0;
			frameCounter = 0;
		}

		//The engine works on a fixed update system, where each update is 1/frameRate seconds of time.
		//Because of this, there can be a situation where there is, for instance, a fixed update of 16ms, 
		//but 20ms of actual time has passed. To ensure all time is accounted for, all passed time is
		//stored in unprocessedTime, and then the engine processes as much time as it can. Any
		//unaccounted time can then be processed later, since it will remain stored in unprocessedTime.
		while (unprocessedTime > m_frameTime)
		{
			m_window->Update();

			if (m_window->IsCloseRequested())
			{
				Stop();
			}

			//Input must be processed here because the window may have found new
			//input events from the OS when it updated. Since inputs can trigger
			//new game actions, the game also needs to be updated immediately 
			//afterwards.
			m_game->ProcessInput(m_window->GetInput(), (float)m_frameTime);
			m_game->Update((float)m_frameTime);

			//Since any updates can put on-screen objects in a new place, the flag
			//must be set to re-render the scene.
			render = true;

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			m_game->Render();

			//The newly rendered image will be in the window's back buffer,
			//so the buffers must be swapped to display the new image.
			m_window->SwapBuffers();

			frames++;
		}
		else
		{
			//If no rendering is needed, sleep for some time so the OS
			//can use the processor for other tasks.
			Util::Sleep(1);
		}
	}
}

void Core::Stop()
{
	if (!m_isRunning)
	{
		return;
	}
		
	m_isRunning = false;
}

