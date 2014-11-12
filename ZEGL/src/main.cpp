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

#include "stdafx.h"

#include "core.h"
#include "game.h"
#include "logfile.h"
#include "util.h"
#include "window.h"

#ifdef _DEBUG
#include "vld.h"
#endif

int main(int argc, char *argv[])
{   
	LOG_INIT("ZEGL");

	Window window(800, 600, "ZEGL");
	Game game;

	Core engine(60, &window, &game);
	engine.Start();

	LOG_CLEANUP();

	return 0;
}

