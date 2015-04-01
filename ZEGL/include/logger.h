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

#include <string>
#include <sstream>

namespace Logger
{
	void Init(const std::string& fname = "ZEGL");
	void Close();

	void NewLine();

	void LogInfo(const std::string& msg);
	void LogWarning(const std::string& msg);
	void LogError(const std::string& msg);
}

#define LOG_INIT(fname)     Logger::Init(fname);
#define LOG_CLOSE()         Logger::Close();
#define LOG_NEWLINE()		Logger::NewLine();
#define LOG_INFO(msg)		Logger::LogInfo(static_cast<std::ostringstream&>(std::ostringstream().flush() << msg).str());
#define LOG_WARNING(msg)	Logger::LogWarning(static_cast<std::ostringstream&>(std::ostringstream().flush() << msg).str());
#define LOG_ERROR(msg)		Logger::LogError(static_cast<std::ostringstream&>(std::ostringstream().flush() << msg << " " << __FUNCTION__ << "() " << __FILE__ << " line " << __LINE__).str());
