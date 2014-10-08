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

#ifndef LOGFILE_H
#define LOGFILE_H

#define LOG_INIT(fname)             LogFile::Init(fname)
#define LOG_CLEANUP()               LogFile::Cleanup()
#define LOG_ENTRY(text, logType)    LogFile::AddEntry(text, logType)
#define LOG_SET_ACTIVE(active)		LogFile::SetLogActive(active)
#ifndef NDEBUG
#define LOT_SET_LEVEL(lvl)          LogFile::SetLogLevel(lvl)
#else
#define LOT_SET_LEVEL(lvl)
#endif

#include <string>
	
class LogFile
{
public:
	enum 
	{
		LOG_ERROR,
		LOG_WARNING,
		LOG_INFO
	};

	static const int MAX_ERROR_LEN = 256;

	static bool Init(const std::string& fname = "ZEGL");
	static void Cleanup();

	static void AddEntry(const std::string& text, int logType);

	static void SetLogLevel(int level);
	static inline void SetLogActive(bool active) { m_isActive = active; }

	static char s_errorMsg[MAX_ERROR_LEN];

protected:
private:
	static const std::string CurrentDateTime();

	static std::string m_fileName;

	static unsigned int m_numWarnings;
	static unsigned int m_numErrors;

	static int m_logLevel;

	static bool m_isActive;
};

#endif
