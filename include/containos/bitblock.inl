/*=============================================================================

Copyright (c) 2007-2013 Ville Ruusutie

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is furnished
to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

=============================================================================*/
#pragma once
#ifndef containos_bitblock_inl
#define containos_bitblock_inl

namespace containos {

template<typename T>
inline BitBlock<T>::~BitBlock()
{
	clear();
}

template<typename T>
inline BitBlock<T>::BitBlock()
{
}

template<typename T>
inline BitBlock<T>::BitBlock(const BitBlock& other)
{
	// TODO
}

template<typename T>
inline T& BitBlock<T>::acquire()
{
	T* ptr = &m_data[m_free.acquire()];
	containos_placement_new(ptr, T);
	return *ptr;
}

template<typename T>
inline void BitBlock<T>::insert(T& item)
{
	containos_placement_copy(&m_data[m_free.acquire()], T, item);
}

template<typename T>
inline void BitBlock<T>::insert(T const& item)
{
	containos_placement_copy(&m_data[m_free.acquire()], T, item);
}

template<typename T>
inline void BitBlock<T>::insert(const BitBlock& other)
{
	// TODO
}

template<typename T>
inline void BitBlock<T>::remove(size_t index)
{
	containos_assert(m_free.isSet(index));
	containos_placement_delete(&m_data[index], T);
	m_free.remove(index);
}

template<typename T>
inline void BitBlock<T>::clear()
{
	uint32_t mask = 0;//m_free;
	while(mask != 0) {
		if((mask & 1) == 0)
			continue;
		//containos_placement_delete(m_data[0]);
	}
}

template<typename T>
__forceinline size_t BitBlock<T>::size() const
{
	return m_free.count();
}

template<typename T>
__forceinline size_t BitBlock<T>::capasity() const
{
	return 32;
}

} // end of containos

#endif
