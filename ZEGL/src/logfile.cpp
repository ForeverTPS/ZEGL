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
#include <fstream>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

unsigned int    LogFile::m_numWarnings = 0u;
unsigned int    LogFile::m_numErrors = 0u;
std::string		LogFile::m_fileName = "";
bool			LogFile::m_isActive = false;

#ifndef NDEBUG
int             LogFile::m_logLevel = LOG_INFO;
#else
int             LogFile::m_logLevel = LOG_ERROR;
#endif

char LogFile::s_errorMsg[LogFile::MAX_ERROR_LEN];

bool LogFile::Init(const std::string& fname) 
{
	bool success = true;
    char path[MAX_ERROR_LEN];
    char filepath[MAX_ERROR_LEN];
    
#ifdef WIN32
    char* home = getenv("HOMEPATH");
#else
    char* home = getenv("HOME");
#endif
    
    if (home != nullptr)
    {
        snprintf(path, sizeof(path), "%s/Library/Application Support/ZEGL", home);
        snprintf(filepath, sizeof(filepath), "%s/Library/Application Support/ZEGL/%s", home, (fname + ".html").c_str());
        
        mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

	m_fileName = filepath;

	std::ofstream logfile(m_fileName, std::ios_base::out);

	if (logfile.is_open())
	{
		m_isActive = true;

		logfile << fname << std::endl;
		logfile << "<br /><br />" << std::endl;
		logfile << "<span style=""font-family: ""verdana""; color: #000000; "">" << std::endl;
		logfile << "Log started at " + CurrentDateTime() + "</span><hr / >" << std::endl;
		logfile << "<br />" << std::endl;

		logfile.close();
	}
	else
	{
		m_fileName = "";
		m_isActive = false;
		success = false;
	}

	return success;
}

void LogFile::Cleanup() 
{
	std::ofstream logfile(m_fileName, std::ios_base::out | std::ios_base::app);

	if (logfile.is_open()) 
	{
		logfile << "<br />" << std::endl;
		logfile << "<hr / >" << std::endl;
		logfile << "Log stopped at " + CurrentDateTime() << std::endl;
		logfile << "<br /><br />" << std::endl;
		logfile << "Number of warnings: " << m_numWarnings << std::endl;
		logfile << "<br />" << std::endl;
		logfile << "Number of errors: " << m_numErrors << std::endl << std::endl;	

		logfile.close();
	}
}

void LogFile::AddEntry(const std::string& text, int logType) 
{
	if (!m_isActive || m_fileName.empty() || logType > m_logLevel) 
	{
		return;
	}

	std::ofstream logfile(m_fileName, std::ios_base::out | std::ios_base::app);

	if (logfile.is_open())
	{
		switch (logType) 
		{
			case LOG_ERROR:
				logfile << "<span style=""color:#AA0000;"">" << CurrentDateTime() << " : [ERROR]: ";
				m_numErrors++;
				break;

			case LOG_WARNING:
				logfile << "<span style=""color:#EAC117;"">" << CurrentDateTime() << " : [WARNING]: ";
				m_numWarnings++;
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
		m_logLevel = level;
	}
}

const std::string LogFile::CurrentDateTime() 
{
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
    
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}
