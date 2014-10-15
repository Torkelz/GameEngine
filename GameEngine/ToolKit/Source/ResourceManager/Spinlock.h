#pragma once
#include <atomic>

class SpinLock
{
private:
	std::atomic_flag flag;

public:
	inline void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire));
	}
	inline bool try_lock()
	{
		return !flag.test_and_set(std::memory_order_acquire);
	}
	inline void unlock()
	{
		flag.clear(std::memory_order_release);
	}

	inline SpinLock(void)
	{
		flag.clear();
	}

};

