#pragma once

#include <utility>

namespace Allocator
{
	class DoubleEdgeAllocator
	{
	public:
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

	public:
		/**
		* @param p_Size The amount of memory to be allocated in bytes.
		* Throws MemoryException if it fails to allocate memory or if p_Size is zero.
		*/
		DoubleEdgeAllocator(UINT p_Size);

		/**
		* @param p_Buffer An already allocated memory chunk.
		* @param p_Size The amount of memory p_Buffer allocates.
		* Throws MemoryException if p_Buffer is nullptr or if p_Size is zero.
		*/
		DoubleEdgeAllocator(char *p_Buffer, UINT p_Size);

		~DoubleEdgeAllocator();

		/**
		* @param p_Size The amount of memory to be allocated in bytes.
		* @return If able to allocate memory it returns a pointer to the allocated chunk.
		* @return If failed to allocate memory it returns nullptr.
		*/
		void *alloc(UINT p_Size, DoubleEdgeAllocator::Edge p_Edge);

		/**
		* Moves the memory marker back to a previous state.
		* @param p_Marker The positon the stack should be rolled back to.
		*/
		void freeToTopMarker(const UINT  &p_Marker);

		/**
		* Moves the memory marker back to a previous state.
		* @param p_Marker The positon the stack should be rolled back to.
		*/
		void freeToBottomMarker(const UINT  &p_Marker);

		/**
		* Resets the marker to the start of allocated memory.
		*/
		void clear();

		/**
		* @return Returns where the Top marker currently is.
		*/
		UINT  getTopMarker() const;

		/**
		* @return Returns where the bottom marker currently is.
		*/
		UINT  getBottomMarker() const;
	};
}