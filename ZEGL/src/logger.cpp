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

#include "logger.h"
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

namespace Logger
{
	namespace
	{
		const std::string CurrentDateTime()
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

		unsigned int	m_numWarnings	= 0u;
		unsigned int	m_numErrors		= 0u;
		std::ofstream	m_logfile;
	}

	void Init(const std::string& fname)
	{
		bool success = true;
		char path[256];
		char filepath[256];

#ifdef WIN32
		TCHAR myDocuments[256];
		HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, myDocuments);

		if (result == S_OK)
		{
			size_t i;
			wcstombs_s(&i, path, (size_t)256, myDocuments, (size_t)256);
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

		m_logfile.open(filepath, std::ios_base::out);

		if (m_logfile.is_open())
		{
			m_logfile << fname << std::endl;
			m_logfile << "<br /><br />" << std::endl;
			m_logfile << "<span style=""font-family: ""verdana""; color: #000000; "">" << std::endl;
			m_logfile << "Log started at " + CurrentDateTime() + "</span><hr / >" << std::endl;
			m_logfile << "<br />" << std::endl;
		}
	}

	void Logger::Close()
	{
		if (m_logfile.is_open())
		{
			m_logfile << "<br />" << std::endl;
			m_logfile << "<hr / >" << std::endl;
			m_logfile << "Log stopped at " + CurrentDateTime() << std::endl;
			m_logfile << "<br /><br />" << std::endl;
			m_logfile << "Number of warnings: " << m_numWarnings << std::endl;
			m_logfile << "<br />" << std::endl;
			m_logfile << "Number of errors: " << m_numErrors << std::endl << std::endl;

			m_logfile.close();
		}
	}

	void NewLine()
	{
		if (m_logfile.is_open())
		{
			m_logfile << "<br />" << std::endl;
		}
	}

	void LogInfo(const std::string& msg)
	{
		if (m_logfile.is_open())
		{
			m_logfile << "<span style = ""color:#008080; "">" << CurrentDateTime() << " : [INFO] : " << msg << "</span><br />" << std::endl;
		}
	}

	void LogWarning(const std::string& msg)
	{
		if (m_logfile.is_open())
		{
			m_logfile << "<span style = ""color:#EAC117; "">" << CurrentDateTime() << " : [WARN] : " << msg << "</span><br />" << std::endl;
			m_numWarnings++;
		}
	}

	void LogError(const std::string& msg)
	{
		if (m_logfile.is_open())
		{
			m_logfile << "<span style = ""color:#AA0000; "">" << CurrentDateTime() << " : [ERROR] : " << msg << "</span><br />" << std::endl;
			m_numErrors++;
		}
	}
}