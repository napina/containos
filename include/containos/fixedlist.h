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
#ifndef containos_fixedlist_h
#define containos_fixedlist_h

#include "containos\config.h"

namespace containos {

template<typename T,int Capasity>
class FixedList
{
public:
    ~FixedList();
    FixedList();
    FixedList(FixedList const& other);

    T& acquire();
    void insert(T& item);
    void insert(T const& item);
    void remove(int index);
    void removeLast();
    void clear();

    T& operator[](int index);
    T const& operator[](int index) const;
    int capasity() const;
    int size() const;
    bool isEmpty() const;
    bool hasSpace() const;

private:
    T m_items[Capasity];
    int m_size;
};

} // end of containos

#include "containos/fixedlist.inl"

#endif
