#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <iostream>
#include <cstdarg>
#include <string>

#define LOGGER Logger::getLogger()

/* Singleton Logger Class */
class Logger
{
public:

	/* Set Log file path*/
	void setFilePath(const std::string& filePath);

	/* File Log function */
	void LogToFile(const std::string& sMessage);
	
	/* Close LogFile */
	void closeLogFile();

	/* Console Log function */
	void Log(const std::string& sMessage);
	
	/* Operator overloading*/
	Logger& operator<<(const std::string& sMessage);
	
	/* Singleton get method */
	static Logger& getLogger() {
		static Logger logger;
		return logger;
	}

private:

	/* Private constructor */
	Logger() = default;

	/* Private destructor */
	~Logger() = default;

	/* Delete copy constructor */
	Logger(const Logger&) = delete;

	/* Delete assignment operator */
	Logger& operator=(const Logger&) = delete;

	/* Log file stream object */
	static std::ofstream m_Logfile;
};
#endif