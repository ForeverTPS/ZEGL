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

#pragma once

#if defined(_WIN64)
	#define OS_WINDOWS
	#define OS_WINDOWS_64
	#define WIN32_LEAN_AND_MEAN
#elif defined(_WIN32)
	#define OS_WINDOWS
	#define OS_WINDOWS_32
	#define WIN32_LEAN_AND_MEAN
#elif defined(__linux__) || defined(__linux)
	#define OS_LINUX
#elif defined(__unix__) || defined(__unix)
	#define OS_UNIX
#elif defined(__APPLE__) || defined(__MACH__)
	#define OS_APPLE
	
	#include "CoreFoundation/CoreFoundation.h"

	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	char path[PATH_MAX];
	if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
	{
		// error!
	}
	CFRelease(resourcesURL);

	chdir(path);

#else
	#define OS_OTHER
#endif