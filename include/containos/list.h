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
#ifndef containos_list_h
#define containos_list_h

#include "containos/container.h"

namespace containos {

// Simple continuous list of items
// TODO add template specialization for auto grow
template<typename T, typename Allocator>// = DefaultAllocator>
class List : protected Container<Allocator>
{
    typedef Container<Allocator> Base;
public:
    typedef T* iterator;
    typedef T const* const_iterator;

    ~List();
    List();
    List(size_t capasity);
    List(const List& other);

    T& acquire();
    void insert(T& item);
    void insert(T const& item);
    template<typename Allocator2> void insert(List<T,Allocator2> const& other);
    void remove(int index);
    void remove(iterator ite);
    void removeLast();
    void clear();

    void resize(size_t newSize);
    void reserve(size_t capasity);
    void clearAndFree();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    T& operator[](size_t index);
    T const& operator[](size_t index) const;
    T const* mem() const;
    T const& last() const;
    size_t size() const;
    size_t capasity() const;

private:
    T* m_mem;
    size_t m_size;
    size_t m_capasity;
};

} // end of containos

#include "containos/list.inl"

#endif
