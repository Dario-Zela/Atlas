#include "pch.h"
#include "ThreadPool.h"

namespace Atlas
{
	void ThreadPool::CreatePool(int numberOfThreads)
	{
		m_Alive = true;
		m_Mutex.SetMax(numberOfThreads);

		m_Threads.reserve(numberOfThreads);
		for (int i = 0; i < numberOfThreads; i++)
			m_Threads.emplace_back(std::bind(&ThreadPool::Thread, this));
	}

	ThreadPool::~ThreadPool()
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Alive = false;
			m_Condition.notify_all();
		}

		for (auto& thread : m_Threads)
			thread.join();
	}

	void ThreadPool::AddWork(std::function<void(void)> work)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);

		while (m_Mutex.IsFullyUsed());

		m_Jobs.emplace(std::move(work));
		m_Condition.notify_one();
		m_Mutex.InUse();
	}

	void ThreadPool::Sync()
	{
		while (!m_Mutex.IsSynced());
	}

	void ThreadPool::Thread()
	{
		std::function<void(void)> job;

		while (true)
		{
			{
				std::unique_lock<std::mutex> lock(m_Mutex);

				m_Mutex.Waiting();
				m_Condition.wait(lock);

				if (!m_Alive)
					return;

				job = std::move(m_Jobs.front());
				m_Jobs.pop();
			}
			
			job();
		}
	}
}