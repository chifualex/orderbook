#ifndef THREADMANAGER_HPP
#define THREADMANAGER_HPP

#include <vector>
#include <thread>
#include <functional>
#include <iostream>
#include <memory>

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	/* Join all threads */
	void joinAll();

	/* Create thread template function */
	template <typename T, typename ...Args>
	std::thread* createThread(std::shared_ptr<T> obj, void(T::*mf)(Args...), Args ... args)
	{
		m_threads.push_back(std::thread(std::bind(mf, obj, args...)));
		return &m_threads.at(m_threads.size() - size_t(1));
	}

private:
	std::vector<std::thread> m_threads;
};
#endif
