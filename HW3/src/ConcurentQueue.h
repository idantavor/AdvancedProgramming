#pragma once
#include <queue>
#include <mutex>
using namespace std;



template<class T>
class ConcurentQueue {
private:
	std::queue<T> queue;
	mutex queue_lock;

public:
	void push(const T& data)
	{
		std::lock_guard<std::mutex> lock(queue_lock);
		queue.push(data);
	}

	bool empty()
	{	
		std::lock_guard<std::mutex> lock(queue_lock);
		bool isEmpty = queue.empty();
		return isEmpty;
	}

	T& front()
	{	
		std::lock_guard<std::mutex> lock(queue_lock);
		T& front=queue.front();
		return front;
	}


	void pop()
	{
		std::lock_guard<std::mutex> lock(queue_lock);
		queue.pop();
	}

	T get() {
		std::lock_guard<std::mutex> lock(queue_lock);
		if (!queue.empty()) {
			T data = queue.front();
			queue.pop();
			return data;
		}
		else {
			return NULL;
		}
		
		
	}
};
