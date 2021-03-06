#include "LinearAllocator.h"
#include "MemoryExceptions.h"

#include <malloc.h>
#include <string>

namespace Allocator
{
	LinearAllocator::LinearAllocator()
		: m_Size(0), m_Original(false), m_Marker(0), m_Buffer(nullptr)
	{ }

	LinearAllocator::LinearAllocator(UINT p_Size)
		: m_Size(p_Size), m_Original(true), m_Marker(0)
	{
		if (m_Size == 0)
			throw MemoryException("LinearAllocator, size was 0", __LINE__, __FILE__);

		m_Buffer = (char*)malloc(m_Size + sizeof(void*));
		if (m_Buffer == nullptr)
			throw MemoryException("LinearAllocator, failed to allocate memory", __LINE__, __FILE__);
	}

	LinearAllocator::LinearAllocator(char *p_Buffer, UINT p_Size)
		: m_Buffer(p_Buffer), m_Size(p_Size), m_Original(false), m_Marker(0)
	{
		if (m_Size == 0)
			throw MemoryException("LinearAllocator, size was 0", __LINE__, __FILE__);
		if (m_Buffer == nullptr)
			throw MemoryException("LinearAllocator, buffer parameter was nullptr", __LINE__, __FILE__);
	}

	LinearAllocator::~LinearAllocator(void)
	{
		/**
		 * Memory should be returned to the OS if this is where the memory was originally allocated.
		 * Else set to nullptr not to mess up for others.
		 */
		if (m_Original)
			free(m_Buffer);

		m_Buffer = nullptr;
	}

	void *LinearAllocator::allocate(UINT p_Size)
	{
		UINT cSize = m_Marker.fetch_add(p_Size, std::memory_order_acquire);

		if (cSize + p_Size  >= m_Size)
		{
			m_Marker.fetch_sub(p_Size, std::memory_order_release);
			return nullptr;
		}

		void *currentAddress = m_Buffer + cSize +  p_Size;
		return currentAddress;
	}

	void LinearAllocator::freeMarkerTo(UINT p_Marker)
	{
		//Should the program or programmer be smart if the p_Marker is larger than either m_Marker or m_Size?
		m_Marker = p_Marker;
	}

	void LinearAllocator::clear(void)
	{
		m_Marker = 0;
	}

	LinearAllocator::UINT LinearAllocator::getMarker(void) const
	{
		return m_Marker.load();
	}
	LinearAllocator::UINT LinearAllocator::alignForwardAdjustment(const void *p_Address, UINT p_Alignment)
	{
		UINT adjustment = p_Alignment - (reinterpret_cast<uintptr_t>(p_Address)& static_cast<uintptr_t>(p_Alignment - 1));

		if (adjustment == p_Alignment)
			return 0; // already aligned

		return adjustment;
	}
}