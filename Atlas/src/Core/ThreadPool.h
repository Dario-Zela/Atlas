#pragma once
#include <condition_variable>
#include <thread>
#include <mutex>
#include <functional>
#include <queue>
#include <atomic>
#include "Graphics/RenderGraphAPI/Pass.h"
#include "Graphics/D3DWrappers/DeferredRenderContext.h"

namespace Atlas 
{
	//A custum mutex that has a counter added to it
	class CountedMutex : public std::mutex
	{
	public:
		//Makes a mutex with a maximum number of threads 
		//of 0 and a count of available threads of 0
		CountedMutex()
			: std::mutex(), m_Max(0), m_Count(0) { }

		//Returns if the available threads is the same as the max
		bool IsSynced() { return m_Max == m_Count; }
		//Returns if the available threads is 0
		bool IsFullyUsed() { return m_Count == 0; }

		//Sets the maximum number of threads
		void SetMax(uint numberOfThreads) { m_Max = numberOfThreads; }

		//States that a thread is in use
		void InUse() { m_Count--; }
		//States that a threads is available to use
		void Waiting() { m_Count++; }
	private:
		//Max number of threads
		uint m_Max;
		//An atomic value indicating the available threads
		std::atomic<uint> m_Count;
	};

	//A pull of threads that execute tasks
	class ThreadPool
	{
	public:
		//Cretes the threadpool
		ThreadPool() : m_Created(false) {}
		//Clears the memory of the threadpool
		~ThreadPool();

		//This sets up the mutex and the pool of threads
		void CreatePool(uint numberOfThreads);

		//Adds a job to the job queue and gives it to a thread
		void AddWork(std::function<void(Pass*, wrl::ComPtr<ID3D11DeviceContext>, int)> work, Pass* executable, int threadNum);
		//Waits for all the threads to finish working
		void Sync();
	private:
		//The task each thread will compleate
		void Thread(int index);
		//This will handle any exeptions that will be
		//Thrown by the threads
		void HandleExeptions();

		//The mutex with thread management capability
		CountedMutex m_Mutex;
		//The list of threads
		std::vector<std::thread> m_Threads;
		//The list of exeptions generated by the separate threads
		std::vector<std::exception> m_Exceptions;
		//The queue of jobs
		std::queue<std::tuple<std::function<void(Pass*, wrl::ComPtr<ID3D11DeviceContext>, int)>, Pass*, int>> m_Jobs;
		//A condition variable
		std::condition_variable m_Condition;
		//The deferred contexts that will be used by the threads
		DeferredRenderContext* m_Contexts;
		
		//Has the threadpool already been created
		bool m_Created;
	};
}