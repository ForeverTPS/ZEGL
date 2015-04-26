/**
* @section LICENSE
*
* Copyright(c) 2014, Phil Sampson

* http://www.zamma.co.uk
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
*
* \mainpage ZEGL Index Page
*
* \section intro_sec Introduction
*
* This game engine has been developed in C++ using the following libraries / extensions
*
* - [SDL2](https://www.libsdl.org/index.php)
* - [OpenGL 3.x](https://www.opengl.org/)
* - [GLM](http://glm.g-truc.net/0.9.6/index.html)
* - [GLEW](http://glew.sourceforge.net/)
* - [stb_image - From the stb project](https://github.com/nothings/stb)
* - [tinyxml2](https://github.com/leethomason/tinyxml2)
* - [FontStash - Forked and modified for my use](https://github.com/Zammalad/fontstash)
*
* All library and dll files are packaged with the source so there is no need to download them. 
* This helps ensure that the compatibility is maintained if new versions are released.
*
* Github Repo - [https://github.com/Zammalad/ZEGL](https://github.com/Zammalad/ZEGL)
*
* The `Game` class is designed to be used as a base class and inherited from to create
* each new game. Once implemented, your main.cpp file should look like this (where `MyGame`
* is your inherited game class)
*
* An example is included with the source.
*
* \code{.cpp}
* #include "platform.h"
* #include "core.h"
* #include "mygame.h"
* #include "logger.h"
* #include "system.h"
* #include "util.h"
* #include "window.h"
* 
* using namespace ZEGL;
* 
* int main(int argc, char *argv[])
* {
*	// Initialize the logger class and pass a filename
*	LOG_INIT("ZEGL");
* 
*	// Perform system initialization
*	if (System::Init())
*	{
*		// Create a window with width 800, height 600 and a title
*		Window window(800, 600, "ZEGL");
*		MyGame game;
* 
*		// Create the core system passing in the desired target fps
*		/ and pointers to the window and game just created
*		Core engine(60, &window, &game);
*		// Enter the game loop
*		engine.Start();
* 
*		// System cleanup
*		System::Quit();
*	}
* 
*	// Finish logging
*	LOG_CLOSE();
* 
*	return 0;
* }
* \endcode
*/