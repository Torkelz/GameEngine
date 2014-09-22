#pragma once

#include <atomic>

class SpinLock
{
	std::atomic_flag flag;
public:
	void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire))
			;
	}
	bool try_lock()
	{
		return !flag.test_and_set(std::memory_order_acquire);
	}
	void unlock()
	{
		flag.clear(std::memory_order_release);
	}
	SpinLock()
	{
		flag.clear();
	}
};

