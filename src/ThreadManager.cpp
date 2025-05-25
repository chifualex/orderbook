#include "ThreadManager.h"


/* Thread constructor */
ThreadManager::ThreadManager()
{
	// Get hardware concurrency capability
	unsigned int threadNr = std::thread::hardware_concurrency() - 1;

	if (threadNr < 2)
	{
		m_threads.reserve(2);
	}
	else
	{
		m_threads.reserve(threadNr);
	}
}

/* Thread destructor */
ThreadManager::~ThreadManager()
{
	m_threads.clear();
}

/* Join all threads */
void ThreadManager::joinAll()
{
	for (size_t i = 0; i < m_threads.size(); i++)
	{
		m_threads.at(i).join();
	}
}
