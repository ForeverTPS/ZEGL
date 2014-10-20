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
#include "logfile.h"
#include "window.h"

#ifdef __APPLE__
#include "CoreFoundation/CoreFoundation.h"
#endif

#if defined(_DEBUG) && defined(_MSC_FULL_VER)
#include "vld.h"
#endif

int main(int argc, char *argv[])
{
    // ----------------------------------------------------------------------------
    // This makes relative paths work in C++ in Xcode by changing directory to the Resources folder inside the .app bundle
#ifdef __APPLE__
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
    }
    CFRelease(resourcesURL);
    
    chdir(path);
#endif
    // ----------------------------------------------------------------------------
    
	LOG_INIT("ZEGL");

	Window window(800, 600, "ZEGL");
	Game game;

	Core engine(60, &window, &game);
	engine.Start();

	LOG_CLEANUP();

	return 0;
}

