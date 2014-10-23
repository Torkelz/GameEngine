#pragma once
#include <atomic>

namespace Allocator
{
	class LinearAllocator
	{
	public:
		typedef unsigned int UINT;

	private:
		/**
		 * Marks where next allocation begins.
		 */
		std::atomic<UINT> m_Marker;

		/**
		 * Describes the size of m_Buffer.
		 */
		UINT m_Size;

		/**
		 * Memory.
		 */
		char *m_Buffer;

		/**
		 * Describes if memory should be returned to OS or not when deallocated.
		 * true, return to OS. false, nullptr
		 */
		bool m_Original;
	public:
		/**
		 * Default constructor. Don't use it.
		 */
		LinearAllocator(void);

		/**
		 * @param p_Size The amount of memory to be allocated in bytes.
		 * Throws MemoryException if it fails to allocate memory or if p_Size is zero.
		 */
		LinearAllocator(UINT p_Size);

		/**
		 * @param p_Buffer An already allocated memory chunk.
		 * @param p_Size The amount of memory p_Buffer allocates.
		 * Throws MemoryException if p_Buffer is nullptr or if p_Size is zero.
		 */
		LinearAllocator(char *p_Buffer, UINT p_Size);

		/**
		 * Default destructor.
		 */
		~LinearAllocator(void);

		/**
		 * Allocates a new memory chunk.
		 *
		 * @param p_Size The amount of memory to be allocated in bytes.
		 * @return If able to allocate memory it returns a pointer to the allocated chunk.
		 * @return If failed to allocate memory it returns nullptr.
		 */
		void *allocate(UINT p_Size);

		template<typename T>
		T *allocate(UINT p_Alignment = 0)
		{
			UINT tempMarker = m_Marker.load();

			void* address = m_Buffer + tempMarker;

			UINT adjustment = alignForwardAdjustment(address, p_Alignment);

			if (tempMarker + sizeof(T) + adjustment >= m_Size)
			{
				return nullptr;
			}

			void *alignedAdress = m_Buffer + tempMarker + adjustment;

			m_Marker = tempMarker + adjustment + sizeof(T);
			return (T*)alignedAdress;
		}


		/**
		 * Moves the memory marker back to a previous state.
		 * @param p_Marker The positon the stack should be rolled back to.
		 */
		void freeMarkerTo(UINT p_Marker);

		/**
		 * Resets the marker to the start of allocated memory.
		 */
		void clear(void);

		/**
		 * @return Returns where the marker currently is.
		 */
		UINT getMarker(void) const;

	private:
		LinearAllocator(const LinearAllocator& p_Other) = delete; // non construction-copyable
		LinearAllocator& operator=(const LinearAllocator&) = delete; // non copyable
		void *alignForward(void* p_Address, UINT p_Alignment);
		UINT alignForwardAdjustment(const void *Address, UINT p_Alignment);
	};
}

inline void* operator new (size_t p_Size, Allocator::LinearAllocator &p_Allocator)
{
	return p_Allocator.allocate(p_Size);
}