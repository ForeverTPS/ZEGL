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


#include "example.h"

#if !defined(NDEBUG) && defined(_MSC_FULL_VER)
	#include "vld.h"
#endif

using namespace ZEGL;

int main(int argc, char *argv[])
{
	LOG_INIT("ZEGL");

	if (System::Init())
	{
		Window window(800, 600, "ZEGL");
		MyGame game;

		Core engine(60, &window, &game);
		engine.Start();

		System::Quit();
	}

	LOG_CLOSE();

	return 0;
}

