
#include "FileStreamerImpl.h"

FileStreamerImpl::FileStreamerImpl(std::string const& filePath, Queue<std::string>& processingQueue) : m_processingQueue(processingQueue)
{
	m_isRunning = true;
	m_filePath = filePath;
}

void FileStreamerImpl::read()
{
	/* Read function - Read from beginning of the file and send line by line to queue*/
	std::string line = "";
	m_file.seekg(0, std::ios::beg);
	while (!m_file.eof())
	{
		std::getline(m_file, line);
		m_processingQueue.push(line);
	}

	/* Added for testing */
	m_processingQueue.push("Exit");
}

void FileStreamerImpl::open()
{
	/* File open */
	m_file.open(m_filePath, std::ios::in);
	if (!m_file)
	{
		std::cout << "File does not exists in the specified path" << std::endl;
	}
}

void FileStreamerImpl::setThreadStatus(bool isRunning)
{
	m_isRunning = isRunning;
}

FileStreamerImpl::~FileStreamerImpl()
{
	close();
}

void FileStreamerImpl::close()
{
	m_file.close();
}
