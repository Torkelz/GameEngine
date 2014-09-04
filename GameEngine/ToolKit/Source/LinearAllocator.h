#pragma once

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
		UINT m_Marker;

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
	public:
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

		~LinearAllocator();

		/**
		 * @param p_Size The amount of memory to be allocated in bytes.
		 * @return If able to allocate memory it returns a pointer to the allocated chunk.
		 * @return If failed to allocate memory it returns nullptr.
		 */
		void *alloc(UINT p_Size);

		/**
		 * Moves the memory marker back to a previous state.
		 * @param p_Marker The positon the stack should be rolled back to.
		 */
		void freeToMarker(const UINT &p_Marker);

		/**
		 * Resets the marker to the start of allocated memory.
		 */
		void clear();

		/**
		 * @return Returns where the marker currently is.
		 */
		const UINT &getMarker() const;

	private:
	};
}

