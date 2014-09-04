#pragma once

namespace Allocator
{
	class LinearAllocator
	{
	public:
		typedef unsigned int UINT;

	private:
		UINT m_Marker;
		UINT m_Size;
		char *m_Buffer;
	public:
	public:
		LinearAllocator(UINT p_Size);
		LinearAllocator(char *p_Buffer, UINT p_Size);
		~LinearAllocator();

		void *alloc(UINT p_Size);
		void freeToMarker(const UINT &p_Marker);
		void clear();

		const UINT &getMarker() const;

	private:
	};
}

