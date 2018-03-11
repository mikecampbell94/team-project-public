#include "ThreadPool.h"

ThreadPool::ThreadPool(const int numThreads)
{
	running = true;
	initialiseWorkers(numThreads);
}

ThreadPool::ThreadPool()
{
	running = true;

	//Create as many threads as the hardware allows but keep one for the main thread.
	const int numThreads = (std::max)(std::thread::hardware_concurrency(), unsigned(2)) - 1;

	initialiseWorkers(numThreads);
}

void ThreadPool::initialiseWorkers(int numWorkers)
{
	for (int i = 0; i < numWorkers; ++i)
	{
		threads.emplace_back(&ThreadPool::continiouslyPollForNewTask, this);
	}
}

void ThreadPool::continiouslyPollForNewTask()
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

void ThreadPool::joinAllThreads()
{
	for (auto& thread : threads) 
	{
		if (thread.joinable()) 
		{
			thread.join(); //Stop all threads.
		}
	}
}