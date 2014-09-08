#pragma once

#include <utility>

namespace Allocator
{
	class DoubleEdgeAllocator
	{
	public:
		/**
		 * Flag indicating in which end of the buffer to store the data.
		*/
		enum class Edge
		{
			TOP,
			BOTTOM
		};
		typedef unsigned int UINT;

	private:
		UINT m_TopMarker;
		UINT m_BottomMarker;

		char *m_Buffer;
		UINT m_Size;
		bool m_Original;

	public:
		/**
		 * Throws MemoryException if it fails to allocate memory or if p_Size is zero.
		 *
		 * @param p_Size The amount of memory to be allocated in bytes.
		 */
		DoubleEdgeAllocator(UINT p_Size);

		/**
		 * Throws MemoryException if p_Buffer is nullptr or if p_Size is zero.
		 *
		 * @param p_Buffer An already allocated memory chunk.
		 * @param p_Size The amount of memory p_Buffer allocates.
		 */
		DoubleEdgeAllocator(char *p_Buffer, UINT p_Size);

		/**
		 * Default destructor.
		 */
		~DoubleEdgeAllocator(void);

		/**
		 * Allocates a new memory chunk.
		 *
		 * @param p_Size The amount of memory to be allocated in bytes.
		 * @param p_Edge The end part of the buffer the allocation should be made in.
		 * @return If able to allocate memory it returns a pointer to the allocated chunk.
		 * @return If failed to allocate memory it returns nullptr.
		 */
		void *allocate(UINT p_Size, DoubleEdgeAllocator::Edge p_Edge);

		/**
		 * Moves the top memory marker back to a previous state.
		 *
		 * @param p_Marker The positon the stack should be rolled back to.
		 */
		void freeTopMarkerTo(UINT p_Marker);

		/**
		 * Moves the bottom memory marker back to a previous state.
		 * 
		 * @param p_Marker The positon the stack should be rolled back to.
		 */
		void freeBottomMarkerTo(UINT p_Marker);

		/**
		 * Resets the marker to the start of allocated memory.
		 */
		void clear(void);

		/**
		 * Gets the position of the top marker.
		 *
		 * @return where the top marker currently is.
		 */
		UINT  getTopMarker(void) const;

		/**
		 * Gets the position of the bottom marker.
		 *
		 * @return where the bottom marker currently is.
		 */
		UINT  getBottomMarker(void) const;
	};
}

inline void* operator new (size_t p_Size, Allocator::DoubleEdgeAllocator &p_Allocator, Allocator::DoubleEdgeAllocator::Edge p_Edge)
{
	return p_Allocator.allocate(p_Size, p_Edge);
}