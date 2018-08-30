#include "ThreadPool.h"

// ctor launch workers
ThreadPool::ThreadPool(size_t threads):stop(false){
	for (size_t i = 0; i < threads; ++i)
		workers.emplace_back(
			[this] {
				for (;;) {
					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(this->queue_mutex);
						this->condition.wait(lock,[this] { return this->stop || !this->tasks.empty(); });
						if (this->stop && this->tasks.empty())
							return;
						task = std::move(this->tasks.front());
						this->tasks.pop();
					}
					task();
				}
			}
		);
}

// dtor join all threads
ThreadPool::~ThreadPool(){
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		stop = true;
	}
	condition.notify_all();
	for (auto& worker : workers)
		worker.join();
}