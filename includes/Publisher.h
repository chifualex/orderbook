#ifndef PUBLISHER_H
#define PUBLISHER_H

#include "Queue.h"
#include "Logger.h"

class Publisher
{
public:

	/* Constructor */
	Publisher(bool logToFile, bool logEnabled, Queue<std::string>& publishingQueue);

	/* Copy constructor */
	Publisher(const Publisher& other);

	/* Destructor */
	~Publisher();

	/* Method used to publish (log) messages */
	void processPublishingMessages();

private:
	Queue<std::string>& m_publishingQueue;
	bool m_logToFile;
	bool m_logEnabled;
};
#endif