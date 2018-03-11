#include "ThreadPool.h"
#include <ostream>
#include <iostream>

ThreadPool::ThreadPool(const int numThreads)
{
	running = true;
	InitialiseWorkers(numThreads);
}

ThreadPool::ThreadPool()
{
	running = true;

	//Create as many threads as the hardware allows but keep one for the main thread.
	const int numThreads = (std::max)(std::thread::hardware_concurrency(), unsigned(2)) - 1;

	InitialiseWorkers(numThreads);
}

void ThreadPool::InitialiseWorkers(int numWorkers)
{
	for (int i = 0; i < numWorkers; ++i)
	{
		threads.emplace_back(&ThreadPool::FindNewTask, this);
	}
}

void ThreadPool::FindNewTask()
{
	//Continiously poll the queue until all work is done
	while (running) 
	{
		std::unique_ptr<Task> newTask = nullptr;

		if (taskQueue.WaitPop(newTask)) 
		{
			//A new job has been found. Put it on a thread and do it...
			newTask->execute();
		}
	}
}

void ThreadPool::JoinAll()
{
	for (auto& thread : threads) 
	{
		if (thread.joinable()) 
		{
			thread.join(); //Stop all threads.
		}
	}
}