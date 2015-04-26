/*
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

#pragma once

#include <string>
#include <sstream>

/**
* Using a namespace rather than a static class or singleton etc to provide
* a robust but lightweight logging system.
*/
namespace Logger
{
	/**
	* Initialize the log.
	*
	* Created the file and logs the start time. The file will be created in the user 
	* home directory under a folder ZEGL with the passed file name and a html extension. 
	*
	* \param[in] fname File name for the log file. Name only, no path or extension.
	*
	* \warning Do not call this function directly, instead use the preprocessor macro
	* LOG_INIT(fname)
	*/
	void Init(const std::string& fname = "ZEGL");

	/**
	* Closes the log file.
	*
	* Logs the end time, number of warning and errors and closes the log file down. 
	*
	* \warning Do not call this function directly, instead use the preprocessor macro
	* LOG_CLOSE()
	*/
	void Close();

	/**
	* Inserts a new line in the log file. 
	*
	* \warning Do not call this function directly, instead use the preprocessor macro
	* LOG_NEWLINE()
	*/
	void NewLine();

	/**
	* Logs an information message to the log file.
	*
	* Logs the passed message string to the log file with a date/time stamp and labels
	* it as [INFO]. 
	*
	* \param[in] msg Message string to log
	*
	* \warning Do not call this function directly, instead use the preprocessor macro
	* LOG_INFO(msg)
	*/
	void LogInfo(const std::string& msg);

	/**
	* Logs a warning message to the log file.
	*
	* Logs the passed message string to the log file with a date/time stamp and labels
	* it as [WARN]. 
	*
	* \param[in] msg Message string to log
	*
	* \warning Do not call this function directly, instead use the preprocessor macro
	* LOG_WARNING(msg)
	*/
	void LogWarning(const std::string& msg);

	/**
	* Logs an error message to the log file.
	*
	* Logs the passed message string to the log file with a date/time stamp and labels
	* it as [ERROR]. 
	*
	* \param[in] msg Message string to log
	*
	* \warning Do not call this function directly, instead use the preprocessor macro
	* LOG_ERROR(msg) which will also auto append the file name and line number where
	* the error occurred
	*/
	void LogError(const std::string& msg);

#define LOG_INIT(fname)     Logger::Init(fname);
#define LOG_CLOSE()         Logger::Close();
#define LOG_NEWLINE()		Logger::NewLine();
#define LOG_INFO(msg)		Logger::LogInfo(static_cast<std::ostringstream&>(std::ostringstream().flush() << msg).str());
#define LOG_WARNING(msg)	Logger::LogWarning(static_cast<std::ostringstream&>(std::ostringstream().flush() << msg).str());
#define LOG_ERROR(msg)		Logger::LogError(static_cast<std::ostringstream&>(std::ostringstream().flush() << msg << " " << __FUNCTION__ << "() " << __FILE__ << " line " << __LINE__).str());
}