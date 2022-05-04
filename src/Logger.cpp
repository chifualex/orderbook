#include "Logger.h"

std::ofstream Logger::m_Logfile;

void Logger::setFilePath(const std::string& filePath)
{
	/* Open file for logging */
	m_Logfile.open(filePath.c_str(), std::ios::out | std::ios::app);
}

void Logger::LogToFile(const std::string& sMessage)
{
	/* Log to file */
	m_Logfile << sMessage << "\n";
}

void Logger::closeLogFile()
{
	m_Logfile.close();
}

void Logger::Log(const std::string& sMessage)
{
	/* Log on console */
	std::cout << sMessage << "\n";
}

Logger& Logger::operator<<(const std::string& sMessage)
{
	/* Log on console */
	std::cout << sMessage << "\n";
	return *this;
}