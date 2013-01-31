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
#ifndef containos_fixedlist_inl
#define containos_fixedlist_inl

namespace containos {

template<typename T,int Capasity>
inline FixedList<T,Capasity>::~FixedList()
{
    clear();
}

template<typename T,int Capasity>
inline FixedList<T,Capasity>::FixedList()
    : m_size(0)
{
}

template<typename T,int Capasity>
inline FixedList<T,Capasity>::FixedList(FixedList const& other)
    : m_size(other.m_size)
{
    for(int i = 0; i < other.m_size; ++i) {
        m_items[i] = other.m_items[i];
    }
}

template<typename T,int Capasity>
__forceinline T& FixedList<T,Capasity>::acquire()
{
    containos_assert(m_size < Capasity);
    return m_items[m_size++];
}

template<typename T,int Capasity>
__forceinline void FixedList<T,Capasity>::insert(T& item)
{
    containos_assert(m_size < Capasity);
    m_items[m_size] = item;
    ++m_size;
}

template<typename T,int Capasity>
__forceinline void FixedList<T,Capasity>::insert(T const& item)
{
    containos_assert(m_size < Capasity);
    m_items[m_size] = item;
    ++m_size;
}

template<typename T,int Capasity>
inline void FixedList<T,Capasity>::remove(int index)
{
    containos_assert(index < m_size);
    m_items[index] = m_items[--m_size];
}

template<typename T,int Capasity>
inline void FixedList<T,Capasity>::removeLast()
{
    containos_assert(m_size > 0);
    --m_size;
}

template<typename T,int Capasity>
__forceinline void FixedList<T,Capasity>::clear()
{
    m_size = 0;
}

template<typename T,int Capasity>
__forceinline T& FixedList<T,Capasity>::operator[](int index)
{
    containos_assert(index < m_size);
    return m_items[index];
}

template<typename T,int Capasity>
__forceinline T const& FixedList<T,Capasity>::operator[](int index) const
{
    containos_assert(index < m_size);
    return m_items[index];
}

template<typename T,int Capasity>
__forceinline int FixedList<T,Capasity>::size() const
{
    return m_size;
}

template<typename T,int Capasity>
__forceinline int FixedList<T,Capasity>::capasity() const
{
    return Capasity;
}

template<typename T,int Capasity>
__forceinline bool FixedList<T,Capasity>::isEmpty() const
{
    return m_size == 0;
}

template<typename T,int Capasity>
__forceinline bool FixedList<T,Capasity>::hasSpace() const
{
    return m_size < Capasity;
}

} // end of containos

#endif
