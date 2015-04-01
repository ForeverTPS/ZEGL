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
#include "random.h"
#include "timing.h"
#include "util.h"
#include "window.h"

using namespace ZEGL;

Core::Core(double frameRate, Window* window, Game* game) :
	m_isRunning(false),
	m_frameTime(1.0 / frameRate),
	m_window(window),
	m_game(game)
{
	Random::Init();
	m_game->Init(m_window);
}

void Core::Start()
{
	if (m_isRunning)
	{
		return;
	}

	m_isRunning = true;

	double lastTime = Time::GetTime(); // Current time at the start of the last frame
	double frameCounter = 0;           // Total passed time since last frame counter display
	double unprocessedTime = 0;        // Amount of passed time that the engine hasn't accounted for
	int frames = 0;                    // Number of frames rendered since last

	while (m_isRunning)
	{
		bool render = false;						

		double startTime = Time::GetTime();			// Current time at the start of the frame.
		double passedTime = startTime - lastTime;	// Amount of passed time since last frame.
		lastTime = startTime;

		unprocessedTime += passedTime;
		frameCounter += passedTime;

		if (frameCounter >= 1.0)
		{
			// The frame counter code here is a temporary, basic profiling tool.
			printf("FPS: %i | %f ms\n", frames, 1000.0 / ((double)frames));

			frames = 0;
			frameCounter = 0;
		}

		while (unprocessedTime > m_frameTime)
		{
			m_window->Update();

			if (m_window->IsCloseRequested())
			{
				Stop();
			}

			m_game->ProcessInput(m_window->GetInput(), (float)m_frameTime);
			m_game->Update((float)m_frameTime);

			render = true;

			unprocessedTime -= m_frameTime;
		}

		if (render)
		{
			m_game->Render();
			m_window->SwapBuffers();
			frames++;
		}
		else
		{
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

