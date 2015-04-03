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

#include "time.h"

namespace ZEGL
{
	class Timer
	{
	public:
		Timer() : m_started(false), m_startTime(0) {}

		void Start()
		{
			if (!m_started)
			{
				m_started = true;
				m_startTime = Time::GetTime();
			}
		}

		double Stop()
		{
			double lastTime = 0;
			if (m_started)
			{
				lastTime = GetTicks();
				m_started = false;
			}

			return lastTime;
		}

		void Restart()
		{
			Stop();
			Start();
		}

		double GetTicks()
		{
			return m_started ? Time::GetTime() - m_startTime : 0;
		}

	protected:
	private:
		bool	m_started;
		double	m_startTime;
	};
}