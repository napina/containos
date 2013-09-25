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
#ifndef containos_bitblock_h
#define containos_bitblock_h

#include "containos/bitset.h"

namespace containos {

// Fast memory block with 32/64 elements depending which is native
template<typename T>
struct BitBlock
{
    struct iterator;
    struct const_iterator;

    ~BitBlock();
    BitBlock();
    BitBlock(BitBlock const& other);

    T& acquire(size_t& index);
    size_t insert(T& item);
    size_t insert(T const& item);
    void remove(size_t index);
    void remove(iterator const& i);
    void clear();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    T& operator[](size_t index);
    T const& operator[](size_t index) const;
    size_t size() const;
    size_t capasity() const;

private:
    bitset m_mask;
    char m_data[bitset::num_bits*sizeof(T)];
};

} // end of containos

#include "containos/bitblock.inl"

#endif
