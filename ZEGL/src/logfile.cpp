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

#include "logfile.h"
#include "util.h"
#include <fstream>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef WIN32
#include <direct.h>
#include <iostream>
#include <shlobj.h>
#include <stdlib.h>

#pragma comment(lib, "shell32.lib")
#endif

unsigned int    LogFile::s_numWarnings = 0u;
unsigned int    LogFile::s_numErrors = 0u;
std::string		LogFile::s_fileName = "";
bool			LogFile::s_isActive = false;

#ifndef NDEBUG
int             LogFile::s_logLevel = LOG_INFO;
#else
int             LogFile::s_logLevel = LOG_ERROR;
#endif

char LogFile::s_errorMsg[LogFile::MAX_ERROR_LEN];

bool LogFile::Init(const std::string& fname) 
{
	bool success = true;
    char path[MAX_ERROR_LEN];
    char filepath[MAX_ERROR_LEN];
    
#ifdef WIN32
	TCHAR myDocuments[MAX_ERROR_LEN];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocuments);

	if (result == S_OK)
	{
		size_t i;
		wcstombs_s(&i, path, (size_t)MAX_ERROR_LEN,	myDocuments, (size_t)MAX_ERROR_LEN);
		snprintf(path, sizeof(path), "%s/%s", path, "ZEGL");
		snprintf(filepath, sizeof(filepath), "%s/%s", path, (fname + ".html").c_str());

		_mkdir(path);
	}
#else
    char* home = getenv("HOME");
	if (home != nullptr)
	{
		snprintf(path, sizeof(path), "%s/Library/Application Support/ZEGL", home);
		snprintf(filepath, sizeof(filepath), "%s/Library/Application Support/ZEGL/%s", home, (fname + ".html").c_str());

        mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }
#endif

	s_fileName = filepath;

	std::ofstream logfile(s_fileName, std::ios_base::out);

	if (logfile.is_open())
	{
		s_isActive = true;

		logfile << fname << std::endl;
		logfile << "<br /><br />" << std::endl;
		logfile << "<span style=""font-family: ""verdana""; color: #000000; "">" << std::endl;
		logfile << "Log started at " + CurrentDateTime() + "</span><hr / >" << std::endl;
		logfile << "<br />" << std::endl;

		logfile.close();
	}
	else
	{
		s_fileName = "";
		s_isActive = false;
		success = false;
	}

	return success;
}

void LogFile::Cleanup() 
{
	std::ofstream logfile(s_fileName, std::ios_base::out | std::ios_base::app);

	if (logfile.is_open()) 
	{
		logfile << "<br />" << std::endl;
		logfile << "<hr / >" << std::endl;
		logfile << "Log stopped at " + CurrentDateTime() << std::endl;
		logfile << "<br /><br />" << std::endl;
		logfile << "Number of warnings: " << s_numWarnings << std::endl;
		logfile << "<br />" << std::endl;
		logfile << "Number of errors: " << s_numErrors << std::endl << std::endl;	

		logfile.close();
	}
}

void LogFile::AddEntry(const std::string& text, int logType) 
{
	if (!s_isActive || s_fileName.empty() || logType > s_logLevel) 
	{
		return;
	}

	std::ofstream logfile(s_fileName, std::ios_base::out | std::ios_base::app);

	if (logfile.is_open())
	{
		switch (logType) 
		{
			case LOG_ERROR:
				logfile << "<span style=""color:#AA0000;"">" << CurrentDateTime() << " : [ERROR]: ";
				s_numErrors++;
				break;

			case LOG_WARNING:
				logfile << "<span style=""color:#EAC117;"">" << CurrentDateTime() << " : [WARNING]: ";
				s_numWarnings++;
				break;

			default: //[LOG_INFO]
				logfile << "<span style=""color:#008080;"">" << CurrentDateTime() << " : [INFO]: ";
				break;
		}

		logfile << text << "</span><br />" << std::endl;

		logfile.close();
	}
}

void LogFile::SetLogLevel(int level) 
{
	if (level >= LOG_ERROR && level <= LOG_INFO) 
	{
		s_logLevel = level;
	}
}

const std::string LogFile::CurrentDateTime() 
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
    
#ifdef WIN32
	localtime_s(&tstruct, &now);
#else
    tstruct = *localtime(&now);
#endif
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}
