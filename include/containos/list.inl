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

#include "containos/allocator.h"
#include <memory.h>

#pragma warning(disable:4127)

namespace containos {

template<typename T,typename GrowRule>
__forceinline List<T,GrowRule>::~List()
{
    clearAndFree();
}

template<typename T,typename GrowRule>
__forceinline List<T,GrowRule>::List()
    : m_mem(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_allocator(DefaultAllocator::instance())
{
}

template<typename T,typename GrowRule>
__forceinline List<T,GrowRule>::List(size_t capacity)
    : m_mem(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_allocator(DefaultAllocator::instance())
{
    reserve(capacity);
}

template<typename T,typename GrowRule>
template<typename GrowRule2>
inline List<T,GrowRule>::List(List<T,GrowRule2> const& other)
    : m_mem(nullptr)
    , m_size(0)
    , m_capacity(0)
    , m_allocator(DefaultAllocator::instance())
{
    copy(other);
}

template<typename T,typename GrowRule>
__forceinline void List<T,GrowRule>::operator=(List<T,GrowRule> const& other)
{
    clearAndFree();
    copy(other);
}

template<typename T,typename GrowRule>
template<typename GrowRule2>
__forceinline void List<T,GrowRule>::operator=(List<T,GrowRule2> const& other)
{
    clearAndFree();
    copy(other);
}

template<typename T,typename GrowRule>
__forceinline T& List<T,GrowRule>::acquire()
{
    if(GrowRule::enabled && m_size >= m_capacity) {
        reserve(m_capacity + GrowRule::count);
    }
    containos_assert(m_size < m_capacity);
    containos_placement_new(&m_mem[m_size], T);
    return m_mem[m_size++];
}

template<typename T,typename GrowRule>
__forceinline void List<T,GrowRule>::insert(T& item)
{
    if(GrowRule::enabled && m_size >= m_capacity) {
        reserve(m_capacity + GrowRule::count);
    }
    containos_assert(m_size < m_capacity);
    containos_placement_copy(&m_mem[m_size], T, item);
    ++m_size;
}

template<typename T,typename GrowRule>
__forceinline void List<T,GrowRule>::insert(T const& item)
{
    if(GrowRule::enabled && m_size >= m_capacity) {
        reserve(m_capacity + GrowRule::count);
    }
    containos_assert(m_size < m_capacity);
    containos_placement_copy(&m_mem[m_size], T, item);
    ++m_size;
}

template<typename T,typename GrowRule>
template<typename GrowRule2>
inline void List<T,GrowRule>::insert(List<T,GrowRule2> const& other)
{
    size_t count = other.size();
    if(GrowRule::enabled && (m_size + count) >= m_capacity) {
        reserve(m_capacity + count + GrowRule::count);
    }
    containos_assert((m_size + count) <= m_capacity);
    for(size_t i = 0; i < count; ++i) {
        containos_placement_copy(&m_mem[m_size++], T, other[i]);
    }
}

template<typename T,typename GrowRule>
__forceinline void List<T,GrowRule>::remove(size_t index)
{
    containos_assert(index < m_size);
    containos_placement_delete(&m_mem[index], T);
    --m_size;
    //m_mem[index] = m_mem[m_size];
    containos_memcpy(&m_mem[index], &m_mem[m_size], sizeof(T));
}

template<typename T,typename GrowRule>
__forceinline void List<T,GrowRule>::remove(iterator& ite)
{
    containos_assert(ite != end());
    ptrdiff_t index = ite - m_mem;
    containos_placement_delete(&m_mem[index], T);
    --m_size;
    containos_memcpy(&m_mem[index], &m_mem[m_size], sizeof(T));
}

template<typename T,typename GrowRule>
__forceinline void List<T,GrowRule>::removeLast()
{
    containos_assert(m_size > 0);
    --m_size;
    containos_placement_delete(&m_mem[m_size], T);
}

template<typename T,typename GrowRule>
__forceinline bool List<T,GrowRule>::pop(T& result)
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

template<typename T,typename GrowRule>
inline void List<T,GrowRule>::resize(size_t newSize)
{
    T* newMem = nullptr;
    size_t copyCount = m_size < newSize ? m_size : newSize;
    if(newSize > 0) {
        newMem = (T*)m_allocator->alloc(sizeof(T) * newSize,__alignof(T));
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
    m_allocator->dealloc(m_mem);

    m_mem = newMem;
    m_capacity = newSize;
    m_size = newSize;
}

template<typename T,typename GrowRule>
__forceinline void List<T,GrowRule>::resize(iterator& newEnd)
{
    ptrdiff_t size = newEnd - m_mem;
    resize(size);
}

template<typename T,typename GrowRule>
inline void List<T,GrowRule>::resizeNoCopy(size_t newSize)
{
    clearAndFree();

    if(newSize == 0)
        return;

    m_mem = (T*)m_allocator->alloc(sizeof(T) * newSize,__alignof(T));
    m_capacity = newSize;
    m_size = newSize;
    for(size_t i = 0; i < newSize; ++i) {
        containos_placement_new(&m_mem[i], T);
    }
}

template<typename T,typename GrowRule>
__forceinline void List<T,GrowRule>::resizeNoCopy(iterator& newEnd)
{
    ptrdiff_t size = newEnd - m_mem;
    resizeNoCopy(size);
}

template<typename T,typename GrowRule>
inline void List<T,GrowRule>::reserve(size_t newCapacity)
{
    reserve(newCapacity, DefaultAllocator::instance());
}

template<typename T,typename GrowRule>
inline void List<T,GrowRule>::reserve(size_t newCapacity, Allocator* allocator)
{
    if(newCapacity <= m_capacity)
        return;

    T* newMem = (T*)allocator->alloc(sizeof(T) * newCapacity,__alignof(T));
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
        m_allocator->dealloc(m_mem);
    }
    m_mem = newMem;
    m_capacity = newCapacity;
    m_allocator = allocator;
}

template<typename T,typename GrowRule>
inline void List<T,GrowRule>::clearAndFree()
{
    if(m_mem == nullptr)
        return;

    for(size_t i = 0; i < m_size; ++i) {
        containos_placement_delete(&m_mem[i], T);
    }
    m_allocator->dealloc(m_mem);
    m_mem = nullptr;
    m_size = 0;
    m_capacity = 0;
}

template<typename T,typename GrowRule>
__forceinline void List<T,GrowRule>::clear()
{
    for(size_t i = 0; i < m_size; ++i) {
        containos_placement_delete(&m_mem[i], T);
    }
    m_size = 0;
}

template<typename T,typename GrowRule>
__forceinline typename List<T,GrowRule>::iterator List<T,GrowRule>::begin()
{
    return m_mem;
}

template<typename T,typename GrowRule>
__forceinline typename List<T,GrowRule>::iterator List<T,GrowRule>::end()
{
    return m_mem + m_size;
}

template<typename T,typename GrowRule>
__forceinline typename List<T,GrowRule>::const_iterator List<T,GrowRule>::begin() const
{
    return m_mem;
}

template<typename T,typename GrowRule>
__forceinline typename List<T,GrowRule>::const_iterator List<T,GrowRule>::end() const
{
    return m_mem + m_size;
}

template<typename T,typename GrowRule>
__forceinline T& List<T,GrowRule>::operator[](size_t index)
{
    containos_assert(index < m_size);
    return m_mem[index];
}

template<typename T,typename GrowRule>
__forceinline T const& List<T,GrowRule>::operator[](size_t index) const
{
    containos_assert(index < m_size);
    return m_mem[index];
}

template<typename T,typename GrowRule>
__forceinline T const* List<T,GrowRule>::mem() const
{
    return m_mem;
}

template<typename T,typename GrowRule>
__forceinline T const& List<T,GrowRule>::last() const
{
    containos_assert(m_size > 0);
    return m_mem[m_size - 1];
}

template<typename T,typename GrowRule>
__forceinline size_t List<T,GrowRule>::size() const
{
    return m_size;
}

template<typename T,typename GrowRule>
__forceinline size_t List<T,GrowRule>::capacity() const
{
    return m_capacity;
}

template<typename T,typename GrowRule>
__forceinline Allocator* List<T,GrowRule>::allocator()
{
    return m_allocator;
}

template<typename T,typename GrowRule>
template<typename GrowRule2>
inline void List<T,GrowRule>::copy(List<T,GrowRule2> const& other)
{
    if(other.m_size > 0) {
        m_allocator = other.m_allocator;
        m_mem = (T*)m_allocator->alloc(sizeof(T) * other.m_size,__alignof(T));
        m_capacity = other.m_size;
        m_size = other.m_size;
        for(size_t i = 0; i < m_size; ++i) {
            containos_placement_copy(&m_mem[i], T, other.m_mem[i]);
        }
    }
}

} // end of containos

#endif
