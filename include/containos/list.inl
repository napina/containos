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
#ifndef containos_list_inl
#define containos_list_inl

#pragma warning(disable:4127)

namespace containos {

template<typename T,typename GrowRule,typename Allocator>
__forceinline List<T,GrowRule,Allocator>::~List()
{
    clearAndFree();
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline List<T,GrowRule,Allocator>::List()
    : Base()
    , m_mem(nullptr)
    , m_size(0)
    , m_capasity(0)
{
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline List<T,GrowRule,Allocator>::List(size_t capasity)
    : Base()
    , m_mem(nullptr)
    , m_size(0)
    , m_capasity(0)
{
    reserve(capasity);
}

template<typename T,typename GrowRule,typename Allocator>
template<typename Allocator2, typename GrowRule2>
inline List<T,GrowRule,Allocator>::List(List<T,GrowRule2,Allocator2> const& other)
    : Base()
    , m_mem(nullptr)
    , m_size(0)
    , m_capasity(0)
{
    copy(other);
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline void List<T,GrowRule,Allocator>::operator=(List<T,GrowRule,Allocator> const& other)
{
    clearAndFree();
    copy(other);
}

template<typename T,typename GrowRule,typename Allocator>
template<typename Allocator2, typename GrowRule2>
__forceinline void List<T,GrowRule,Allocator>::operator=(List<T,GrowRule2,Allocator2> const& other)
{
    clearAndFree();
    copy(other);
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline T& List<T,GrowRule,Allocator>::acquire()
{
    if(GrowRule::enabled && m_size >= m_capasity) {
        reserve(m_capasity + GrowRule::count);
    }
    containos_assert(m_size < m_capasity);
    containos_placement_new(&m_mem[m_size], T);
    return m_mem[m_size++];
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline void List<T,GrowRule,Allocator>::insert(T& item)
{
    if(GrowRule::enabled && m_size >= m_capasity) {
        reserve(m_capasity + GrowRule::count);
    }
    containos_assert(m_size < m_capasity);
    containos_placement_copy(&m_mem[m_size], T, item);
    ++m_size;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline void List<T,GrowRule,Allocator>::insert(T const& item)
{
    if(GrowRule::enabled && m_size >= m_capasity) {
        reserve(m_capasity + GrowRule::count);
    }
    containos_assert(m_size < m_capasity);
    containos_placement_copy(&m_mem[m_size], T, item);
    ++m_size;
}

template<typename T,typename GrowRule,typename Allocator>
template<typename Allocator2, typename GrowRule2>
inline void List<T,GrowRule,Allocator>::insert(List<T,GrowRule2,Allocator2> const& other)
{
    size_t count = other.size();
    if(GrowRule::enabled && (m_size + count) >= m_capasity) {
        reserve(m_capasity + count + GrowRule::count);
    }
    containos_assert((m_size + count) <= m_capasity);
    for(size_t i = 0; i < count; ++i) {
        containos_placement_copy(&m_mem[m_size++], T, other[i]);
    }
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline void List<T,GrowRule,Allocator>::remove(size_t index)
{
    containos_assert(index < m_size);
    containos_placement_delete(&m_mem[index], T);
    --m_size;
    //m_mem[index] = m_mem[m_size];
    containos_memcpy(&m_mem[index], &m_mem[m_size], sizeof(T));
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline void List<T,GrowRule,Allocator>::remove(iterator& ite)
{
    containos_assert(ite != end());
    ptrdiff_t index = ite - m_mem;
    containos_placement_delete(&m_mem[index], T);
    --m_size;
    containos_memcpy(&m_mem[index], &m_mem[m_size], sizeof(T));
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline void List<T,GrowRule,Allocator>::removeLast()
{
    containos_assert(m_size > 0);
    --m_size;
    containos_placement_delete(&m_mem[m_size], T);
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline bool List<T,GrowRule,Allocator>::pop(T& result)
{
    if(m_size > 0) {
        --m_size;
        containos_placement_delete(&result, T);
        containos_placement_copy(&result, T, m_mem[m_size]);
        containos_placement_delete(&m_mem[m_size], T);
        return true;
    }
    return false;
}

template<typename T,typename GrowRule,typename Allocator>
inline void List<T,GrowRule,Allocator>::resize(size_t newSize)
{
    T* newMem = nullptr;
    size_t copyCount = m_size < newSize ? m_size : newSize;
    if(newSize > 0) {
        newMem = Base::template constructArray<T>(newSize);
        if(copy_rule::allowed) {
            containos_memcpy(newMem, m_mem, copyCount * sizeof(T));
            for(size_t i = copyCount; i < newSize; ++i) {
                containos_placement_new(&newMem[i], T);
            }
        } else {
            for(size_t i = 0; i < copyCount; ++i) {
                containos_placement_copy(&newMem[i], T, m_mem[i]);
            }
            for(size_t i = copyCount; i < newSize; ++i) {
                containos_placement_new(&newMem[i], T);
            }
            copyCount = 0;
        }
    }
    for(size_t i = copyCount; i < m_size; ++i) {
        containos_placement_delete(&m_mem[i], T);
    }
    Base::template destructArray<T>(m_mem, m_capasity);

    m_mem = newMem;
    m_capasity = newSize;
    m_size = newSize;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline void List<T,GrowRule,Allocator>::resize(iterator& newEnd)
{
    ptrdiff_t size = newEnd - m_mem;
    resize(size);
}

template<typename T,typename GrowRule,typename Allocator>
inline void List<T,GrowRule,Allocator>::resizeNoCopy(size_t newSize)
{
    clearAndFree();

    if(newSize == 0)
        return;

    m_mem = Base::template constructArray<T>(newSize);
    m_capasity = newSize;
    m_size = newSize;
    for(size_t i = 0; i < newSize; ++i) {
        containos_placement_new(&m_mem[i], T);
    }
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline void List<T,GrowRule,Allocator>::resizeNoCopy(iterator& newEnd)
{
    ptrdiff_t size = newEnd - m_mem;
    resizeNoCopy(size);
}

template<typename T,typename GrowRule,typename Allocator>
inline void List<T,GrowRule,Allocator>::reserve(size_t newCapasity)
{
    if(newCapasity <= m_capasity)
        return;

    T* newMem = Base::template constructArray<T>(newCapasity);
    if(m_mem != nullptr) {
        if(copy_rule::allowed) {
            containos_memcpy(newMem, m_mem, m_size * sizeof(T));
        } else {
            for(size_t i = 0; i < m_size; ++i) {
                containos_placement_copy(&newMem[i], T, m_mem[i]);
            }
        }
        for(size_t i = 0; i < m_size; ++i) {
            containos_placement_delete(&m_mem[i], T);
        }
        Base::template destructArray<T>(m_mem, m_capasity);
    }
    m_mem = newMem;
    m_capasity = newCapasity;
}

template<typename T,typename GrowRule,typename Allocator>
inline void List<T,GrowRule,Allocator>::clearAndFree()
{
    if(m_mem == nullptr)
        return;

    for(size_t i = 0; i < m_size; ++i) {
        containos_placement_delete(&m_mem[i], T);
    }
    Base::template destructArray<T>(m_mem, m_capasity);
    m_mem = nullptr;
    m_size = 0;
    m_capasity = 0;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline void List<T,GrowRule,Allocator>::clear()
{
    for(size_t i = 0; i < m_size; ++i) {
        containos_placement_delete(&m_mem[i], T);
    }
    m_size = 0;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline typename List<T,GrowRule,Allocator>::iterator List<T,GrowRule,Allocator>::begin()
{
    return m_mem;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline typename List<T,GrowRule,Allocator>::iterator List<T,GrowRule,Allocator>::end()
{
    return m_mem + m_size;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline typename List<T,GrowRule,Allocator>::const_iterator List<T,GrowRule,Allocator>::begin() const
{
    return m_mem;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline typename List<T,GrowRule,Allocator>::const_iterator List<T,GrowRule,Allocator>::end() const
{
    return m_mem + m_size;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline T& List<T,GrowRule,Allocator>::operator[](size_t index)
{
    containos_assert(index < m_size);
    return m_mem[index];
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline T const& List<T,GrowRule,Allocator>::operator[](size_t index) const
{
    containos_assert(index < m_size);
    return m_mem[index];
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline T const* List<T,GrowRule,Allocator>::mem() const
{
    return m_mem;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline T const& List<T,GrowRule,Allocator>::last() const
{
    containos_assert(m_size > 0);
    return m_mem[m_size - 1];
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline size_t List<T,GrowRule,Allocator>::size() const
{
    return m_size;
}

template<typename T,typename GrowRule,typename Allocator>
__forceinline size_t List<T,GrowRule,Allocator>::capasity() const
{
    return m_capasity;
}

template<typename T,typename GrowRule,typename Allocator>
template<typename Allocator2,typename GrowRule2>
inline void List<T,GrowRule,Allocator>::copy(List<T,GrowRule2,Allocator2> const& other)
{
    if(other.m_size > 0) {
        m_mem = Base::template constructArray<T>(other.m_size);
        m_capasity = other.m_size;
        m_size = other.m_size;
        for(size_t i = 0; i < m_size; ++i) {
            containos_placement_copy(&m_mem[i], T, other.m_mem[i]);
        }
    }
}

} // end of containos

#endif
