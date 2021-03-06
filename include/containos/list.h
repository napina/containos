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
#ifndef containos_list_h
#define containos_list_h

#include "containos/common.h"

namespace containos {

template<int Count>
struct ListGrowRule {
    static const bool enabled = Count > 0;
    static const int count = Count;
};

class Allocator;

// Simple continuous list of items
template<typename T,typename GrowRule=ListGrowRule<32>>
class List
{
public:
    typedef T* iterator;
    typedef T const* const_iterator;
    typedef allow_memcpy<T> copy_rule;

    ~List();
    List();
    List(size_t capacity);

    void operator=(List<T,GrowRule> const& other);
    template<typename GrowRule2> List(List<T,GrowRule2> const& other);
    template<typename GrowRule2> void operator=(List<T,GrowRule2> const& other);

    T& acquire();
    void insert(T& item);
    void insert(T const& item);
    template<typename GrowRule2> void insert(List<T,GrowRule2> const& other);
    void remove(size_t index);
    void remove(iterator& ite);
    void removeLast();
    bool pop(T& result);

    void resize(size_t newSize);
    void resize(iterator& newEnd);
    void resizeNoCopy(size_t newSize);
    void resizeNoCopy(iterator& newEnd);
    void reserve(size_t capacity);
    void reserve(size_t capacity, Allocator* allocator);
    void clearAndFree();
    void clear();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    T& operator[](size_t index);
    T const& operator[](size_t index) const;
    T const* mem() const;
    T const& last() const;
    size_t size() const;
    size_t capacity() const;
    Allocator* allocator();

private:
    template<typename GrowRule2> void copy(List<T,GrowRule2> const& other);

    T* m_mem;
    size_t m_size;
    size_t m_capacity;
    Allocator* m_allocator;
};

} // end of containos

#include "containos/list.inl"

#endif
