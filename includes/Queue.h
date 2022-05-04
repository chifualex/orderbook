#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <queue>
#include <condition_variable>
#include <mutex>

template<typename T>
class Queue
{
public:

	/* constructor of Queue with unfixed member size */
	Queue()
	{

	}

	/* destructor of Queue */
	~Queue()
	{

	}

	/* Queue push function */
	void push(const T &object)
	{
		/* Acquire lock on the queue*/
		std::lock_guard<std::mutex> lock(m_mutex);

		/* Add the data to the queue */
		m_queue.push(object);

		/* Notify others that data is ready */
		m_condition_variable.notify_one();
	}

	/* Get data from the queue. Wait for data if not available */
	T pop()
	{
		/* Acquire lock on the queue */
		std::unique_lock<std::mutex> lock(m_mutex);
		
		/* When there is no data, wait till someone fills it.
		** Lock is automatically released in the wait and obtained
		* again after the wai*/
		m_condition_variable.wait(lock, [this]() // Lambda funct
		{
			return !m_queue.empty();
		});

		/* Retrieve the data from the queue */
		T result = m_queue.front();
		m_queue.pop();

		return result;
	}// Lock is automatically released here

	/* Queue clear function */
	void clear()
	{
		while (!m_queue.empty())
		{
			m_queue.pop();
		}
	}

	int size()
	{
		return (int)m_queue.size();
	}

private:
	std::mutex m_mutex;
	std::condition_variable m_condition_variable;
	std::queue<T> m_queue;
};

#endif
