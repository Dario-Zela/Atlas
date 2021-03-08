#include "pch.h"
#include "ThreadPool.h"

namespace Atlas
{
	void ThreadPool::CreatePool(uint numberOfThreads)
	{
		//Make sure the threadpool has not been already created
		AT_CORE_ASSERT(!m_Created, "You cannot recrate a thread pool");

		//Update it's status
		m_Created = true;

		//Set the maximum number of the mutex
		m_Mutex.SetMax(numberOfThreads);

		//Add all of the threads and assign them the task method as a job
		for (int i = 0; i < numberOfThreads; i++)
			m_Threads.emplace_back(std::bind(&ThreadPool::Thread, this, i));

		//Make enough contexts for each thread
		m_Contexts = new DeferredRenderContext[numberOfThreads]();
	}

	ThreadPool::~ThreadPool()
	{
		{
			//Make all threads quit
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Created = false;
			m_Condition.notify_all();
		}

		//Wait for all of them to join
		for (auto& thread : m_Threads)
			thread.join();

		//Make sure there are no outstanding exeptions
		HandleExeptions();

		//Delete the contexts
		delete[] m_Contexts;
	}

	void ThreadPool::AddWork(std::function<void(Pass*, wrl::ComPtr<ID3D11DeviceContext>, int)> work, Pass* executable, int index)
	{
		//Wait until there is a thread that can be used
		while (m_Mutex.IsFullyUsed());
		
		//Make sure there is no exeption
		HandleExeptions();

		//Add the task to the queue
		m_Jobs.emplace(work, executable, index);

		//There is an error taht work gets added as empty
		//Check if that has happened
		if (!std::get<0>(m_Jobs.front()))
		{
			//If it has, pop the job and readd it
			m_Jobs.pop();
			m_Jobs.emplace(work, executable, index);
		}

		//Notify one of the tasks to start working and 
		//Make one of the task in waiting
		m_Condition.notify_one();
		m_Mutex.InUse();
	}

	void ThreadPool::Sync()
	{

		//Wait while there are jobs, checking for exeptions
		while (!m_Jobs.empty()) HandleExeptions();

		//Wait until all task are done, checking for exeptions
		while (!m_Mutex.IsSynced()) HandleExeptions();

		//Check for exeptions one last time
		HandleExeptions();

	}

	void ThreadPool::Thread(int threadNum)
	{
		//Define variables to be used later
		std::function<void(Pass*, wrl::ComPtr<ID3D11DeviceContext>, int)> job;
		Pass* executable = nullptr;
		int index = 0;

		//Until returned from inside
		while (true)
		{
			{
				//Wait for the thread to become available
				std::unique_lock<std::mutex> lock(m_Mutex);
				
				m_Mutex.Waiting();		//Signifying it is available
				
				m_Condition.wait(lock);

				//If the threadpool is not alive anymore
				if (!m_Created)
					//End the execution
					return;

				//Else, add a job
				auto tuple = m_Jobs.front();
				job = std::move(std::get<0>(tuple));
				executable = std::get<1>(tuple);
				index = std::get<2>(tuple);
				m_Jobs.pop();
			}

			//And execute it
			try 
			{
				job(executable, m_Contexts[threadNum].GetContext(), index);
			}
			//If there are any exeption, add them to the list
			catch (std::exception& e)
			{
				m_Exceptions.push_back(std::exception(( "The pass " + executable->GetName() + " gave the exeption: " + e.what()).c_str()));
			}
		}
	}

	void ThreadPool::HandleExeptions()
	{
		//If there are any exeptions, return
		if (m_Exceptions.empty()) return;

		//Else, throw the first exeption
		throw m_Exceptions[0];
	}
}