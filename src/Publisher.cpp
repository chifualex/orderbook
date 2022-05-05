#include "Publisher.h"

/* Set queue reference to main publishing queue*/
Publisher::Publisher(bool logToFile, bool logEnabled, Queue<std::string>& publishingQueue) : m_publishingQueue(publishingQueue)
{
	m_logToFile = logToFile;
	m_logEnabled = logEnabled;
}

Publisher::Publisher(const Publisher & other) : m_publishingQueue(other.m_publishingQueue)
{
}

Publisher::~Publisher()
{
}

void Publisher::processPublishingMessages()
{
	/* Publishing task method*/
	while (true)
	{
		/* Wait for publishing messages*/
		std::string stream = m_publishingQueue.pop();
		
		if (stream == "Exit")
		{
			break;
		}

		/* Log received messages by logging type */
		if (m_logEnabled)
		{
			if (m_logToFile)
			{
				Logger::getLogger().LogToFile(stream);
			}
			else
			{
				Logger::getLogger().Log(stream);
			}
		}
	}
}
