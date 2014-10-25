/*=============================================================================

Copyright (c) 2013 Ville Ruusutie

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
#ifndef containos_fixedlist_inl
#define containos_fixedlist_inl

namespace containos {

template<typename T,size_t Capacity>
inline FixedList<T,Capacity>::~FixedList()
{
    clear();
}

template<typename T,size_t Capacity>
inline FixedList<T,Capacity>::FixedList()
    : m_size(0)
{
}

template<typename T,size_t Capacity>
inline FixedList<T,Capacity>::FixedList(FixedList const& other)
    : m_size(other.m_size)
{
    for(size_t i = 0; i < other.m_size; ++i) {
        m_items[i] = other.m_items[i];
    }
}

template<typename T,size_t Capacity>
__forceinline T& FixedList<T,Capacity>::acquire()
{
    containos_assert(m_size < Capacity);
    return m_items[m_size++];
}

template<typename T,size_t Capacity>
__forceinline void FixedList<T,Capacity>::insert(T& item)
{
    containos_assert(m_size < Capacity);
    m_items[m_size] = item;
    ++m_size;
}

template<typename T,size_t Capacity>
__forceinline void FixedList<T,Capacity>::insert(T const& item)
{
    containos_assert(m_size < Capacity);
    m_items[m_size] = item;
    ++m_size;
}

template<typename T,size_t Capacity>
inline void FixedList<T,Capacity>::remove(size_t index)
{
    containos_assert(index < m_size);
    m_items[index] = m_items[--m_size];
}

template<typename T,size_t Capacity>
inline void FixedList<T,Capacity>::removeLast()
{
    containos_assert(m_size > 0);
    --m_size;
}

template<typename T,size_t Capacity>
__forceinline void FixedList<T,Capacity>::clear()
{
    m_size = 0;
}

template<typename T,size_t Capacity>
__forceinline T& FixedList<T,Capacity>::operator[](size_t index)
{
    containos_assert(index < m_size);
    return m_items[index];
}

template<typename T,size_t Capacity>
__forceinline T const& FixedList<T,Capacity>::operator[](size_t index) const
{
    containos_assert(index < m_size);
    return m_items[index];
}

template<typename T,size_t Capacity>
__forceinline size_t FixedList<T,Capacity>::size() const
{
    return m_size;
}

template<typename T,size_t Capacity>
__forceinline size_t FixedList<T,Capacity>::capacity() const
{
    return Capacity;
}

template<typename T,size_t Capacity>
__forceinline bool FixedList<T,Capacity>::isEmpty() const
{
    return m_size == 0;
}

template<typename T,size_t Capacity>
__forceinline bool FixedList<T,Capacity>::hasSpace() const
{
    return m_size < Capacity;
}

} // end of containos

#endif
